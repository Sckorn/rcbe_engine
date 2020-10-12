#include <type_traits>

#include <X11/keysym.h>

#include <core/EditorInputManager.hpp>
#include <common/utils/stack_utils.hpp>

#include <core/gl_helpers.hpp>
#include <data_types/math/Matrix.hpp>
#include <data_types/math/Vector.hpp>

namespace rcbe::core {
EditorInputManager::EditorInputManager(const rcbe::rendering::RenderingContextPtr& rendering_context)
:
rendering_context_ { rendering_context }
{
    if (rendering_context_ == nullptr) {
        throw std::runtime_error("Rendering context as member is nullptr");
    };
}

void EditorInputManager::init() {
    if (methods_initialized_)
        return;

    register_handler(InputEventType::key_release, [this](InputManagerImplementation::event_stack_type& arg) {
        if (arg.empty()) {
            BOOST_LOG_TRIVIAL(error) << "Event stack is empty!";
            return;
        } else {
            if (arg.size() > 1) {
                auto event = utils::pop(arg);
                auto prev_event = utils::pop(arg);

                if (static_cast<int>(InputEventType::key_press) != prev_event.type) {
                    BOOST_LOG_TRIVIAL(warning) << "Event order is broken!";
                    std::decay_t<decltype(arg)> empty_stack{};
                    arg.swap(empty_stack);
                }
                BOOST_LOG_TRIVIAL(debug) << "Released button in " << event.xkey.time - prev_event.xkey.time;
            } else {
                auto event = utils::pop(arg);
                BOOST_LOG_TRIVIAL(debug) << "Released button in " << event.xkey.time;
            }
        }
    });
    register_handler(InputEventType::key_press, [this](InputManagerImplementation::event_stack_type& arg) {
        if (arg.empty()) {
            BOOST_LOG_TRIVIAL(error) << "Event stack is empty";
            return;
        }

        if (this->rendering_context_ == nullptr) {
            BOOST_LOG_TRIVIAL(error) << "Rendering context is nullptr";
            return;
        }
        auto event = utils::pop(arg);
        BOOST_LOG_TRIVIAL(debug) << event.xkey.keycode;

        auto key_sym = XLookupKeysym(&(event.xkey), 0);

        switch(key_sym) {
            case XK_w:
            case XK_W:
            {
                rendering_context_->tvec = rendering_context_->tvec + rcbe::math::Vector3d {0.0, 0.0, 1.0};
            }
                break;
            case XK_s:
            case XK_S:
            {
                rendering_context_->tvec = rendering_context_->tvec + rcbe::math::Vector3d {0.0, 0.0, -1.0};
            }
                break;
            case XK_a:
            case XK_A:
            {
                rendering_context_->tvec = rendering_context_->tvec + rcbe::math::Vector3d {1.0, 0.0, 0.0};
            }
                break;
            case XK_d:
            case XK_D:
            {
                rendering_context_->tvec = rendering_context_->tvec + rcbe::math::Vector3d {-1.0, 0.0, 0.0};
            }
                break;
            case XK_Up:
            {
                rendering_context_->rvec = rendering_context_->rvec + rcbe::math::Vector3d {-1.0, 0.0, 0.0};
            }
                break;
            case XK_Down:
            {
                rendering_context_->rvec = rendering_context_->rvec + rcbe::math::Vector3d {1.0, 0.0, 0.0};
            }
                break;
            case XK_Left:
            {
                rendering_context_->rvec = rendering_context_->rvec + rcbe::math::Vector3d {0.0, -1.0, 0.0};
            }
                break;
            case XK_Right:
            {
                rendering_context_->rvec = rendering_context_->rvec + rcbe::math::Vector3d {0.0, 1.0, 0.0};
            }
                break;
            case XK_Page_Up: {
                rendering_context_->tvec = rendering_context_->tvec + rcbe::math::Vector3d {0.0, -1.0, 0.0};
            }
            break;
            case XK_Page_Down: {
                rendering_context_->tvec = rendering_context_->tvec + rcbe::math::Vector3d {0.0, 1.0, 0.0};
            }
                break;
            default:
                BOOST_LOG_TRIVIAL(info) << "Unhandled KeySym " << key_sym;
                break;
        }

        arg.push(event);
    });
    register_handler(InputEventType::button_release, [this](InputManagerImplementation::event_stack_type& arg) {
        const auto e = utils::pop(arg);
        auto et = static_cast<MouseEventType>(e.xbutton.button);
        switch (et) {
            case MouseEventType ::wheel_up:
            {
                rendering_context_->zoom += 0.1;
            }
                break;
            case MouseEventType ::wheel_down:
            {
                rendering_context_->zoom -= 0.1;
            }
                break;
            case MouseEventType ::left_button:
            case MouseEventType ::right_button:
            case MouseEventType ::middle_button:
            {
                // just ignoring it for now
            }
                break;

            case MouseEventType ::unknown:
            default:
            {
                BOOST_LOG_TRIVIAL(debug) << "Unknown type of mouse button";
            }
                break;
        }
    });

    methods_initialized_ = true;
}

bool EditorInputManager::try_process_event(InputManagerImplementation::input_event_reference event) {
    if (!methods_initialized_) {
        init();
    }
    return InputManagerImplementation::try_process_event(event);
}

}