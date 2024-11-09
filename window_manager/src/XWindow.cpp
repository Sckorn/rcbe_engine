#include "XWindow.hpp"

#include <future>

#include <boost/log/trivial.hpp>

#include <rcbe-engine/core/AbstractInputManager.hpp>
#include <rcbe-engine/core/EditorInputManager.hpp>
#include <rcbe-engine/core/GameInputManager.hpp>
#include <rcbe-engine/utils/json_utils.hpp>

GLXFBConfig *chooseFbConfig(Display *d, int screen) {
    int glAttrib[] = {
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER, True,
        None};
    int num;
    GLXFBConfig *ret = glXChooseFBConfig(d, screen, glAttrib, &num);
    if (ret == nullptr)
        throw std::runtime_error("Unable to find a frame buffer config!");

    return ret;
}

namespace rcbe::core {
XWindow::XWindow(window_config &&config, const WindowContextPtr &window_context)
    : config_ {std::move(config)}
    , root_display_(window_context->getRootDisplay())
    , rendering_context_ {std::make_shared<rendering::RenderingContext>()} {
    using window_type_enum = decltype(config_.type);
    const std::unordered_map<window_type_enum, WindowCreateHandler> handlers = {
        {window_type_enum::drawing_window, [this](const WindowContextPtr &window_context) -> bool { return this->createSimpleDrawingWindow(window_context); }},
        {window_type_enum::gl_rendering_window, [this](const WindowContextPtr &window_context) -> bool { return this->createRasterizerWindow(window_context); }},
        {window_type_enum::unknown, [](const WindowContextPtr &) -> bool { return false; }}};

    if (!handlers.at(config_.type)(window_context))
        throw std::runtime_error("Can't create window of type " + std::to_string(static_cast<size_t>(config_.type)));
}

XWindow::~XWindow() {
    if (!killed_)
        kill();
}

bool XWindow::createRasterizerWindow(const WindowContextPtr &window_context) {
    GLXFBConfig *fbConfigs = chooseFbConfig(root_display_, window_context->getScreenNumber());
    XVisualInfo *visInfo = glXGetVisualFromFBConfig(root_display_, fbConfigs[0]);

    if (visInfo == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "VisualInfo is null pointer";
        return false;
    }

    rendering_context_->setVisualId(visInfo->visualid);
    rendering_context_->setDisplay(XOpenDisplay(nullptr));
    rendering_context_->setBackgroundColor(config_.background_color);
    rendering_context_->setWindowDimensions(config_.size);
    rendering_context_->setDeleteMessage(window_context->getDeleteMsg());

    attributes_.colormap = XCreateColormap(
        rendering_context_->getDisplay(),
        window_context->getRootWindow(),
        visInfo->visual, AllocNone);

    rendering_context_->setDrawable(XCreateWindow(rendering_context_->getDisplay(), window_context->getRootWindow(),
                                                  config_.position.x(), config_.position.y(),
                                                  config_.size.width, config_.size.height, 0,
                                                  visInfo->depth, InputOutput, visInfo->visual, CWColormap,
                                                  &attributes_));

    rendering_context_->setGlxContext(
        glXCreateContext(rendering_context_->getDisplay(), visInfo, nullptr, true));
    if (rendering_context_->getGlxContext() == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "GLX create context is NULL";
        return false;
    }

    XSetStandardProperties(
        rendering_context_->getDisplay(),
        rendering_context_->getDrawable(),
        config_.caption.c_str(),
        "", None, nullptr, 0, nullptr);

    // TODO: remove this creation, input manager should be manually set from the outside
    if (config_.process_input) {
        if (config_.editor) {
            input_manager_ = std::make_shared<AbstractInputManager>(EditorInputManager(EditorInputManager::HandlerCollection {}));
        } else {
            if (!config_.input_scheme.empty()) {
                input_manager_ = std::make_shared<AbstractInputManager>(GameInputManager(utils::read_raw(config_.input_scheme)));
            } else {
                BOOST_LOG_TRIVIAL(error) << "Input requested but no input scheme provided!";
                return false;
            }
        }
    }

    auto mask = ExposureMask | StructureNotifyMask;
    if (config_.process_input)
        mask = mask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | PointerMotionMask;

    XSelectInput(root_display_, rendering_context_->getDrawable(), mask);
    auto delete_message = rendering_context_->getDeleteMessage();

    XSetWMProtocols(root_display_, rendering_context_->getDrawable(), &(delete_message), 1);

    XFree(visInfo);
    XFree(fbConfigs);
    XSync(rendering_context_->getDisplay(), false);

    return true;
}

bool XWindow::createSimpleDrawingWindow(const WindowContextPtr &window_context) {
    rendering_context_->setDisplay(XOpenDisplay(nullptr));
    rendering_context_->setBackgroundColor(config_.background_color);
    rendering_context_->setWindowDimensions(config_.size);
    rendering_context_->setDeleteMessage(window_context->getDeleteMsg());

    unsigned long black, white;
    black = BlackPixel(rendering_context_->getDisplay(), window_context->getScreenNumber()); /* get color black */
    white = WhitePixel(rendering_context_->getDisplay(), window_context->getScreenNumber()); /* get color white */

    rendering_context_->setDrawable(XCreateSimpleWindow(rendering_context_->getDisplay(), window_context->getRootWindow(),
                                                        config_.position.x(), config_.position.y(),
                                                        config_.size.width, config_.size.height, 5, black, white));

    XSetStandardProperties(
        rendering_context_->getDisplay(),
        rendering_context_->getDrawable(),
        config_.caption.c_str(),
        "", None, nullptr, 0, nullptr);

    auto mask = ExposureMask | StructureNotifyMask;
    if (config_.process_input)
        mask = mask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | PointerMotionMask;

    XSelectInput(rendering_context_->getDisplay(), rendering_context_->getDrawable(), mask);

    gc_ = XCreateGC(rendering_context_->getDisplay(), rendering_context_->getDrawable(), 0, nullptr);

    XSetBackground(rendering_context_->getDisplay(), gc_, white);
    XSetForeground(rendering_context_->getDisplay(), gc_, black);

    XClearWindow(rendering_context_->getDisplay(), rendering_context_->getDrawable());
    XMapRaised(rendering_context_->getDisplay(), rendering_context_->getDrawable());

    return true;
}

void XWindow::onConfigure(window::ConfigureHandlerType &&handler) {
    configure_handler_ = std::move(handler);
}

void XWindow::onUnmap(window::UunmapHandlerType &&handler) {
    unmap_handler_ = std::move(handler);
}

void XWindow::windowLoop() {

    std::thread worker {
        [this]() {
            while (running_) {
                XEvent event;
                if (XPending(rendering_context_->getDisplay())) {
                    XNextEvent(rendering_context_->getDisplay(), &event);
                    if (event.type == ClientMessage && static_cast<unsigned int>(event.xclient.data.l[0]) ==
                                                           rendering_context_->getDeleteMessage()) {
                        BOOST_LOG_TRIVIAL(info) << "Window close event received.";
                        break;
                    } else if (event.type == Expose && event.xexpose.count == 0 && config_.type == decltype(config_.type)::drawing_window) {
                        BOOST_LOG_TRIVIAL(info) << "Window exposure event received.";
                        // redraw X11 window
                        XClearWindow(rendering_context_->getDisplay(), rendering_context_->getDrawable());
                    }

                    if (config_.type == decltype(config_.type)::drawing_window) {
                        if (config_.process_input)
                            if (!input_manager_->tryProcessEvent(event))
                                BOOST_LOG_TRIVIAL(warning) << "Event handler is absent! Event won't be handled!";
                    }
                }
            }

            if (renderer_->running())
                renderer_->stop();

            if (!killed_) {
                kill();
            }
        }};

    while (running_) {
        XEvent event;
        std::lock_guard lg {input_event_mutex_};
        XNextEvent(root_display_, &event);

        if (config_.process_input)
            if (!input_manager_->tryProcessEvent(event))
                switch (event.type) {
                    // Structure notify
                    case ConfigureNotify: {// size or position change

                        BOOST_LOG_TRIVIAL(info) << "configure win " << rendering_context_->getDrawable() << ": pos [" << event.xconfigurerequest.x << " " << event.xconfigurerequest.y << "], size [" << event.xconfigurerequest.width << " "
                                                << event.xconfigurerequest.height << "]";
                        // Warning this is not thread safe
                        rendering_context_->setWindowDimensions({event.xconfigurerequest.width, event.xconfigurerequest.height});

                        if (configure_handler_)
                            configure_handler_();

                        if (renderer_)
                            renderer_->reshape();
                    } break;
                    case CirculateNotify:
                        BOOST_LOG_TRIVIAL(info) << "CirculateNotify";
                        break;
                    case DestroyNotify: {
                        BOOST_LOG_TRIVIAL(warning) << "DestroyNotify event signals that a running window was killed_, in order for window to stop gracefully use stop_window_loop!";
                        stopWindowLoop();
                    } break;
                    case GravityNotify:
                        BOOST_LOG_TRIVIAL(info) << "GravityNotify";
                        break;
                    case ReparentNotify:
                        BOOST_LOG_TRIVIAL(info) << "ReparentNotify";
                        break;
                    case MapNotify:
                        BOOST_LOG_TRIVIAL(info) << "Map win " << event.xmap.window;
                        break;
                    case UnmapNotify: {
                        std::lock_guard lg2 {kill_mutex_};
                        BOOST_LOG_TRIVIAL(info) << "UnMap win " << event.xunmap.window;
                        if (unmap_handler_)
                            unmap_handler_();

#ifdef RDMN_OPENGL
                        if (rendering_context_)
                            rendering_context_->glContextFromDefault();
#endif

                        if (renderer_ && renderer_->running())
                            renderer_->stop();

                        if (running_)
                            stopWindowLoop();
                    } break;
                    case ClientMessage: {
                        BOOST_LOG_TRIVIAL(info) << "Client message";
                    } break;
                    case Expose: {
                        BOOST_LOG_TRIVIAL(info) << "Exposure event";
                        if (configure_handler_)
                            configure_handler_();
                    } break;
                        // Unhandled message
                    default: {
                        BOOST_LOG_TRIVIAL(info) << "Type " << event.type;
                    } break;
                }
    }

    if (worker.joinable()) {
        worker.join();
    }
}

void XWindow::kill() {
    std::lock_guard lg {kill_mutex_};
    if (!killed_) {
        if (config_.type == decltype(config_.type)::gl_rendering_window)
            glXDestroyContext(rendering_context_->getDisplay(), rendering_context_->getGlxContext());
        else
            XFreeGC(rendering_context_->getDisplay(), gc_);
        XDestroyWindow(rendering_context_->getDisplay(), rendering_context_->getDrawable());
        XCloseDisplay(rendering_context_->getDisplay());
        if (renderer_)
            renderer_->stop();
        if (running_)
            stopWindowLoop();

        killed_ = true;
    }
}

void XWindow::show() {
    XMapWindow(rendering_context_->getDisplay(), rendering_context_->getDrawable());
    BOOST_LOG_TRIVIAL(debug) << "Window should be visible";
}

const std::shared_ptr<AbstractInputManager> &XWindow::getInputManager() const {
    std::lock_guard lg {input_manager_access_mutex_};
    return input_manager_;
}

const window_config &XWindow::getConfig() const {
    return config_;
}

const std::shared_ptr<rendering::RenderingContext> &XWindow::getRenderingContext() const {
    return rendering_context_;
}

std::optional<GC> XWindow::getGraphicContext() const {
    if (config_.type == decltype(config_.type)::drawing_window) {
        return {gc_};
    }

    return std::nullopt;
}

void XWindow::startWindowLoop() {
    {
        bool current_expected = false;
        running_.compare_exchange_strong(current_expected, true);
        if (current_expected)
            BOOST_LOG_TRIVIAL(warning) << "WindowLoop is already running!";
    }

    windowLoop();
}

void XWindow::stopWindowLoop() {
    bool current_expected = true;
    running_.compare_exchange_strong(current_expected, false);
    if (!current_expected)
        BOOST_LOG_TRIVIAL(warning) << "Window loop already stopped";
}

const rdmn::render::RendererPtr &XWindow::getRenderer() const {
    std::lock_guard lg {renderer_access_mutex_};
    return renderer_;
}

void XWindow::setRenderer(rdmn::render::RendererPtr renderer_ptr) {
    std::lock_guard lg {renderer_access_mutex_};
    if (renderer_ptr == nullptr)
        throw std::runtime_error("Null pointer assigned into renderer ptr");

    renderer_ = std::move(renderer_ptr);
}

void XWindow::setInputManager(const std::shared_ptr<AbstractInputManager> &manager) {
    std::lock_guard lg {input_manager_access_mutex_};
    input_manager_ = manager;
}
}// namespace rcbe::core
