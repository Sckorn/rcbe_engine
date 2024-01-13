#include <rcbe-engine/datamodel/math/Matrix.hpp>

#include <rcbe-engine/camera/Camera.hpp>

#include <rcbe-engine/fundamentals/convinience.hpp>
#include <rcbe-engine/datamodel/math/math_constants.hpp>
#include <rcbe-engine/datamodel/math/matrix_helpers.hpp>
#include <rcbe-engine/exceptions/not_implemented.hpp>

/// Vulkan Euler Are ZXY


namespace rcbe::rendering {
Camera::Camera(const rendering::RenderingContextPtr &context, const rendering::camera_config &config)
:
initial_position_ { config.camera_position }
, initial_lookat_ { config.camera_lookat }
, initial_up_ { config.camera_up }
, max_fov_ { config.max_fov }
, min_fov_ { config.min_fov }
, def_fov_ { config.def_fov }
, context_ { context }
, transform_ {lookAt(initial_position_, initial_lookat_, initial_up_) }
#ifdef RDMN_VULKAN
, initial_euler_ { math::yprFromMatrix(transform_.matrix()) }
, yaw_ { std::get<0>(initial_euler_) }
, pitch_ { std::get<1>(initial_euler_) }
#endif
{
    if (!context_)
        throw std::runtime_error("Rendering context for Camera is nullptr!");

    context_->updateFov(fov_);

    BOOST_LOG_TRIVIAL(debug) << "Initial Camera view rotation matrix \n" << transform_.matrix();

    updateContext(transform_);

    BOOST_LOG_TRIVIAL(debug) << "Initial Yaw " << yaw_.as_deg() << " Pitch " << pitch_.as_deg();
}

void Camera::updateContext(const TransformType &t) {
    transform_ = t;
    context_->updateTransform(t);
}

math::Matrix4x4 Camera::lookAt(const math::Vector3d& camera_position, const math::Vector3d& lookat, const math::Vector3d& up) {
#ifdef RDMN_VULKAN
    return lookAtVulkan(camera_position, lookat, up);
#elif defined(RDMN_OPENGL)
    return lookAtGL(camera_position, lookat, up);
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
#else
    static_assert(false, "Rasterizer backend not selected, build configuration is malformed!");
#endif
}

#ifdef RDMN_OPENGL
math::Matrix4x4 Camera::lookAtGL(
        const math::Vector3d& camera_position,
        const math::Vector3d& lookat,
        const math::Vector3d& up) {
    position_ = camera_position;
    // direction is negated due to OpenGL's positive Z pointing towards camera,
    // and as such a vector pointing forward out of the camera is negative Z
    direction_ = -((lookat - position_).normalized());
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
#endif

#ifdef RDMN_VULKAN
math::Matrix4x4 Camera::lookAtVulkan(
        const math::Vector3d& camera_position,
        const math::Vector3d& lookat,
        const math::Vector3d& up) {
    position_ = camera_position;
    direction_ = (lookat - position_).normalized();
    right_ = math::Vector3d::cross(direction_, up).normalized(); // u
    up_ = math::Vector3d::cross(right_, direction_).normalized(); // v

    return math::Matrix4x4 {
            right_.x(), right_.y(), right_.z(), -(right_ * position_),
            up_.x(), up_.y(), up_.z(), -(up_ * position_),
            -direction_.x(), -direction_.y(), -direction_.z(), (direction_ * position_),
            0, 0, 0, 1
    };
}
#endif

const Camera::TransformType& Camera::getTransform() const {
    std::lock_guard lg{translate_mutex_};
    return transform_;
}

void Camera::resetView() {
    std::scoped_lock l(rotate_mutex_, translate_mutex_);
    fov_ = def_fov_;
    context_->updateFov(fov_);
    updateContext(math::Transform(lookAt(initial_position_, initial_lookat_, initial_up_)));
#ifdef RDMN_VULKAN
    yaw_ = std::get<0>(initial_euler_);
    pitch_ = std::get<1>(initial_euler_);
#endif
#ifdef RDMN_OPENGL
    yaw_ = math::yaw { math::deg(-90.)};
    pitch_ = math::pitch { math::deg(0.)};
#endif
}

math::Vector3d Camera::cameraDirection() const {
#if RDMN_VULKAN
    return (direction_.normalized());
#elif defined(RDMN_OPENGL)
    return -(direction_.normalized());
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
#else
    static_assert(false, "Rasterizer backend not selected, build configuration malformed!");
#endif
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

#ifdef RDMN_VULKAN
    updateContext(math::Transform(lookAt(new_pos, new_pos + look_direction, up_)));
#elif RDMN_OPENGL
    updateContext(math::Transform(lookAt(new_pos, new_pos + look_direction, initial_up_)));
#elif RDMN_DIRECTX
    static_assert(false, "DirectX rasterizer is not supported!");
#endif
}
#ifdef RDMN_OPENGL
math::Matrix4x4 Camera::directionVectorGL() {
    const double yaw_rad = static_cast<const double>(yaw_.as_rad());
    const double pitch_rad = static_cast<const double>(pitch_.as_rad());

    const auto x = std::cos(yaw_rad) * std::cos(pitch_rad);
    const auto y = std::sin(pitch_rad);
    const auto z = std::sin(yaw_rad) * std::cos(pitch_rad);
    math::Vector3d new_direction(x, y, z);

    return lookAt(position_, position_ + new_direction.normalized(), initial_up_);
}
#endif

#ifdef RDMN_VULKAN
math::Matrix4x4 Camera::directionVectorVulkan() {
    const double yaw_rad = static_cast<const double>(yaw_.as_rad());
    const double pitch_rad = -static_cast<const double>(pitch_.as_rad());
    /// Roll isn't changed from outside, since it makes no sense for us
    const double roll_rad = static_cast<const double>(std::get<2>(initial_euler_).as_rad());

    const auto x = std::cos(roll_rad) * std::sin(yaw_rad) + std::cos(yaw_rad) * std::sin(roll_rad) * std::sin(pitch_rad);
    const auto y = std::sin(roll_rad) * std::sin(yaw_rad) - std::cos(roll_rad) * std::cos(yaw_rad) * std::sin(pitch_rad);
    const auto z = std::cos(pitch_rad) * std::cos(yaw_rad);
    math::Vector3d new_direction(x, y, z);

    return lookAt(position_, position_ - new_direction, up_);
}
#endif

math::Matrix4x4 Camera::matrixFromYPR(const math::yaw &ystep, const math::pitch &pstep) {
    const auto rm = math::matrixFromZXY(math::roll{}, pstep, -ystep);
    const auto res = rm * transform_.matrix();

    direction_ = -res.getRow(2).lowerDimension().normalized();
    up_ = res.getRow(1).lowerDimension().normalized();
    right_ = res.getRow(0).lowerDimension().normalized();

    return res;
}

math::Matrix4x4 Camera::rotationImpl() {
#if defined(RDMN_OPENGL)
    return directionVectorGL();
#elif defined(RDMN_VULKAN)
    return directionVectorVulkan();
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
    throw exception::not_implemented(std::string(R_READABLE_FUNC_NAME) + " not implemented for Windows");
#else
    static_assert(false, "Rasterizer backend not selected, build configuration malformed!");
#endif
}

math::Matrix4x4 Camera::rotationImpl(const math::yaw &ystep, const math::pitch &pstep) {
#if defined(RDMN_OPENGL)
    /// TODO: implement for OpenGL @sckorn
    throw rcbe::exception::NotImplementedException("Step rotations are not implemented for OpenGL rasterizer");
#elif defined(RDMN_VULKAN)
    return matrixFromYPR(ystep, pstep);
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
    throw exception::NotImplementedException(std::string(R_READABLE_FUNC_NAME) + " not implemented for Windows");
#else
    static_assert(false, "Rasterizer backend not selected, build configuration malformed!");
#endif
}

const math::pitch &Camera::getPitch() const noexcept {
    std::lock_guard lg{rotate_mutex_};

    return pitch_;
}

const math::yaw &Camera::getYaw() const noexcept {
    std::lock_guard lg{rotate_mutex_};

    return yaw_;
}

math::pitch Camera::clampPitch(math::pitch pitch) const {
    if (auto degpihalfpos = math::pitch { math::deg { 90. } }; pitch > degpihalfpos) {
        return degpihalfpos;
    } else if(auto degpihalfneg = math::pitch { math::deg { -90. } }; pitch < degpihalfneg ) {
        return degpihalfneg;
    }

    return pitch;
}

void Camera::rotate(math::pitch&& pstep, math::yaw&& ystep) {
    std::lock_guard lg{rotate_mutex_};

#if RDMN_VULKAN
    updateContext(math::Transform(rotationImpl(ystep, pstep)));
    pitch_ += pstep;
    pitch_ = clampPitch(pitch_);
    yaw_ += ystep;
#elif defined(RDMN_OPENGL)
    pitch_ += pstep;
    pitch_ = clampPitch(pitch_);
    yaw_ += ystep;
    updateContext(math::Transform(rotationImpl()));
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
    /// TODO: impl for DirectX @sckorn
    throw exception::not_implmented(std::string(R_READABLE_FUNC_NAME) + " not implemented for Windows");
#else
    static_assert(false, "Rasterizer backend not selected, build configuration malformed!");
#endif
}

void Camera::rotate(math::pitch&& step) {
    std::lock_guard lg{rotate_mutex_};

#if RDMN_VULKAN
    updateContext(math::Transform(rotationImpl(math::yaw { math::deg { 0.0 } }, step )));
    pitch_ += step;
    pitch_ = clampPitch(pitch_);
#elif RDMN_OPENGL
    pitch_ += step;
    pitch_ = clampPitch(pitch_);
    updateContext(math::Transform(rotationImpl( )));
#elif defined(RDMN_OPENGL) && defined(_WIN32)
    throw exception::not_implmented(std::string(R_READABLE_FUNC_NAME) + " not implemented for Windows");
#else
    static_assert(false, "Rasterizer backend not selected, build configuration malformed!");
#endif
}

void Camera::rotate(math::yaw&& step) {
    std::lock_guard lg{rotate_mutex_};

#if RDMN_VULKAN
    updateContext(math::Transform(rotationImpl(step, math::pitch { math::deg { 0.0 } } )));
    yaw_ += step;
#elif RDMN_OPENGL
    yaw_ += step;
    updateContext(math::Transform(rotationImpl()));
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
    throw exception::not_implmented(std::string(R_READABLE_FUNC_NAME) + " not implemented for Windows");
#else
    static_assert(false, "Rasterizer backend not selected, build configuration malformed!");
#endif
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
    pitch_ = clampPitch(std::move(pitch_));
    updateContext(math::Transform(rotationImpl()));
}

void Camera::setYaw(math::yaw&& y) {
    std::lock_guard lg{rotate_mutex_};
    yaw_ = y;
    updateContext(math::Transform(rotationImpl()));
}

void Camera::zoomIn(rcbe::core::EngineScalar step) {
    auto zoom_copy = fov_;
    zoom_copy -= math::deg(step);
    if (zoom_copy >= min_fov_) {
        fov_ = zoom_copy;
    }
    context_->updateFov(fov_);
}

void Camera::zoomOut(rcbe::core::EngineScalar step) {
    auto zoom_copy = fov_;
    zoom_copy += math::deg(step);
    if (zoom_copy <= max_fov_) {
        fov_ = zoom_copy;
    }
    context_->updateFov(fov_);
}

CameraPtr make_camera(rendering::RenderingContextPtr ctx, const rendering::camera_config &config) {
    return std::make_shared<Camera>(ctx, config);
}
}