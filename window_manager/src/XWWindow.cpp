#include <future>

#include <rcbe-engine/core/XWWindow.hpp>
#ifdef __linux__
#include "XWindow.hpp"
#elif _WIN32
#include "WinWindow.hpp"
#endif

#include <rcbe-engine/core/AbstractInputManager.hpp>

namespace rcbe::core {
XWWindow::XWWindow(window_config &&config_, const WindowContextPtr &window_context)
:
impl_ { std::make_shared<XWindow>(std::move(config_), window_context) }
{

}

XWWindow::~XWWindow() {

}

void XWWindow::kill() {
    impl_->kill();
}

void XWWindow::show() {
    impl_->mapWindow();
}

const std::shared_ptr<core::AbstractInputManager> &XWWindow::getInputManager() const {
    return impl_->getInputManager();
}

void XWWindow::setInputManager(const std::shared_ptr<AbstractInputManager> &manager) {
    impl_->setInputManager(manager);
}

const window_config &XWWindow::getConfig() const {
    return impl_->getConfig();
}

const std::shared_ptr<rendering::RenderingContext> &XWWindow::getRenderingContext() const {
    return impl_->getRenderingContext();
}

void XWWindow::setRenderer(rdmn::render::RendererPtr renderer_ptr) {
    impl_->setRenderer(std::move(renderer_ptr));
}

const rdmn::render::RendererPtr &XWWindow::getRenderer() const {
    return impl_->getRenderer();
}

std::optional<GC> XWWindow::getGraphicContext() const {
    return impl_->getGraphicContext();
}

void XWWindow::startWindowLoop() {
    impl_->startWindowLoop();
}

void XWWindow::stopWindowLoop() {
    impl_->stopWindowLoop();
}

void XWWindow::onConfigure(window::ConfigureHandlerType &&handler) {
    impl_->onConfigure(std::move(handler));
}

void XWWindow::onUnmap(window::UunmapHandlerType &&handler) {
    impl_->onUnmap(std::move(handler));
}
}
