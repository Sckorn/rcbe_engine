#include <data_types/rendering/RenderingContext.hpp>


namespace rcbe::rendering {
void RenderingContext::set_display(Display* d) {
    x_display_ = d;
}

Display* RenderingContext::get_display() const {
    return x_display_;
}

void RenderingContext::set_delete_message(Atom a) {
    x_delete_message_ = a;
}

Atom RenderingContext::get_delete_message() const {
    return x_delete_message_;
}

void RenderingContext::set_glx_context(GLXContext glxc) {
    gl_x_context_ = glxc;
}

GLXContext RenderingContext::get_glx_context() const {
    return gl_x_context_;
}

void RenderingContext::set_drawable(GLXDrawable d) {
    gl_x_window_ = d;
}

GLXDrawable RenderingContext::get_drawable() const {
    return gl_x_window_;
}

void RenderingContext::set_background_color(const rcbe::visual::RGBAColor& color) {
    background_color_ = color;
}

const rcbe::visual::RGBAColor& RenderingContext::get_background_color() const {
    return background_color_;
}

void RenderingContext::set_window_dimensions(const rcbe::core::Dimensions& d) {
    std::lock_guard lg {dimensions_mutex_};

    window_dimensions_ = d;
}

const rcbe::core::Dimensions& RenderingContext::get_window_dimensions() const {
    std::lock_guard lg {dimensions_mutex_};

    return window_dimensions_;
}

void RenderingContext::update_transform(const rcbe::math::Transform& trn) {
    std::lock_guard lg {transform_mutex_};

    scene_transform_ = rcbe::math::MatrixColumnMajorAdaptor(trn.matrix());
}

const math::MatrixColumnMajorAdaptor& RenderingContext::get_transform_column_major() const {
    std::lock_guard lg {transform_mutex_};

    return scene_transform_;
}

void RenderingContext::set_mouse_coordinates(const rcbe::math::Vector2d& v) {
    std::lock_guard lg {mouse_mutex_};

    mouse_coordinates_ = v;
}

const std::optional<rcbe::math::Vector2d>& RenderingContext::get_mouse_coordinates() const {
    std::lock_guard lg {mouse_mutex_};

    return mouse_coordinates_;
}

rcbe::core::EngineScalar RenderingContext::get_zoom() const {
    std::lock_guard lg {zoom_mutex_};

    return zoom_;
}

void RenderingContext::update_zoom(const rcbe::core::EngineScalar zoom) {
    std::lock_guard lg {zoom_mutex_};

    zoom_ = zoom;
}

double RenderingContext::delta_time() const {
    std::lock_guard lg{time_mutex_};
    auto delta_time = std::abs(static_cast<long long>(current_time_ - previous_time_));
    BOOST_LOG_TRIVIAL(debug) << "Delta " << delta_time << " and divided " << static_cast<double>(delta_time) / 1000000;
    return static_cast<double>(delta_time) / 1000000;
}

void RenderingContext::set_current_time(std::chrono::microseconds t) {
    std::lock_guard lg{time_mutex_};
    previous_time_ = current_time_;
    current_time_ = t.count();
}
}