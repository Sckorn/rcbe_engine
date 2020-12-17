#include <rcbe-engine/camera/Camera.hpp>

#include <rcbe-engine/datamodel/math/math_constants.hpp>

namespace rcbe::rendering {
Camera::Camera(const rendering::RenderingContextPtr &context, const rendering::camera_config &config)
:
initial_position_ { config.camera_position }
, initial_lookat_ { config.camera_lookat }
, initial_up_ { config.camera_up }
, context_ { context }
, transform_ {lookAt(initial_position_, initial_lookat_, initial_up_) }
{
    if (!context_)
        throw std::runtime_error("Rendering context for Camera is nullptr!");

    BOOST_LOG_TRIVIAL(debug) << transform_.matrix();

    updateContext(transform_);
}

void Camera::updateContext(const TransformType &t) {
    transform_ = t;
    context_->updateTransform(t);
}

math::Matrix4x4 Camera::lookAt(const math::Vector3d& camera_position, const math::Vector3d& lookat, const math::Vector3d& up) {
    position_ = camera_position;
    direction_ = -((lookat - position_).normalized());  // direction is negated due to OpenGL's positive Z pointing towards camera,
                                                        // and as such a vector pointing forward out of the camera is negative Z
    right_ = math::Vector3d ::cross(up, direction_).normalized();
    up_ = math::Vector3d ::cross(direction_, right_).normalized();

    auto left_part = math::Matrix4x4 {
            right_.x(), right_.y(), right_.z(), 0,
            up_.x(), up_.y(), up_.z(), 0,
            direction_.x(), direction_.y(), direction_.z(), 0,
            0, 0, 0, 1
    };

    auto right_part = math::Matrix4x4 {
            1, 0, 0, -position_.x(),
            0, 1, 0, -position_.y(),
            0, 0, 1, -position_.z(),
            0, 0, 0, 1
    };

    return left_part * right_part;
}

const Camera::TransformType& Camera::getTransform() const {
    std::lock_guard lg{translate_mutex_};
    return transform_;
}

void Camera::resetView() {
    std::lock_guard lg{rotate_mutex_};
    std::lock_guard lg2{translate_mutex_};
    zoom_ = 45.;
    context_->updateZoom(zoom_);
    updateContext(math::Transform(lookAt(initial_position_, initial_lookat_, initial_up_)));
    yaw_ = math::yaw(math::deg(-90.));
    pitch_ = math::pitch(math::deg(0.));

}

math::Vector3d Camera::cameraDirection() const {
    return -(direction_.normalized());
}

math::Vector3d Camera::cameraUp() const {
    return up_.normalized();
}

math::Vector3d Camera::cameraRight() const {
    return math::Vector3d::cross(cameraDirection(), cameraUp()).normalized();
}

void Camera::translate(const rcbe::math::Vector3d& direction, const rcbe::math::Vector3d& look_direction) {
    std::lock_guard lg{translate_mutex_};
    const auto curr_pos = position_;
    const auto new_pos = direction + curr_pos;

    updateContext(math::Transform(lookAt(new_pos, new_pos + look_direction, initial_up_)));
}

math::Matrix4x4 Camera::rotationImpl() {
    const auto x = std::cos(static_cast<const double>(yaw_.as_rad())) * std::cos(static_cast<const double>(pitch_.as_rad()));
    const auto y = std::sin(static_cast<const double>(pitch_.as_rad()));
    const auto z = std::sin(static_cast<const double>(yaw_.as_rad())) * std::cos(static_cast<const double>(pitch_.as_rad()));
    math::Vector3d new_direction(x, y, z);

    return lookAt(position_, position_ + new_direction.normalized(), initial_up_);
}

const math::pitch &Camera::getPitch() const noexcept {
    std::lock_guard lg{rotate_mutex_};

    return pitch_;
}

const math::yaw &Camera::getYaw() const noexcept {
    std::lock_guard lg{rotate_mutex_};

    return yaw_;
}

void Camera::rotate(math::pitch&& pstep, math::yaw&& ystep) {
    std::lock_guard lg{rotate_mutex_};

    pitch_ += pstep;
    yaw_ += ystep;
    updateContext(math::Transform(rotationImpl()));
}

void Camera::rotate(math::pitch&& step) {
    std::lock_guard lg{rotate_mutex_};

    pitch_ += step;
    updateContext(math::Transform(rotationImpl()));
}

void Camera::rotate(math::yaw&& step) {
    std::lock_guard lg{rotate_mutex_};

    yaw_ += step;
    updateContext(math::Transform(rotationImpl()));
}

void Camera::setAngles(math::pitch&& p, math::yaw&& y) {
    std::lock_guard lg{rotate_mutex_};

    pitch_ = p;
    yaw_ = y;
    updateContext(math::Transform(rotationImpl()));
}

void Camera::setPitch(math::pitch&& p) {
    std::lock_guard lg{rotate_mutex_};
    pitch_ = p;
    updateContext(math::Transform(rotationImpl()));
}

void Camera::setYaw(math::yaw&& y) {
    std::lock_guard lg{rotate_mutex_};
    yaw_ = y;
    updateContext(math::Transform(rotationImpl()));
}

void Camera::zoomIn(rcbe::core::EngineScalar step) {
    auto zoom_copy = zoom_;
    zoom_copy -= step;
    if (zoom_copy >= 1.) {
        zoom_ = zoom_copy;
    }
    context_->updateZoom(zoom_);
}

void Camera::zoomOut(rcbe::core::EngineScalar step) {
    auto zoom_copy = zoom_;
    zoom_copy += step;
    if (zoom_copy <= 45.) {
        zoom_ = zoom_copy;
    }
    context_->updateZoom(zoom_);
}

CameraPtr make_camera(rendering::RenderingContextPtr ctx, const rendering::camera_config &config) {
    return std::make_shared<Camera>(ctx, config);
}
}