#include <future>

#include <boost/log/trivial.hpp>

#include <core/EditorInputManager.hpp>
#include <core/GameInputManager.hpp>
#include <core/AbstractInputManager.hpp>

#include <common/utils/json_utils.hpp>

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
XWindow::XWindow(WindowConfig &&config, Display *root_display, int screen_number, const Window &root_window,
                 const Atom &delete_msg)
        :
        config_{std::move(config)}, root_display_(root_display)
        , rendering_context_{(config_.type == WindowConfig::WindowType::GL_RENDERING_WINDOW) ? std::make_shared<rendering::RenderingContext>() : nullptr}
{
    GLXFBConfig *fbConfigs = chooseFbConfig(root_display_, screen_number);
    XVisualInfo *visInfo = glXGetVisualFromFBConfig(root_display_, fbConfigs[0]);

    if (visInfo == nullptr)
        throw std::runtime_error("VisualInfo is null pointer");

    rendering_context_->x_display = XOpenDisplay(NULL);
    rendering_context_->background_color = config_.background_color;
    rendering_context_->window_dimensions = config.size;
    rendering_context_->x_delete_message = delete_msg;

    attributes_.colormap = XCreateColormap(rendering_context_->x_display, root_window, visInfo->visual, AllocNone);
    switch (config_.type) {
        case WindowConfig::WindowType::GL_RENDERING_WINDOW: {
            rendering_context_->gl_x_window = XCreateWindow(rendering_context_->x_display, root_window,
                                                            config_.position.x(), config_.position.y(),
                                                            config_.size.width, config_.size.height, 0,
                                                            visInfo->depth, InputOutput, visInfo->visual, CWColormap,
                                                            &attributes_);
            rendering_context_->gl_x_context = glXCreateContext(rendering_context_->x_display, visInfo, nullptr, true);
            if (rendering_context_->gl_x_context == nullptr)
                throw std::runtime_error("GLX create context is NULL");
        }
            break;
        case WindowConfig::WindowType::DRAWING_WINDOW: {
            rendering_context_->gl_x_window = XCreateSimpleWindow(rendering_context_->x_display, root_window,
                                                                  config_.position.x(), config_.position.y(),
                                                                  config_.size.width, config_.size.height, 0, 0, 0);
        }
            break;
        case WindowConfig::WindowType::UNKNOWN:
        default: {
            throw std::runtime_error("Unknown type of the window!");
        }
            break;
    }
    XSetStandardProperties(rendering_context_->x_display, rendering_context_->gl_x_window, config_.caption.c_str(), "",None,nullptr,0,nullptr);

    if (config_.process_input) {
        if (config.editor) {
            input_manager_ = EditorInputManager::create(rendering_context_);
        } else {
            if (!config_.input_scheme.empty()) {
                input_manager_ = GameInputManager::create(utils::read_raw(config_.input_scheme));
            } else {
                throw std::runtime_error("Input requested but no input scheme provided!");
            }
        }
    }

    auto mask = ExposureMask | StructureNotifyMask;
    if (config_.process_input)
        mask = mask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | PointerMotionMask;

    XSelectInput(root_display_, rendering_context_->gl_x_window, mask);
    XSetWMProtocols(root_display_, rendering_context_->gl_x_window, &(rendering_context_->x_delete_message), 1);

    XFree(visInfo);
    XFree(fbConfigs);
}

XWindow::~XWindow() {
    if (!killed_)
        kill();
}

void XWindow::on_configure(window::configure_handler_t &&handler) {
    configure_handler_ = std::move(handler);
}

void XWindow::on_unmap(window::unmap_handler_t &&handler) {
    unmap_handler_ = std::move(handler);
}

void XWindow::window_loop() {

    std::thread worker{
            [this]() {
                while (running_) {
                    XEvent event;
                    if (XPending(rendering_context_->x_display)) {
                        XNextEvent(rendering_context_->x_display, &event);
                        if (event.type == ClientMessage && static_cast<unsigned int>(event.xclient.data.l[0]) ==
                                                           rendering_context_->x_delete_message) {
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

        if (!input_manager_->try_process_event(event))
        switch (event.type) {
            // Structure notify
            case ConfigureNotify: {   // size or position change

                BOOST_LOG_TRIVIAL(info) << "configure win " << rendering_context_->gl_x_window << ": pos [" << event.xconfigurerequest.x << " " <<
                                        event.xconfigurerequest.y << "], size [" << event.xconfigurerequest.width << " "
                                        << event.xconfigurerequest.height << "]";
                // Warning this is not thread safe
                rendering_context_->window_dimensions.width = event.xconfigurerequest.width;
                rendering_context_->window_dimensions.height = event.xconfigurerequest.height;

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
                    rendering_context_->gl_context_from_default();

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
        glXDestroyContext(rendering_context_->x_display, rendering_context_->gl_x_context);
        XDestroyWindow(rendering_context_->x_display, rendering_context_->gl_x_window);
        XCloseDisplay(rendering_context_->x_display);
        renderer_->stop();
        if (running_)
            stop_window_loop();

        killed_ = true;
    }
}

void XWindow::map_window() {
    XMapWindow(rendering_context_->x_display, rendering_context_->gl_x_window);
}

const AbstractInputManager& XWindow::get_input_manager() const {
    if (input_manager_) {
        return input_manager_;
    } else
        throw std::runtime_error("Trying to get unassigned input manager!");
}

const WindowConfig &XWindow::get_config() const {
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
}
