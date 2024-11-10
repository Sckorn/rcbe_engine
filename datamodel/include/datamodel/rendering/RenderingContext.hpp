#ifndef RCBE_RENDERINGCONTEXT_HPP
#define RCBE_RENDERINGCONTEXT_HPP

#include <atomic>
#include <memory>
#include <mutex>
#include <optional>

#ifdef __linux__
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/MatrixColumnMajorAdaptor.hpp>
#include <rcbe-engine/datamodel/math/Transform.hpp>
#include <rcbe-engine/datamodel/math/rotation_units.hpp>
#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>

namespace rcbe::rendering {
class R_PUBLIC_API RenderingContext {
public:

    using TimePointType = std::chrono::steady_clock::time_point;

    RenderingContext() = default;
    ~RenderingContext() = default;

    RenderingContext(const RenderingContext &other) = delete;
    RenderingContext &operator=(const RenderingContext &other) = delete;

    RenderingContext(RenderingContext &&other) = delete;
    RenderingContext &operator=(RenderingContext &&other) = delete;

#ifdef RDMN_OPENGL
    void glContextFromThis() const {
        auto status = glXMakeCurrent(x_display_, gl_x_window_, gl_x_context_);
        if (status == False) {
            throw std::runtime_error("Can't set rendering context from current GLX context");
        }
    }

    void glContextFromDefault() const {
        auto status = glXMakeCurrent(x_display_, None, nullptr);
        if (status == False) {
            throw std::runtime_error("Can't set rendering context to default value");
        }
    }
#endif

#ifdef __linux__
    void setDisplay(Display *d);
    [[nodiscard]] Display *getDisplay() const noexcept;

    void setDeleteMessage(Atom a);
    [[nodiscard]] Atom getDeleteMessage() const noexcept;

    void setGlxContext(GLXContext glxc);
    [[nodiscard]] GLXContext getGlxContext() const noexcept;

    void setDrawable(GLXDrawable d);
    [[nodiscard]] GLXDrawable getDrawable() const noexcept;

    [[nodiscard]] VisualID visualId() const;
    void setVisualId(VisualID id);
#endif

    void setBackgroundColor(const rcbe::visual::RGBAColor &color);
    [[nodiscard]] const rcbe::visual::RGBAColor &getBackgroundColor() const noexcept;

    void setWindowDimensions(const rcbe::core::IntegralDimensions &d);
    [[nodiscard]] const rcbe::core::IntegralDimensions &getWindowDimensions() const;

    void updateTransform(const rcbe::math::Transform &trn);
    [[nodiscard]] const math::MatrixColumnMajorAdaptor<core::EngineScalar> &getSceneTransformColumnMajor() const;
    [[nodiscard]] const math::Transform &getSceneTransform() const;

    void setMouseCoordinates(const rcbe::math::Vector2d &v);
    [[nodiscard]] const std::optional<rcbe::math::Vector2d> &getMouseCoordinates() const;

    [[nodiscard]] rcbe::math::deg getFov() const;

    void updateFov(rcbe::math::deg zoom);

    void startTime(TimePointType &&t);
    [[nodiscard]] float computeDeltaTime();

private:

    mutable std::mutex transform_mutex_;
    mutable std::mutex dimensions_mutex_;
    mutable std::mutex mouse_mutex_;
    mutable std::mutex zoom_mutex_;
    mutable std::mutex time_mutex_;
    mutable std::mutex visualid_mutex_;

    // I presume pointer to X Display, should not be deleted, research
#ifdef __linux__
    Display *x_display_ = nullptr;
    Atom x_delete_message_;
    GLXContext gl_x_context_;
    GLXDrawable gl_x_window_;
    VisualID visual_id_;
#endif

    rcbe::visual::RGBAColor background_color_;
    rcbe::core::IntegralDimensions window_dimensions_;

    math::MatrixColumnMajorAdaptor<core::EngineScalar> scene_transform_;
    math::Transform rowmajor_scene_transform_;

    std::optional<rcbe::math::Vector2d> mouse_coordinates_;

    /// TODO: read this from config @sckorn @radameon
    rcbe::math::deg zoom_ = rcbe::math::deg(45.);

    TimePointType start_time_;
    float delta_time_ = 0;

#ifdef RDMN_OPENGL
    uint64_t previous_time_ = 0;
    uint64_t current_time_ = 0;
#endif
};

using RenderingContextPtr = std::shared_ptr<RenderingContext>;
using RenderingContextConstPtr = std::shared_ptr<const RenderingContext>;

using RendererStopHandlerType = std::function<void(RenderingContextPtr)>;// context ptr reserved for future use
}// namespace rcbe::rendering

#endif// RCBE_RENDERINGCONTEXT_HPP