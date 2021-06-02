#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>


namespace rcbe::rendering {
void RenderingContext::setDisplay(Display* d) {
    x_display_ = d;
}

Display *RenderingContext::getDisplay() const noexcept {
    return x_display_;
}

void RenderingContext::setDeleteMessage(Atom a) {
    x_delete_message_ = a;
}

Atom RenderingContext::getDeleteMessage() const noexcept {
    return x_delete_message_;
}

void RenderingContext::setGlxContext(GLXContext glxc) {
    gl_x_context_ = glxc;
}

GLXContext RenderingContext::getGlxContext() const noexcept {
    return gl_x_context_;
}

void RenderingContext::setDrawable(GLXDrawable d) {
    gl_x_window_ = d;
}

GLXDrawable RenderingContext::getDrawable() const noexcept {
    return gl_x_window_;
}

void RenderingContext::setBackgroundColor(const rcbe::visual::RGBAColor& color) {
    background_color_ = color;
}

const rcbe::visual::RGBAColor &RenderingContext::getBackgroundColor() const noexcept {
    return background_color_;
}

void RenderingContext::setWindowDimensions(const rcbe::core::Dimensions& d) {
    std::lock_guard lg {dimensions_mutex_};

    window_dimensions_ = d;
}

const rcbe::core::Dimensions &RenderingContext::getWindowDimensions() const {
    std::lock_guard lg {dimensions_mutex_};

    return window_dimensions_;
}

void RenderingContext::updateTransform(const rcbe::math::Transform& trn) {
    std::lock_guard lg {transform_mutex_};

    scene_transform_ = rcbe::math::MatrixColumnMajorAdaptor(trn.matrix());
}

const math::MatrixColumnMajorAdaptor<core::EngineScalar> &RenderingContext::getTransformColumnMajor() const {
    std::lock_guard lg {transform_mutex_};

    return scene_transform_;
}

void RenderingContext::setMouseCoordinates(const rcbe::math::Vector2d& v) {
    std::lock_guard lg {mouse_mutex_};

    mouse_coordinates_ = v;
}

const std::optional<rcbe::math::Vector2d> &RenderingContext::getMouseCoordinates() const {
    std::lock_guard lg {mouse_mutex_};

    return mouse_coordinates_;
}

rcbe::math::deg RenderingContext::getFov() const {
    std::lock_guard lg {zoom_mutex_};

    return zoom_;
}

void RenderingContext::updateFov(const rcbe::math::deg zoom) {
    std::lock_guard lg {zoom_mutex_};

    zoom_ = zoom;
}

double RenderingContext::deltaTime() const {
    std::lock_guard lg{time_mutex_};
    auto delta_time = std::abs(static_cast<long long>(current_time_ - previous_time_));
    BOOST_LOG_TRIVIAL(debug) << "Delta " << delta_time << " and divided " << static_cast<double>(delta_time) / 1000000;
    return static_cast<double>(delta_time) / 1000000;
}

void RenderingContext::setCurrentTime(std::chrono::microseconds t) {
    std::lock_guard lg{time_mutex_};
    previous_time_ = current_time_;
    current_time_ = t.count();
}
}