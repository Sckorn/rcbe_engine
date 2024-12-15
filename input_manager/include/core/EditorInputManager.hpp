#ifndef RCBE_ENGINE_EDITORINPUTMANAGER_HPP
#define RCBE_ENGINE_EDITORINPUTMANAGER_HPP

#ifdef _WIN32
#include <Windows.h>
#include <windowsx.h>
#endif

#include <memory>

#include <rcbe-engine/core/InputManagerImplementation.hpp>

#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/math/euler_angles.hpp>

#include <rdmn-engine/logger/trivial_logger.hpp>


#ifdef _WIN32
inline constexpr float MOVE_AND_STRAFE_SPEED = 1000;
#endif
#ifdef __linux__
inline constexpr float MOVE_AND_STRAFE_SPEED = 100;
#endif

namespace rcbe::core {
class R_PUBLIC_API EditorInputManager final : protected InputManagerImplementation {
    using InputEventReference = InputManagerImplementation::InputEventReference;
    using PreviousEventReference = InputManagerImplementation::PreviousEventReference;

public:

    using HandlerCollection = std::vector<InputManagerImplementation::HandlerIntermidiateStorage>;

    EditorInputManager() = default;
    explicit EditorInputManager(HandlerCollection &&h);
    ~EditorInputManager() = default;
    EditorInputManager(const EditorInputManager &other) = delete;
    EditorInputManager(EditorInputManager &&other) = default;
    EditorInputManager &operator=(const EditorInputManager &other) = delete;
    EditorInputManager &operator=(EditorInputManager &&other) = default;

    /// TODO: @sckorn pack expansions were introduced, DO IT!!!
    // no pack expansions for lambda capture in c++17 so waiting on c++20
    // until then only one object for capture here
    template <typename ContextPtr, typename Capture>
    static EditorInputManager create(const ContextPtr &ctx, const Capture &c) {
        HandlerCollection h = {
            {InputEventType::key_release,
             [c](InputManagerImplementation &im, InputEventReference event, PreviousEventReference previous) {
                 if (!previous) {
                     RDMN_LOG(RDMN_LOG_ERROR) << "No key press for corresponding key release!";
                     return;
                 }
                 auto prev_event = *previous;

                 if (static_cast<int>(InputEventType::key_press) != prev_event.type) {
                     RDMN_LOG(RDMN_LOG_WARN) << "Event order is broken!";
                 }
#ifdef __linux__
                 RDMN_LOG(RDMN_LOG_DEBUG) << "Released button in " << event.xkey.time - prev_event.xkey.time;
#endif
             }},
            {rcbe::core::InputEventType::key_press,
             [rendering_context = ctx, &c](
                 InputManagerImplementation &im,
                 InputManagerImplementation::InputEventReference event,
                 InputManagerImplementation::PreviousEventReference previous) {
                 const auto speed = rendering_context->computeDeltaTime() * MOVE_AND_STRAFE_SPEED;

                 if (im.getValue(rcbe::core::KeyboardEventType::symbol_w)) {
                     c->translate(c->cameraDirection() * speed, c->cameraDirection());
                     return;
                 }

                 if (im.getValue(rcbe::core::KeyboardEventType::symbol_s)) {
                     c->translate(-c->cameraDirection() * speed, c->cameraDirection());
                     return;
                 }

                 if (im.getValue(rcbe::core::KeyboardEventType::symbol_a)) {
                     c->translate(-c->cameraRight() * speed, c->cameraDirection());
                     return;
                 }

                 if (im.getValue(rcbe::core::KeyboardEventType::symbol_d)) {
                     c->translate(c->cameraRight() * speed, c->cameraDirection());
                     return;
                 }

                 if (im.getValue(rcbe::core::KeyboardEventType::arrow_right)) {
                     rcbe::math::yaw yaw(rcbe::math::deg(1. * speed));
                     c->rotate(std::move(yaw));
                     return;
                 }

                 if (im.getValue(rcbe::core::KeyboardEventType::arrow_left)) {
                     rcbe::math::yaw yaw(rcbe::math::deg(-1. * speed));
                     c->rotate(std::move(yaw));
                     return;
                 }

                 if (im.getValue(rcbe::core::KeyboardEventType::arrow_up)) {
#ifdef RDMN_OPENGL
                     auto pitch = c->getPitch();
                     if (static_cast<double>(pitch.as_deg()) <= 90.) {
                         pitch += rcbe::math::deg(1. * speed);
                         c->setPitch(std::move(pitch));
                     }
#elif defined(RDMN_VULKAN)
                      rcbe::math::pitch step {rcbe::math::deg {1. * speed}};
                      c->rotate(std::move(step));
#endif

                     return;
                 }

                 if (im.getValue(rcbe::core::KeyboardEventType::arrow_down)) {
#ifdef RDMN_OPENGL
                     auto pitch = c->getPitch();
                     if (static_cast<double>(pitch.as_deg()) >= -90.) {
                         pitch += rcbe::math::deg(-1. * speed);
                         c->setPitch(std::move(pitch));
                     }
#elif defined(RDMN_VULKAN)
                      rcbe::math::pitch step {rcbe::math::deg {-1. * speed}};
                      c->rotate(std::move(step));
#endif
                     return;
                 }
             }},
#ifdef __linux__
            {rcbe::core::InputEventType::button_press,
             [rendering_context = ctx, &c](
                 InputManagerImplementation &im,
                 InputManagerImplementation::InputEventReference event,
                 InputManagerImplementation::PreviousEventReference previous) {
                 if (im.getValue(rcbe::core::MouseEventType::middle_button)) {
                     c->resetView();
                     return;
                 }

                 if (im.getValue(rcbe::core::MouseEventType::wheel_down)) {
                     c->zoomOut();
                     return;
                 }

                 if (im.getValue(rcbe::core::MouseEventType::wheel_up)) {
                     c->zoomIn();
                     return;
                 }
             }},
#endif
#ifdef _WIN32
            {rcbe::core::InputEventType::middle_button_press,
             [rendering_context = ctx, &c](
                 InputManagerImplementation &im,
                 InputManagerImplementation::InputEventReference event,
                 InputManagerImplementation::PreviousEventReference previous) {
                 
                c->resetView();
                return;
             }},
             {rcbe::core::InputEventType::left_button_press,
             [rendering_context = ctx, &c](
                 InputManagerImplementation &im,
                 InputManagerImplementation::InputEventReference event,
                 InputManagerImplementation::PreviousEventReference previous) {
                 
                SetCapture(rendering_context->getWindow()); /// TODO: figure out, why button is not set as pressed, might be windows specifics
                return;
             }},
             {rcbe::core::InputEventType::left_button_release,
             [rendering_context = ctx, &c](
                 InputManagerImplementation &im,
                 InputManagerImplementation::InputEventReference event,
                 InputManagerImplementation::PreviousEventReference previous) {
                 
                ReleaseCapture();
                return;
             }},
             {rcbe::core::InputEventType::wheel_down, // on windows there's a singular event for mouse wheel input
             [rendering_context = ctx, &c](
                 InputManagerImplementation &im,
                 InputManagerImplementation::InputEventReference event,
                 InputManagerImplementation::PreviousEventReference previous) {
                auto zdelta = GET_WHEEL_DELTA_WPARAM(event.wparam);

                if (zdelta > 0) {
                    c->zoomIn();
                } else {
                    c->zoomOut();
                }
                return;
             }},
#endif
            {rcbe::core::InputEventType::mouse_motion,
             [rendering_context = ctx, &c](
                 InputManagerImplementation &im,
                 InputManagerImplementation::InputEventReference event,
                 InputManagerImplementation::PreviousEventReference previous) {
#ifdef __linux__
                 const auto current_x = event.xmotion.x;
                 const auto current_y = event.xmotion.y;
#endif
#ifdef _WIN32
                 const auto current_x = GET_X_LPARAM(event.lparam);
                 const auto current_y = GET_Y_LPARAM(event.lparam);
#endif

                 if (!rendering_context->getMouseCoordinates().has_value()) {
                     rendering_context->setMouseCoordinates(rcbe::math::Vector2d(current_x, current_y));
                 }
#ifdef __linux__
                 if (im.getValue(rcbe::core::MouseEventType::left_button)) {
#endif
#ifdef _WIN32
                 if (event.wparam & MK_LBUTTON) {
#endif
                     auto mc = rendering_context->getMouseCoordinates();
                     auto x_offset = current_x - mc->x();
                     auto y_offset = mc->y() - current_y;

                     rendering_context->setMouseCoordinates(rcbe::math::Vector2d(current_x, current_y));

                     const auto sensitivity = 0.1;

                     x_offset *= sensitivity;
                     y_offset *= sensitivity;

                     auto yaw = c->getYaw();
                     auto pitch = c->getPitch();

#ifdef RDMN_VULKAN
                     math::yaw ystep {rcbe::math::deg(x_offset)};
                     math::pitch pstep {rcbe::math::deg(y_offset)};

                     c->rotate(std::move(pstep), std::move(ystep));
#elif defined(RDMN_OPENGL)
                      yaw += rcbe::math::deg(x_offset);
                      pitch += rcbe::math::deg(y_offset);

                      if (static_cast<double>(pitch.as_deg()) > 90.) {
                          pitch = rcbe::math::pitch(rcbe::math::deg(90));
                      }

                      if (static_cast<double>(pitch.as_deg()) < -90.) {
                          pitch = rcbe::math::pitch(rcbe::math::deg(-90));
                      }

                      c->setAngles(std::move(pitch), std::move(yaw));
#endif
                     return;
                 }

                 rendering_context->setMouseCoordinates(rcbe::math::Vector2d(current_x, current_y));
             }}};

        return EditorInputManager(std::move(h));
    }

    using InputManagerImplementation::getValue;
    using InputManagerImplementation::registerHandler;
    using InputManagerImplementation::tryProcessEvent;
};
}// namespace rcbe::core

#endif//RCBE_ENGINE_EDITORINPUTMANAGER_HPP
