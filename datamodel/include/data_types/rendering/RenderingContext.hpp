#ifndef RCBE_RENDERINGCONTEXT_HPP
#define RCBE_RENDERINGCONTEXT_HPP

#include <memory>
#include <atomic>

#include <X11/X.h>
#include <X11/Xlib.h>

#include <GL/glx.h>

#include <data_types/visual/RGBAColor.hpp>
#include <data_types/core/Dimensions.hpp>

namespace rcbe::rendering
{
struct RenderingContext {
    RenderingContext() = default;
    ~RenderingContext() = default;

    RenderingContext(const RenderingContext& other) = delete;
    RenderingContext &operator=(const RenderingContext& other) = delete;

    RenderingContext(RenderingContext&& other) = default;
    RenderingContext &operator=(RenderingContext&& other) = default;

    void gl_context_from_this() const {
        auto status = glXMakeCurrent(x_display, gl_x_window, gl_x_context);
        if (status == False) {
            throw std::runtime_error("Can't set rendering context from current GLX context");
        }
    }

    void gl_context_from_default() const {
        auto status = glXMakeCurrent(x_display, None, nullptr);
        if (status == False) {
            throw std::runtime_error("Can't set rendering context to default value");
        }
    }

    // I presume pointer to X Display, should not be deleted, research
    Display* x_display;
    Atom x_delete_message;
    GLXContext gl_x_context;
    GLXDrawable gl_x_window;
    rcbe::visual::RGBAColor background_color;
    rcbe::core::Dimensions window_dimensions;
};

using RenderingContextPtr = std::shared_ptr<RenderingContext>;
using RenderingContextConstPtr = std::shared_ptr<const RenderingContext>;

using renderer_stop_handler_t = std::function<void(RenderingContextPtr)>; // context ptr reserved for future use
}

#endif // RCBE_RENDERINGCONTEXT_HPP