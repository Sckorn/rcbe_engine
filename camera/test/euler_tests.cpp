#include <memory>

#include <gtest/gtest.h>

#include <rcbe-engine/camera/Camera.hpp>
#include <rcbe-engine/datamodel/math/matrix_helpers.hpp>
#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>
#include <rcbe-engine/fuzzy_logic/fuzzy_logic.hpp>
#include <rcbe-engine/utils/json_utils.hpp>

/// TODO: implement for OpenGL as well @sckorn @radameon
#ifdef RDMN_VULKAN
/// TODO: reducing computational error to less then 1 deg is fine,
/// but we need to find a way to make it better @sckorn @radameon
TEST(VulkanCamera, GetEulerFromMatrix) {
    auto camera_conf = rcbe::utils::read_from_file<rcbe::rendering::camera_config>(
        "datamodel/data/rendering/vulkan_camera_config.json");
    const auto rend_ctx = std::make_shared<rcbe::rendering::RenderingContext>();
    rcbe::rendering::Camera cam {rend_ctx, camera_conf};

    {
        auto ydeg = cam.getYaw().as_deg();
        auto pdeg = cam.getPitch().as_deg();

        ASSERT_TRUE(rcbe::core::fuzzy_equal(static_cast<double>(ydeg), 0.));
        ASSERT_TRUE(rcbe::core::fuzzy_equal(static_cast<double>(pdeg), 54.7356));
    }

    {
        cam.rotate(rcbe::math::yaw {rcbe::math::deg {5.}});

        const auto [y, p, r] = rcbe::math::yprFromMatrix(cam.getTransform().matrix());// roll is not used
        const auto ydeg = y.as_deg();
        const auto pdeg = p.as_deg();

        /// The negative result is due to the value being negated during rotation
        /// the rotation is negated due to scene having to be rotated into opposite
        /// direction of the rotation, to make it look realistic.
        ASSERT_TRUE(rcbe::core::fuzzy_equal(static_cast<double>(ydeg), -5.));
        ASSERT_TRUE(rcbe::core::fuzzy_equal(static_cast<double>(pdeg), 54.7356));
    }

    {
        cam.rotate(rcbe::math::pitch {rcbe::math::deg {5.}});

        /// Different call [getRotation], due to need to test for 3x3 matrix as well,
        /// Above calls invoke 4x4 overload
        const auto [y, p, r] = rcbe::math::yprFromMatrix(cam.getTransform().getRotation());// roll is not used
        const auto ydeg = y.as_deg();
        const auto pdeg = p.as_deg();

        /// TODO: for some reason there is a 0.7 deg computational error here, try to fix it @sckorn B:862khd1fr
        ASSERT_TRUE(rcbe::core::fuzzy_equal(static_cast<double>(ydeg), -5.72633, 0.1));
        ASSERT_TRUE(rcbe::core::fuzzy_equal(static_cast<double>(pdeg), 59.7139));
    }
}
#endif
