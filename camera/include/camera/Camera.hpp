#ifndef RCBE_ENGINE_CAMERA_HPP
#define RCBE_ENGINE_CAMERA_HPP

#include <mutex>
#include <memory>

#include <rcbe-engine/datamodel/math/euler_angles.hpp>
#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>
#include <rcbe-engine/datamodel/math/Transform.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/math/math_constants.hpp>
#include <rcbe-engine/datamodel/math/rotation_units.hpp>
#include <rcbe-engine/datamodel/rendering/camera_config.hpp>

namespace rcbe::rendering {
class Camera {
public:
    using TransformType = math::Transform;

    Camera() = delete;

    Camera(
            const rendering::RenderingContextPtr &context,
            const rendering::camera_config &config
    );
    ~Camera() = default;

    math::Matrix4x4 lookAt(const math::Vector3d& camera_position, const math::Vector3d& lookat, const math::Vector3d& up);

    [[nodiscard]] const TransformType& getTransform() const;

    void resetView();

    [[nodiscard]] math::Vector3d cameraDirection() const;
    [[nodiscard]] math::Vector3d cameraUp() const;
    [[nodiscard]] math::Vector3d cameraRight() const;

    void translate(const rcbe::math::Vector3d& direction, const rcbe::math::Vector3d& look_direction);

    [[nodiscard]] const math::pitch &getPitch() const noexcept;
    [[nodiscard]] const math::yaw &getYaw() const noexcept;

    void rotate(math::pitch&& pstep, math::yaw&& ystep);
    void rotate(math::pitch&& step = math::pitch(math::deg(1.)));
    void rotate(math::yaw&& step = math::yaw(math::deg(1.)));
    void setAngles(math::pitch&& p, math::yaw&& y);
    void setPitch(math::pitch&& p);
    void setYaw(math::yaw&& y);

    void zoomIn(rcbe::core::EngineScalar step = 1.);
    void zoomOut(rcbe::core::EngineScalar step = 1.);

private:
#ifdef RDMN_OPENGL
    math::Matrix4x4 lookAtGL(const math::Vector3d& camera_position, const math::Vector3d& lookat, const math::Vector3d& up);
    math::Matrix4x4 directionVectorGL();
#endif

#ifdef RDMN_VULKAN
    math::Matrix4x4 lookAtVulkan(const math::Vector3d& camera_position, const math::Vector3d& lookat, const math::Vector3d& up);
    math::Matrix4x4 directionVectorVulkan();
#endif

    math::pitch clampPitch(math::pitch pitch) const;

    void updateContext(const TransformType &t);

    mutable std::mutex translate_mutex_;
    mutable std::mutex rotate_mutex_;

    math::Matrix4x4 matrixFromYPR(const math::yaw &ystep, const math::pitch &pstep);

    math::Matrix4x4 rotationImpl();
    math::Matrix4x4 rotationImpl(const math::yaw &ystep, const math::pitch &pstep);

    math::Vector3d initial_position_;
    math::Vector3d initial_lookat_;
    math::Vector3d initial_up_;

    math::Vector3d position_;
    math::Vector3d direction_;
    math::Vector3d right_;
    math::Vector3d up_;

    const math::deg max_fov_;
    const math::deg min_fov_;
    const math::deg def_fov_;

    math::deg fov_ = def_fov_;

    rendering::RenderingContextPtr context_;

    TransformType transform_;
#ifdef RDMN_OPENGL
    math::yaw yaw_ { math::deg(-90.)};
    math::pitch pitch_ { math::deg(0.)};
#endif
#ifdef RDMN_VULKAN
    std::tuple<math::yaw, math::pitch, math::roll> initial_euler_;
    math::yaw yaw_;
    math::pitch pitch_;
#endif
};

using CameraPtr = std::shared_ptr<Camera>;
using CameraConstPtr = std::shared_ptr<const Camera>;

CameraPtr make_camera(rendering::RenderingContextPtr ctx, const rendering::camera_config &config);
}

#endif //RCBE_ENGINE_CAMERA_HPP
