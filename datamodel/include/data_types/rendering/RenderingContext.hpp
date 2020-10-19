#ifndef RCBE_RENDERINGCONTEXT_HPP
#define RCBE_RENDERINGCONTEXT_HPP

#include <memory>
#include <atomic>

#include <X11/X.h>
#include <X11/Xlib.h>

#include <GL/glx.h>

#include <boost/log/trivial.hpp>

#include <data_types/visual/RGBAColor.hpp>
#include <data_types/core/Dimensions.hpp>
#include <data_types/math/Matrix.hpp>
#include <data_types/math/Transform.hpp>
#include <data_types/math/MatrixColumnMajorAdaptor.hpp>
#include <mutex>

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

    void gl_context_from_this() const {
        auto status = glXMakeCurrent(x_display_, gl_x_window_, gl_x_context_);
        if (status == False) {
            throw std::runtime_error("Can't set rendering context from current GLX context");
        }
    }

    void gl_context_from_default() const {
        auto status = glXMakeCurrent(x_display_, None, nullptr);
        if (status == False) {
            throw std::runtime_error("Can't set rendering context to default value");
        }
    }

    void set_display(Display* d);
    [[nodiscard]] Display* get_display() const;

    void set_delete_message(Atom a);
    [[nodiscard]] Atom get_delete_message() const;

    void set_glx_context(GLXContext glxc);
    [[nodiscard]] GLXContext get_glx_context() const;

    void set_drawable(GLXDrawable d);
    [[nodiscard]] GLXDrawable get_drawable() const;

    void set_background_color(const rcbe::visual::RGBAColor& color);
    [[nodiscard]] const rcbe::visual::RGBAColor& get_background_color() const;

    void set_window_dimensions(const rcbe::core::Dimensions& d);
    [[nodiscard]] const rcbe::core::Dimensions& get_window_dimensions() const;

    void update_transform(const rcbe::math::Transform& trn);
    [[nodiscard]] const math::MatrixColumnMajorAdaptor& get_transform_column_major() const;

    void set_mouse_coordinates(const rcbe::math::Vector2d& v);
    [[nodiscard]] const std::optional<rcbe::math::Vector2d>& get_mouse_coordinates() const;

    [[nodiscard]] rcbe::core::EngineScalar get_zoom() const;

    void update_zoom(const rcbe::core::EngineScalar zoom);

    [[nodiscard]]double delta_time() const;
    void set_current_time(std::chrono::microseconds t);
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

using renderer_stop_handler_t = std::function<void(RenderingContextPtr)>; // context ptr reserved for future use
}

#endif // RCBE_RENDERINGCONTEXT_HPP