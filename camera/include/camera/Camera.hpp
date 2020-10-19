#ifndef RCBE_ENGINE_CAMERA_HPP
#define RCBE_ENGINE_CAMERA_HPP

#include <mutex>
#include <memory>

#include <data_types/math/euler_angles.hpp>
#include <data_types/rendering/RenderingContext.hpp>
#include <data_types/math/Transform.hpp>
#include <data_types/math/Matrix.hpp>
#include <data_types/math/Vector.hpp>

namespace rcbe::rendering {
class Camera {
public:
    using transform_type = math::Transform;

    Camera() = delete;
    Camera(const rendering::RenderingContextPtr &context, const math::Vector3d &position, const math::Vector3d &lookat, const math::Vector3d &up);
    ~Camera() = default;

    math::Matrix4x4 look_at(const math::Vector3d& camera_position, const math::Vector3d& lookat, const math::Vector3d& up);

    [[nodiscard]]const transform_type& get_transform() const;

    void reset_view();

    [[nodiscard]] math::Vector3d camera_direction() const;
    [[nodiscard]] math::Vector3d camera_up() const;
    [[nodiscard]] math::Vector3d camera_right() const;

    void translate(const rcbe::math::Vector3d& direction, const rcbe::math::Vector3d& look_direction);

    [[nodiscard]] const math::pitch &get_pitch() const;
    [[nodiscard]] const math::yaw &get_yaw() const;

    void rotate(math::pitch&& pstep, math::yaw&& ystep);
    void rotate(math::pitch&& step = math::pitch(math::deg(1.)));
    void rotate(math::yaw&& step = math::yaw(math::deg(1.)));
    void set_angles(math::pitch&& p, math::yaw&& y);
    void set_pitch(math::pitch&& p);
    void set_yaw(math::yaw&& y);

    void zoom_in(const rcbe::core::EngineScalar step = 1.);
    void zoom_out(const rcbe::core::EngineScalar step = 1.);

private:
    void update_context(const transform_type &t);

    mutable std::mutex translate_mutex_;
    mutable std::mutex rotate_mutex_;

    math::Matrix4x4 rotation_impl();

    math::Vector3d initial_position_;
    math::Vector3d initial_lookat_;
    math::Vector3d initial_up_;

    math::Vector3d position_;
    math::Vector3d direction_;
    math::Vector3d right_;
    math::Vector3d up_;

    math::pitch pitch_ { math::deg(0.)};
    math::yaw yaw_ { math::deg(-90.)};
    core::EngineScalar zoom_ = 45.;

    rendering::RenderingContextPtr context_;

    transform_type transform_;
};

using CameraPtr = std::shared_ptr<Camera>;
using CameraConstPtr = std::shared_ptr<const Camera>;

CameraPtr make_camera(rendering::RenderingContextPtr ctx, const rcbe::math::Vector3d& pos, const rcbe::math::Vector3d& lookat);
}

#endif //RCBE_ENGINE_CAMERA_HPP
