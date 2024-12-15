#ifndef RDMN_MS_WINDOW_HPP
#define RDMN_MS_WINDOW_HPP

#include <atomic>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <type_traits>

#include <windows.h>

#include <rcbe-engine/core/AbstractInputManager.hpp>
#include <rcbe-engine/core/windows_input_event.hpp>
#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>
#include <rcbe-engine/datamodel/system/WindowContext.hpp>
#include <rcbe-engine/datamodel/system/window_config.hpp>
#include <rcbe-engine/renderer/Renderer.hpp>
#include <rdmn-engine/logger/trivial_logger.hpp>

namespace rcbe::core {
template <typename Implementation>
class BaseWindow {
public:

    static LRESULT CALLBACK windowLoopHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
        Implementation *impl_this = nullptr;

        if (msg == WM_NCCREATE) {
            CREATESTRUCT *create_struct_ptr = reinterpret_cast<CREATESTRUCT *>(lparam);
            impl_this = static_cast<Implementation *>(create_struct_ptr->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(impl_this));

            impl_this->setWindowHandle(hwnd);
        } else {
            impl_this = reinterpret_cast<Implementation *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (impl_this) {
            return impl_this->handleMessage(rdmn::core::windows_input_event {.type = msg, .wparam = wparam, .lparam = lparam});
        } else {
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }

    bool create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        std::wstring window_class_str,
        DWORD dwExStyle = 0,// DWORD dwExStyle - extended window style https://learn.microsoft.com/ru-ru/windows/win32/winmsg/extended-window-styles
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0) {
        WNDCLASS window_class {0};
        window_class.lpfnWndProc = Implementation::windowLoopHandler;
        window_class.hInstance = GetModuleHandle(nullptr);
        window_class.lpszClassName = window_class_str.c_str();
        window_class.style = CS_DBLCLKS;

        RegisterClass(&window_class);

        window_handle_ = CreateWindowEx(dwExStyle, window_class_str.c_str(), lpWindowName, dwStyle, x, y,
                                        nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(nullptr), this);

        return (window_handle_ ? true : false);
    }

    HWND getWindowHandle() const {
        return window_handle_;
    }

protected:

    BaseWindow()
        : window_handle_ {nullptr} {}

    HWND window_handle_;
};

class R_PUBLIC_API MsWindow : public BaseWindow<MsWindow> {
public:

    MsWindow() = delete;
    MsWindow(window_config &&config_, const WindowContextPtr &window_context);
    ~MsWindow();

    [[nodiscard]] const window_config &getConfig() const;

    void show();
    void kill();

    void stopWindowLoop();
    void startWindowLoop();

    LRESULT handleMessage(rdmn::core::windows_input_event we);

    [[nodiscard]] std::optional<HDC> getGraphicContext() const;
    [[nodiscard]] HWND getWindowHandle() const;
    void setWindowHandle(HWND hwnd);

    [[nodiscard]] const rcbe::rendering::RenderingContextPtr &getRenderingContext() const;

    void onConfigure(window::ConfigureHandlerType &&handler);
    void onUnmap(window::UunmapHandlerType &&handler);

    [[nodiscard]] const std::shared_ptr<core::AbstractInputManager> &getInputManager() const;
    void setInputManager(const std::shared_ptr<AbstractInputManager> &manager);

    void setRenderer(rdmn::render::RendererPtr renderer_ptr);
    [[nodiscard]] const rdmn::render::RendererPtr &getRenderer() const;

private:

    void windowLoop();

    window_config config_;
    WindowContextPtr context_;
    rcbe::rendering::RenderingContextPtr rendering_context_ {};
    rdmn::render::RendererPtr renderer_ = nullptr;
    std::shared_ptr<core::AbstractInputManager> input_manager_ = nullptr;
    std::atomic_bool running_ = false;

    window::UunmapHandlerType unmap_handler_;
    window::ConfigureHandlerType configure_handler_;
};
}// namespace rcbe::core


#endif RDMN_MS_WINDOW_HPP// RDMN_MS_WINDOW_HPP