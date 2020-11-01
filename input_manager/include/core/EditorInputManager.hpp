#ifndef RCBE_ENGINE_EDITORINPUTMANAGER_HPP
#define RCBE_ENGINE_EDITORINPUTMANAGER_HPP

#include <memory>

#include <X11/keysym.h>

#include <core/InputManagerImplementation.hpp>
#include <core/gl_helpers.hpp>

#include <common/utils/stack_utils.hpp>

#include <data_types/math/Matrix.hpp>
#include <data_types/math/Vector.hpp>

namespace rcbe::core {
class EditorInputManager;

using EditorInputManagerPtr = std::shared_ptr<EditorInputManager>;
using EditorInputManagerConstPtr = std::shared_ptr<const EditorInputManager>;

class EditorInputManager final : public InputManagerImplementation {
public:
    using handler_collection = std::vector<typename InputManagerImplementation::handler_intermidiate_storage>;

    EditorInputManager() = delete;
    ~EditorInputManager() = default;
    EditorInputManager(const EditorInputManager& other) = delete;
    EditorInputManager(EditorInputManager&& other) = default;
    EditorInputManager& operator=(const EditorInputManager& other) = delete;
    EditorInputManager& operator=(EditorInputManager&& other) = default;

    // A variadic lambda capture is required, as of now we use c++17 which only allows a tuple workaround
    // so we need to upgrade to c++20, until then a signle capture is available
    template <typename Capture>
    static EditorInputManagerPtr create(Capture c) {
        handler_collection h = {
                {
                    InputEventType::key_release, [c](InputManagerImplementationPtr im, input_event_reference event, previous_event_reference previous) {
                        if (!previous) {
                            BOOST_LOG_TRIVIAL(error) << "No key press for corresponding key release!";
                            return;
                        }
                        auto prev_event = *previous;

                        if (static_cast<int>(InputEventType::key_press) != prev_event.type) {
                            BOOST_LOG_TRIVIAL(warning) << "Event order is broken!";
                        }
                        BOOST_LOG_TRIVIAL(debug) << "Released button in " << event.xkey.time - prev_event.xkey.time;
                    }
                },
                {
                        InputEventType::key_press, [c](InputManagerImplementationPtr im, input_event_reference event, previous_event_reference previous) {
                            BOOST_LOG_TRIVIAL(debug) << event.xkey.keycode;

                            auto key_sym = XLookupKeysym(&(event.xkey), 0);

                            switch(key_sym) {
                                case XK_w:
                                case XK_W:
                                {
                                    c->tvec = c->tvec + rcbe::math::Vector3d {0.0, 0.0, 1.0};
                                }
                                    break;
                                case XK_s:
                                case XK_S:
                                {
                                    c->tvec = c->tvec + rcbe::math::Vector3d {0.0, 0.0, -1.0};
                                }
                                    break;
                                case XK_a:
                                case XK_A:
                                {
                                    c->tvec = c->tvec + rcbe::math::Vector3d {1.0, 0.0, 0.0};
                                }
                                    break;
                                case XK_d:
                                case XK_D:
                                {
                                    c->tvec = c->tvec + rcbe::math::Vector3d {-1.0, 0.0, 0.0};
                                }
                                    break;
                                case XK_Up:
                                {
                                    c->rvec = c->rvec + rcbe::math::Vector3d {-1.0, 0.0, 0.0};
                                }
                                    break;
                                case XK_Down:
                                {
                                    c->rvec = c->rvec + rcbe::math::Vector3d {1.0, 0.0, 0.0};
                                }
                                    break;
                                case XK_Left:
                                {
                                    c->rvec = c->rvec + rcbe::math::Vector3d {0.0, -1.0, 0.0};
                                }
                                    break;
                                case XK_Right:
                                {
                                    c->rvec = c->rvec + rcbe::math::Vector3d {0.0, 1.0, 0.0};
                                }
                                    break;
                                case XK_Page_Up: {
                                    c->tvec = c->tvec + rcbe::math::Vector3d {0.0, -1.0, 0.0};
                                }
                                    break;
                                case XK_Page_Down: {
                                    c->tvec = c->tvec + rcbe::math::Vector3d {0.0, 1.0, 0.0};
                                }
                                    break;
                                default:
                                    BOOST_LOG_TRIVIAL(info) << "Unhandled KeySym " << key_sym;
                                    break;
                            }
                        }
                },
                {
                        InputEventType::button_release, [c](InputManagerImplementationPtr im, input_event_reference event, previous_event_reference previous) {
                            auto et = static_cast<MouseEventType>(event.xbutton.button);
                            switch (et) {
                                case MouseEventType ::wheel_up:
                                {
                                    c->zoom += 0.1;
                                }
                                    break;
                                case MouseEventType ::wheel_down:
                                {
                                    c->zoom -= 0.1;
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
                        }
                }
        };
        return std::make_shared<EditorInputManager>(std::move(h));
    }

//private:

    explicit EditorInputManager(handler_collection&& h);
};
}

#endif //RCBE_ENGINE_EDITORINPUTMANAGER_HPP
