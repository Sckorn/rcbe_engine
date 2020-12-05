#include <future>

#include <boost/log/trivial.hpp>

#include <core/EditorInputManager.hpp>
#include <core/GameInputManager.hpp>
#include <core/AbstractInputManager.hpp>

#include <rcbe-engine/utils/json_utils.hpp>

#include "XWindow.hpp"

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
            None
    };
    int num;
    GLXFBConfig *ret = glXChooseFBConfig(d, screen, glAttrib, &num);
    if (ret == nullptr)
        throw std::runtime_error("Unable to find a frame buffer config!");

    return ret;
}

namespace rcbe::core {
XWindow::XWindow(window_config &&config, Display *root_display, int screen_number, const Window &root_window,
                 const Atom &delete_msg)
        :
        config_{std::move(config)}, root_display_(root_display)
        , rendering_context_{(config_.type == window_config::WindowType::gl_rendering_window) ? std::make_shared<rendering::RenderingContext>() : nullptr}
{
    GLXFBConfig *fbConfigs = chooseFbConfig(root_display_, screen_number);
    XVisualInfo *visInfo = glXGetVisualFromFBConfig(root_display_, fbConfigs[0]);

    if (visInfo == nullptr)
        throw std::runtime_error("VisualInfo is null pointer");

    rendering_context_->setDisplay(XOpenDisplay(NULL)); // TODO: use nullptr
    rendering_context_->setBackgroundColor(config_.background_color);
    rendering_context_->setWindowDimensions(config.size);
    rendering_context_->setDeleteMessage(delete_msg);

    attributes_.colormap = XCreateColormap(rendering_context_->getDisplay(), root_window, visInfo->visual, AllocNone);
    switch (config_.type) {
        case window_config::WindowType::gl_rendering_window: {
            rendering_context_->setDrawable(XCreateWindow(rendering_context_->getDisplay(), root_window,
                                                          config_.position.x(), config_.position.y(),
                                                          config_.size.width, config_.size.height, 0,
                                                          visInfo->depth, InputOutput, visInfo->visual, CWColormap,
                                                          &attributes_));
            rendering_context_->setGlxContext(
                    glXCreateContext(rendering_context_->getDisplay(), visInfo, nullptr, true));
            if (rendering_context_->getGlxContext() == nullptr)
                throw std::runtime_error("GLX create context is NULL");
        }
            break;
        case window_config::WindowType::drawing_window: {
            rendering_context_->setDrawable(XCreateSimpleWindow(rendering_context_->getDisplay(), root_window,
                                                                config_.position.x(), config_.position.y(),
                                                                config_.size.width, config_.size.height, 0, 0, 0));
        }
            break;
        case window_config::WindowType::unknown:
        default: {
            throw std::runtime_error("Unknown type of the window!");
        }
            break;
    }
    XSetStandardProperties(rendering_context_->getDisplay(), rendering_context_->getDrawable(), config_.caption.c_str(), "", None, nullptr, 0, nullptr);

    // TODO: remove this creation, input manager should be manually set from the outside
    if (config_.process_input) {
        if (config.editor) {
            input_manager_ = std::make_shared<AbstractInputManager>(EditorInputManager(EditorInputManager::handler_collection{}));
        } else {
            if (!config_.input_scheme.empty()) {
                input_manager_ = std::make_shared<AbstractInputManager>(GameInputManager(utils::read_raw(config_.input_scheme)));
            } else {
                throw std::runtime_error("Input requested but no input scheme provided!");
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
}

XWindow::~XWindow() {
    if (!killed_)
        kill();
}

void XWindow::on_configure(window::ConfigureHandlerType &&handler) {
    configure_handler_ = std::move(handler);
}

void XWindow::on_unmap(window::UunmapHandlerType &&handler) {
    unmap_handler_ = std::move(handler);
}

void XWindow::window_loop() {

    std::thread worker{
            [this]() {
                while (running_) {
                    XEvent event;
                    if (XPending(rendering_context_->getDisplay())) {
                        XNextEvent(rendering_context_->getDisplay(), &event);
                        if (event.type == ClientMessage && static_cast<unsigned int>(event.xclient.data.l[0]) ==
                                                                   rendering_context_->getDeleteMessage()) {
                            BOOST_LOG_TRIVIAL(info) << "Window close event received.";
                            break;
                        }
                    }
                }

                if (renderer_->running())
                    renderer_->stop();

                if (!killed_) {
                    kill();
                }
            }
    };

    while (running_) {
        XEvent event;
        std::lock_guard lg {input_event_mutex_};
        XNextEvent(root_display_, &event);

        if (! input_manager_->try_process_event(event))
        switch (event.type) {
            // Structure notify
            case ConfigureNotify: {   // size or position change

                BOOST_LOG_TRIVIAL(info) << "configure win " << rendering_context_->getDrawable() << ": pos [" << event.xconfigurerequest.x << " " <<
                                        event.xconfigurerequest.y << "], size [" << event.xconfigurerequest.width << " "
                                        << event.xconfigurerequest.height << "]";
                // Warning this is not thread safe
                rendering_context_->setWindowDimensions({event.xconfigurerequest.width, event.xconfigurerequest.height});

                if (configure_handler_)
                    configure_handler_();

                if (renderer_)
                    renderer_->reshape();
            }
                break;
            case CirculateNotify:
                BOOST_LOG_TRIVIAL(info) << "CirculateNotify";
                break;
            case DestroyNotify: {
                BOOST_LOG_TRIVIAL(warning) << "DestroyNotify event signals that a running window was killed_, in order for window to stop gracefully use stop_window_loop!";
                stop_window_loop();
            }
                break;
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

                if (rendering_context_)
                    rendering_context_->glContextFromDefault();

                if (renderer_ && renderer_->running())
                    renderer_->stop();

                if (running_)
                    stop_window_loop();
            }
                break;
            case ClientMessage: {
                BOOST_LOG_TRIVIAL(info) << "Client message";
            }
                break;
            case Expose: {
                BOOST_LOG_TRIVIAL(info) << "Exposure event";
                if (configure_handler_)
                    configure_handler_();
            }
            break;
                // Unhandled message
            default: {
                BOOST_LOG_TRIVIAL(info) << "Type " << event.type;
            }
                break;
        }
    }

    if (worker.joinable()) {
        worker.join();
    }

#ifdef __GNUC__
    BOOST_LOG_TRIVIAL(debug) << "Leaving " << __func__;
#endif
}

void XWindow::kill() {
    std::lock_guard lg {kill_mutex_};
    if (!killed_) {
        glXDestroyContext(rendering_context_->getDisplay(), rendering_context_->getGlxContext());
        XDestroyWindow(rendering_context_->getDisplay(), rendering_context_->getDrawable());
        XCloseDisplay(rendering_context_->getDisplay());
        renderer_->stop();
        if (running_)
            stop_window_loop();

        killed_ = true;
    }
}

void XWindow::map_window() {
    XMapWindow(rendering_context_->getDisplay(), rendering_context_->getDrawable());
}

const std::shared_ptr<AbstractInputManager>& XWindow::get_input_manager() const {
    std::lock_guard lg {input_manager_access_mutex_};
    return input_manager_;
}

const window_config &XWindow::get_config() const {
    return config_;
}

const std::shared_ptr<rendering::RenderingContext> &XWindow::get_context() const {
    return rendering_context_;
}

void XWindow::start_window_loop() {
    {
        std::lock_guard lg{running_mutex_};
        running_ = true;
    }

    window_loop();
#ifdef __GNUC__
    BOOST_LOG_TRIVIAL(debug) << "Leaving " << __func__ ;
#endif
}

void XWindow::stop_window_loop() {
    std::lock_guard lg {running_mutex_};
    running_ = false;
}

[[nodiscard]] const rendering::GLRendererPtr& XWindow::get_renderer() const {
    std::lock_guard lg {renderer_access_mutex_};
    return renderer_;
}

void XWindow::set_renderer(rendering::GLRendererPtr renderer_ptr) {
    std::lock_guard lg {renderer_access_mutex_};
    if (renderer_ptr == nullptr)
        throw std::runtime_error("Null pointer assigned into renderer ptr");

    renderer_ =  std::move(renderer_ptr);
}

void XWindow::set_manager(const std::shared_ptr<AbstractInputManager>& manager) {
    std::lock_guard lg {input_manager_access_mutex_};
    input_manager_ = manager;
}
}
