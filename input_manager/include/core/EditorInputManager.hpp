#ifndef RCBE_ENGINE_EDITORINPUTMANAGER_HPP
#define RCBE_ENGINE_EDITORINPUTMANAGER_HPP

#include <memory>

#include <core/InputManagerImplementation.hpp>
#include <core/gl_helpers.hpp>

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
    explicit EditorInputManager(handler_collection&& h);
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

                            if (im->get_value(KeyboardEventType::symbol_w)) {
                                c->tvec = c->tvec + rcbe::math::Vector3d {0.0, 0.0, 1.0};
                                return;
                            }

                            if (im->get_value(KeyboardEventType::symbol_a)) {
                                c->tvec = c->tvec + rcbe::math::Vector3d {1.0, 0.0, 0.0};
                                return;
                            }

                            if (im->get_value(KeyboardEventType::symbol_s)) {
                                c->tvec = c->tvec + rcbe::math::Vector3d {0.0, 0.0, -1.0};
                                return;
                            }

                            if (im->get_value(KeyboardEventType::symbol_d)) {
                                c->tvec = c->tvec + rcbe::math::Vector3d {-1.0, 0.0, 0.0};
                                return;
                            }

                            if (im->get_value(KeyboardEventType::arrow_up)) {
                                c->rvec = c->rvec + rcbe::math::Vector3d {-1.0, 0.0, 0.0};
                                return;
                            }

                            if (im->get_value(KeyboardEventType::arrow_down)) {
                                c->rvec = c->rvec + rcbe::math::Vector3d {1.0, 0.0, 0.0};
                                return;
                            }

                            if (im->get_value(KeyboardEventType::arrow_left)) {
                                c->rvec = c->rvec + rcbe::math::Vector3d {0.0, -1.0, 0.0};
                                return;
                            }

                            if (im->get_value(KeyboardEventType::arrow_right)) {
                                c->rvec = c->rvec + rcbe::math::Vector3d {0.0, 1.0, 0.0};
                                return;
                            }

                            auto key_sym = XLookupKeysym(&(event.xkey), 0);
                            BOOST_LOG_TRIVIAL(info) << "Unhandled KeySym " << key_sym;
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
};
}

#endif //RCBE_ENGINE_EDITORINPUTMANAGER_HPP
