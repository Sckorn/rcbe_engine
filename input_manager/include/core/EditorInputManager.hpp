#ifndef RCBE_ENGINE_EDITORINPUTMANAGER_HPP
#define RCBE_ENGINE_EDITORINPUTMANAGER_HPP

#include <memory>

#include <core/InputManagerImplementation.hpp>

#include <rcbe-engine/datamodel/math/euler_angles.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace rcbe::core {
class EditorInputManager final : protected InputManagerImplementation {
    using input_event_reference = InputManagerImplementation::input_event_reference;
    using previous_event_reference = InputManagerImplementation::previous_event_reference;
public:
    using handler_collection = std::vector<InputManagerImplementation::handler_intermidiate_storage>;

    EditorInputManager() = default;
    explicit EditorInputManager(handler_collection&& h);
    ~EditorInputManager() = default;
    EditorInputManager(const EditorInputManager& other) = delete;
    EditorInputManager(EditorInputManager&& other) = default;
    EditorInputManager& operator=(const EditorInputManager& other) = delete;
    EditorInputManager& operator=(EditorInputManager&& other) = default;

    // no pack expansions for lambda capture in c++17 so waiting on c++20
    // until then only one object for capture here
    template <typename ContextPtr, typename Capture>
    static EditorInputManager create(const ContextPtr& ctx, const Capture& c) {
        handler_collection h = {
            {
                InputEventType::key_release, [c](InputManagerImplementation& im, input_event_reference event, previous_event_reference previous) {
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
                rcbe::core::InputEventType::key_press, [rendering_context = ctx, &c](InputManagerImplementation& im, InputManagerImplementation::input_event_reference event, InputManagerImplementation::previous_event_reference previous) {

                    const auto speed = rendering_context->deltaTime() * 10;

                    if (im.get_value(rcbe::core::KeyboardEventType::symbol_w)) {
                        c->translate(c->cameraDirection() * speed, c->cameraDirection());
                        return;
                    }

                    if (im.get_value(rcbe::core::KeyboardEventType::symbol_s)) {
                        c->translate(-c->cameraDirection() * speed, c->cameraDirection());
                        return;
                    }

                    if (im.get_value(rcbe::core::KeyboardEventType::symbol_a)) {
                        c->translate(-c->cameraRight() * speed, c->cameraDirection());
                        return;
                    }

                    if (im.get_value(rcbe::core::KeyboardEventType::symbol_d)) {
                        c->translate(c->cameraRight() * speed, c->cameraDirection());
                        return;
                    }

                    if (im.get_value(rcbe::core::KeyboardEventType::arrow_right)) {
                        rcbe::math::yaw yaw(rcbe::math::deg(1. * speed));
                        c->rotate(std::move(yaw));
                        return;
                    }

                    if (im.get_value(rcbe::core::KeyboardEventType::arrow_left)) {
                        rcbe::math::yaw yaw(rcbe::math::deg(-1. * speed));
                        c->rotate(std::move(yaw));
                        return;
                    }

                    if (im.get_value(rcbe::core::KeyboardEventType::arrow_up)) {
                        auto pitch = c->getPitch();
                        if (static_cast<double>(pitch.as_deg()) <= 90.) {
                            pitch += rcbe::math::deg(1. * speed);
                            c->setPitch(std::move(pitch));
                        }

                        return;
                    }

                    if (im.get_value(rcbe::core::KeyboardEventType::arrow_down)) {
                        auto pitch = c->getPitch();
                        if (static_cast<double>(pitch.as_deg()) >= -90.) {
                            pitch += rcbe::math::deg(-1. * speed);
                            c->setPitch(std::move(pitch));
                        }

                        return;
                    }
                }
            },
            {
                rcbe::core::InputEventType::button_press, [rendering_context = ctx, &c] (InputManagerImplementation& im, InputManagerImplementation::input_event_reference event, InputManagerImplementation::previous_event_reference previous) {
                    if (im.get_value(rcbe::core::MouseEventType::middle_button)) {
                        c->resetView();
                        return;
                    }

                    if (im.get_value(rcbe::core::MouseEventType::wheel_down)) {
                        c->zoomOut();
                        return;
                    }

                    if (im.get_value(rcbe::core::MouseEventType::wheel_up)) {
                        c->zoomIn();
                        return;
                    }
                }
            },
            {
                rcbe::core::InputEventType::mouse_motion, [rendering_context = ctx, &c] (InputManagerImplementation& im, InputManagerImplementation::input_event_reference event, InputManagerImplementation::previous_event_reference previous) {
                    const auto current_x = event.xmotion.x;
                    const auto current_y = event.xmotion.y;

                    if (!rendering_context->getMouseCoordinates().has_value()) {
                        rendering_context->setMouseCoordinates(rcbe::math::Vector2d (current_x, current_y));
                    }

                    if (im.get_value(rcbe::core::MouseEventType::left_button)) {
                        auto mc = rendering_context->getMouseCoordinates();
                        auto x_offset = current_x - mc->x();
                        auto y_offset = mc->y() - current_y;

                        rendering_context->setMouseCoordinates(rcbe::math::Vector2d (current_x, current_y));

                        const auto sensitivity = 0.1;

                        x_offset *= sensitivity;
                        y_offset *= sensitivity;

                        auto yaw = c->getYaw();
                        auto pitch = c->getPitch();

                        yaw += rcbe::math::deg(x_offset);
                        pitch += rcbe::math::deg(y_offset);

                        if (static_cast<double>(pitch.as_deg()) > 90.) {
                            pitch = rcbe::math::pitch(rcbe::math::deg(90));
                        }

                        if (static_cast<double>(pitch.as_deg()) < -90.) {
                            pitch = rcbe::math::pitch(rcbe::math::deg(-90));
                        }

                        c->setAngles(std::move(pitch), std::move(yaw));
                        return;
                    }

                    rendering_context->setMouseCoordinates(rcbe::math::Vector2d (current_x, current_y));
                }
            }
        };

        return EditorInputManager(std::move(h));
    }

    using InputManagerImplementation::try_process_event;
    using InputManagerImplementation::register_handler;
    using InputManagerImplementation::get_value;
};
}

#endif //RCBE_ENGINE_EDITORINPUTMANAGER_HPP
