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

    // I presume pointer to X Display, should not be deleted, research
    Display* x_display;
    Atom x_delete_message;
    GLXContext gl_x_context;
    GLXDrawable gl_x_window;
    rcbe::visual::RGBAColor background_color;
    rcbe::core::Dimensions window_dimensions;
};
}

#endif // RCBE_RENDERINGCONTEXT_HPP