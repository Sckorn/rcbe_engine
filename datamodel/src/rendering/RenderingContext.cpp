#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>


namespace rcbe::rendering {
void RenderingContext::setDisplay(Display *d) {
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

void RenderingContext::setBackgroundColor(const rcbe::visual::RGBAColor &color) {
    background_color_ = color;
}

const rcbe::visual::RGBAColor &RenderingContext::getBackgroundColor() const noexcept {
    return background_color_;
}

void RenderingContext::setWindowDimensions(const rcbe::core::IntegralDimensions &d) {
    std::lock_guard lg {dimensions_mutex_};

    window_dimensions_ = d;
}

const rcbe::core::IntegralDimensions &RenderingContext::getWindowDimensions() const {
    std::lock_guard lg {dimensions_mutex_};

    return window_dimensions_;
}

void RenderingContext::updateTransform(const rcbe::math::Transform &trn) {
    std::lock_guard lg {transform_mutex_};

    scene_transform_ = rcbe::math::MatrixColumnMajorAdaptor(trn.matrix());
    rowmajor_scene_transform_ = trn;
}

const math::MatrixColumnMajorAdaptor<core::EngineScalar> &RenderingContext::getSceneTransformColumnMajor() const {
    std::lock_guard lg {transform_mutex_};

    return scene_transform_;
}

const math::Transform &RenderingContext::getSceneTransform() const {
    std::lock_guard lg {transform_mutex_};

    return rowmajor_scene_transform_;
}

void RenderingContext::setMouseCoordinates(const rcbe::math::Vector2d &v) {
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

VisualID RenderingContext::visualId() const {
    std::lock_guard lg {visualid_mutex_};
    return visual_id_;
}

void RenderingContext::setVisualId(VisualID id) {
    std::lock_guard lg {visualid_mutex_};
    visual_id_ = id;
}

void RenderingContext::startTime(TimePointType &&t) {
    std::lock_guard lg {time_mutex_};
    start_time_ = t;
}

float RenderingContext::computeDeltaTime() {
    std::lock_guard lg {time_mutex_};
    auto current = std::chrono::steady_clock::now();
    delta_time_ = std::chrono::duration<float, std::chrono::seconds::period>(current - start_time_).count();
    start_time_ = current;
    return delta_time_;
}

}// namespace rcbe::rendering