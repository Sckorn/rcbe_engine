#ifndef RCBE_RENDERINGCONTEXT_HPP
#define RCBE_RENDERINGCONTEXT_HPP

#include <memory>
#include <atomic>
#include <mutex>

#include <X11/X.h>
#include <X11/Xlib.h>

#include <GL/glx.h>

#include <boost/log/trivial.hpp>

#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>
#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/Transform.hpp>
#include <rcbe-engine/datamodel/math/MatrixColumnMajorAdaptor.hpp>

namespace rcbe::rendering
{
class RenderingContext {
public:
    RenderingContext() = default;
    ~RenderingContext() = default;

    RenderingContext(const RenderingContext& other) = delete;
    RenderingContext &operator=(const RenderingContext& other) = delete;

    RenderingContext(RenderingContext&& other) = delete;
    RenderingContext &operator=(RenderingContext&& other) = delete;

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

    void setDisplay(Display* d);
    [[nodiscard]] Display* getDisplay() const noexcept;

    void setDeleteMessage(Atom a);
    [[nodiscard]] Atom getDeleteMessage() const noexcept;

    void setGlxContext(GLXContext glxc);
    [[nodiscard]] GLXContext getGlxContext() const noexcept;

    void setDrawable(GLXDrawable d);
    [[nodiscard]] GLXDrawable getDrawable() const noexcept;

    void setBackgroundColor(const rcbe::visual::RGBAColor& color);
    [[nodiscard]] const rcbe::visual::RGBAColor& getBackgroundColor() const noexcept;

    void setWindowDimensions(const rcbe::core::Dimensions& d);
    [[nodiscard]] const rcbe::core::Dimensions& getWindowDimensions() const;

    void updateTransform(const rcbe::math::Transform& trn);
    [[nodiscard]] const math::MatrixColumnMajorAdaptor& getTransformColumnMajor() const;

    void setMouseCoordinates(const rcbe::math::Vector2d& v);
    [[nodiscard]] const std::optional<rcbe::math::Vector2d>& getMouseCoordinates() const;

    [[nodiscard]] rcbe::core::EngineScalar getZoom() const;

    void updateZoom(const rcbe::core::EngineScalar zoom);

    [[nodiscard]]double deltaTime() const;
    void setCurrentTime(std::chrono::microseconds t);
private:
    mutable std::mutex transform_mutex_;
    mutable std::mutex dimensions_mutex_;
    mutable std::mutex mouse_mutex_;
    mutable std::mutex zoom_mutex_;
    mutable std::mutex time_mutex_;

    // I presume pointer to X Display, should not be deleted, research
    Display* x_display_ = nullptr;
    Atom x_delete_message_;
    GLXContext gl_x_context_;
    GLXDrawable gl_x_window_;
    rcbe::visual::RGBAColor background_color_;
    rcbe::core::Dimensions window_dimensions_;

    math::MatrixColumnMajorAdaptor scene_transform_;

    std::optional<rcbe::math::Vector2d> mouse_coordinates_;
    rcbe::core::EngineScalar zoom_ = 45.;

    uint64_t previous_time_ = 0;
    uint64_t current_time_ = 0;
};

using RenderingContextPtr = std::shared_ptr<RenderingContext>;
using RenderingContextConstPtr = std::shared_ptr<const RenderingContext>;

using RendererStopHandlerType = std::function<void(RenderingContextPtr)>; // context ptr reserved for future use
}

#endif // RCBE_RENDERINGCONTEXT_HPP