#include <rcbe-engine/datamodel/rendering/matrix_helpers.hpp>
#include <rcbe-engine/fundamentals/convinience.hpp>

namespace {

rcbe::math::Matrix4x4 makePerspectiveGL(const rcbe::core::EngineScalar near,
                                        const rcbe::core::EngineScalar far,
                                        const rcbe::math::deg fov,
                                        const rcbe::core::IntegralDimensions viewport) {
    const auto aspect = static_cast<float>(viewport.width) / static_cast<float>(viewport.height);
    const auto fovy_rad = fov.as_rad();

    const auto tan = std::tan(static_cast<const double>(fovy_rad) / 2);

    if (tan == 0)
        throw std::runtime_error("Can't compute cotangent");

    rcbe::math::Matrix4x4 ret {
        1 / (tan * aspect), 0, 0, 0,
        0, 1 / tan, 0, 0,
        0, 0, -((far + near) / (far - near)), -(2 * far * near) / (far - near),
        0, 0, -1, 0};

    return ret;
}

rcbe::math::Matrix4x4 makePerspectiveVulkan(const rcbe::core::EngineScalar near,
                                            const rcbe::core::EngineScalar far,
                                            const rcbe::math::deg fov,
                                            const rcbe::core::IntegralDimensions viewport) {
    const auto aspect = static_cast<float>(viewport.width) / static_cast<float>(viewport.height);
    const auto fovy_rad = static_cast<float>(fov.as_rad());

    const auto tan = std::tan(static_cast<const double>(fovy_rad) / 2);

    if (tan == 0)
        throw std::runtime_error("Can't compute cotangent");

    const auto half_tan_fov = std::tan(fovy_rad / static_cast<float>(2));

    rcbe::math::Matrix4x4 ret {
        1 / (aspect * half_tan_fov), 0.0, 0.0, 0.0,
        0.0, -(1 / half_tan_fov), 0.0, 0.0,
        0.0, 0.0, far / (near - far), -(far * near) / (far - near),
        0.0, 0.0, -1.0, 0.0};

    return ret;
}

}// namespace

namespace rdmn::render {
rcbe::math::Matrix4x4 makePerspectiveMatrix(
    const rcbe::core::EngineScalar near,
    const rcbe::core::EngineScalar far,
    const rcbe::math::deg fov,
    const rcbe::core::IntegralDimensions viewport) {
#ifdef RDMN_VULKAN
    return makePerspectiveVulkan(near, far, fov, viewport);
#elif defined(RDMN_OPENGL)
    return makePerspectiveGL(near, far, fov, viewport);
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
#else
    static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif
}
}// namespace rdmn::render
