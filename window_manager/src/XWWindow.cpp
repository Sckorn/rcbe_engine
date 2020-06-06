#include <future>

#include <core/XWWindow.hpp>
#ifdef __linux__
#include "XWindow.hpp"
#elif __WIN32
#include "WinWindow.hpp"
#endif

#include <core/AbstractInputManager.hpp>

namespace rcbe::core {
XWWindow::XWWindow(WindowConfig &&config_, Display* root_display, int screen_number, const Window& root_window, const Atom& delete_msg)
:
impl_ { std::make_shared<XWindow>(std::move(config_), root_display, screen_number, root_window, delete_msg) }
{

}

XWWindow::~XWWindow() {

}

void XWWindow::kill() {
    impl_->kill();
}

void XWWindow::show() {
    impl_->map_window();
}

const core::AbstractInputManager& XWWindow::get_input_manager() const {
    return impl_->get_input_manager();
}

const WindowConfig& XWWindow::get_config() const {
    return impl_->get_config();
}

const std::shared_ptr<rendering::RenderingContext>& XWWindow::get_context() const {
    return impl_->get_context();
}

void XWWindow::set_renderer(rcbe::rendering::GLRendererPtr renderer_ptr) {
    impl_->set_renderer(std::move(renderer_ptr));
}

const rcbe::rendering::GLRendererPtr& XWWindow::get_renderer() const {
    return impl_->get_renderer();
}

void XWWindow::start_window_loop() {
    impl_->start_window_loop();
}

void XWWindow::stop_window_loop() {
    impl_->stop_window_loop();
}

void XWWindow::on_configure(window::configure_handler_t&& handler) {
    impl_->on_configure(std::move(handler));
}

void XWWindow::on_unmap(window::unmap_handler_t&& handler) {
    impl_->on_unmap(std::move(handler));
}
}
