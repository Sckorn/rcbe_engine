#include "MsWindow.hpp"

#include <rdmn-engine/logger/trivial_logger.hpp>

namespace rcbe::core {

MsWindow::MsWindow(window_config &&config, const WindowContextPtr &context)
    : config_(std::move(config))
    , context_(context) {
    if (!this->create(config_.caption.c_str(), WS_OVERLAPPEDWINDOW, context_->getWindowClass(), /*dwExStyle=*/0, config_.position.x(), config_.position.y(), config_.size.width, config_.size.height))
        throw std::runtime_error("Can't create window!");

    rendering_context_ = std::make_shared<rcbe::rendering::RenderingContext>();

    rendering_context_->setWindow(getWindowHandle());
    rendering_context_->setInstance(context_->getInstanceHandle());
    rendering_context_->setWindowDimensions(config_.size);
}

MsWindow::~MsWindow() {}

void MsWindow::startWindowLoop() {
    bool expected = running_.load();
    running_.compare_exchange_strong(expected, true);

    windowLoop();
}

void MsWindow::stopWindowLoop() {
    bool expected = running_.load();
    running_.compare_exchange_strong(expected, false);
}

void MsWindow::show() {
    ShowWindow(getWindowHandle(), context_->getShowCommand());
}

LRESULT MsWindow::handleMessage(rdmn::core::windows_input_event we) {
    if (!config_.process_input) return true;

    if (input_manager_) {
        if (input_manager_->tryProcessEvent(we)) return true;
    }

    switch (we.type) {

        case WM_CREATE: {
            if (configure_handler_)
                configure_handler_();
        }
            return false;

        case WM_DESTROY: {
            if (unmap_handler_)
                unmap_handler_();
            PostQuitMessage(0);
            stopWindowLoop();
        }
            return false;

        case WM_SIZE: {
            RDMN_LOG(RDMN_LOG_DEBUG) << "Resize event recieved";
            UINT width = LOWORD(we.lparam);
            UINT height = HIWORD(we.lparam);
            rendering_context_->setWindowDimensions({.width = static_cast<int>(width), .height = static_cast<int>(height)});
            if (renderer_) /// TODO: decouple Renderet and Window, handle this via callback. @sckorn
                renderer_->reshape();
        }
            return false;

        case WM_PAINT: {
        }
            return false;

        case WM_COMMAND: {
        }
            return false;

        default:
            return DefWindowProc(window_handle_, we.type, we.wparam, we.lparam);
    }
    return true;
}

void MsWindow::windowLoop() {
    MSG msg {};
    while (GetMessage(&msg, nullptr, 0, 0) && running_.load()) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void MsWindow::setWindowHandle(HWND hwnd) {
    window_handle_ = hwnd;
}

HWND MsWindow::getWindowHandle() const {
    return window_handle_;
}

std::optional<HDC> MsWindow::getGraphicContext() const {
    return GetDC(window_handle_);
}

void MsWindow::kill() {
    stopWindowLoop();
    DestroyWindow(window_handle_);
}

void MsWindow::onConfigure(window::ConfigureHandlerType &&handler) {
    configure_handler_ = std::move(handler);
}

void MsWindow::onUnmap(window::UunmapHandlerType &&handler) {
    unmap_handler_ = std::move(handler);
}

const rcbe::rendering::RenderingContextPtr &MsWindow::getRenderingContext() const {
    return rendering_context_;
}

const window_config &MsWindow::getConfig() const {
    return config_;
}

const std::shared_ptr<rcbe::core::AbstractInputManager> &MsWindow::getInputManager() const {
    return input_manager_;
}

void MsWindow::setInputManager(const std::shared_ptr<rcbe::core::AbstractInputManager> &manager) {
    input_manager_ = manager;
}

void MsWindow::setRenderer(rdmn::render::RendererPtr renderer_ptr) {
    renderer_ = std::move(renderer_ptr);
}

const rdmn::render::RendererPtr &MsWindow::getRenderer() const {
    return renderer_;
}
}// namespace rcbe::core