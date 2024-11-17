#include <future>

#include <rcbe-engine/core/RWindow.hpp>
#ifdef __linux__
#include "XWindow.hpp"
#elif defined(_WIN32)
#include "MsWindow.hpp"

#endif

#ifdef TMP
#include <rcbe-engine/core/AbstractInputManager.hpp>
#endif

namespace rcbe::core {
RWindow::RWindow(window_config &&config_, const WindowContextPtr &window_context)
#ifdef __linux__
    : impl_ {std::make_shared<XWindow>(std::move(config_), window_context)} {
#elif defined(_WIN32)
    : impl_ {std::make_shared<MsWindow>(std::move(config_), window_context)} {
#endif
}

RWindow::~RWindow() {
}

void RWindow::kill() {
    impl_->kill();
}

void RWindow::show() {
    impl_->show();
}

const std::shared_ptr<core::AbstractInputManager> &RWindow::getInputManager() const {
    return impl_->getInputManager();
}

void RWindow::setInputManager(const std::shared_ptr<AbstractInputManager> &manager) {
    impl_->setInputManager(manager);
}

const window_config &RWindow::getConfig() const {
    return impl_->getConfig();
}

const std::shared_ptr<rendering::RenderingContext> &RWindow::getRenderingContext() const {
    return impl_->getRenderingContext();
}

void RWindow::setRenderer(rdmn::render::RendererPtr renderer_ptr) {
    impl_->setRenderer(std::move(renderer_ptr));
}

const rdmn::render::RendererPtr &RWindow::getRenderer() const {
    return impl_->getRenderer();
}

std::optional<RWindow::GraphicContext> RWindow::getGraphicContext() const {
    return impl_->getGraphicContext();
}

void RWindow::startWindowLoop() {
    impl_->startWindowLoop();
}

void RWindow::stopWindowLoop() {
    impl_->stopWindowLoop();
}

void RWindow::onConfigure(window::ConfigureHandlerType &&handler) {
    impl_->onConfigure(std::move(handler));
}

void RWindow::onUnmap(window::UunmapHandlerType &&handler) {
    impl_->onUnmap(std::move(handler));
}

#ifdef _WIN32
HWND RWindow::getWindowHandle() const {
    return impl_->getWindowHandle();
}
#endif
}// namespace rcbe::core
