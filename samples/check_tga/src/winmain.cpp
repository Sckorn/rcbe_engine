#include <Windows.h>
#include <windowsx.h>

#include <rdmn-engine/logger/trivial_logger.hpp>

#include <rcbe-engine/core/AbstractInputManager.hpp>
#include <rcbe-engine/core/EditorInputManager.hpp>
#include <rcbe-engine/core/WindowManager.hpp>
#include <rcbe-engine/datamodel/system/window_config.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>
#include <rcbe-engine/parsers/tga/tga_parser.hpp>

#include <thread>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    try {
        rcbe::core::WindowManager wm(hInstance,/*window_class=*/ L"RDMNWindow", nCmdShow);
        rcbe::core::window_config wc{};
        wc.caption = L"RCBE Engine";
        wc.background_color = rcbe::visual::RGBAColor(0.5, 0.5, 0.5, 1.0);
        wc.position = decltype(wc.position)(10.0, 10.0);
        wc.size = rcbe::core::IntegralDimensions(1024, 768);
        wc.process_input = true;
        auto main_window = wm.createWindow(std::move(wc));

        rcbe::core::EditorInputManager::HandlerCollection handlers = {
            {
                rcbe::core::InputEventType::left_button_press, 
                [main_window](
                    rcbe::core::InputManagerImplementation &im,
                    rcbe::core::InputManagerImplementation::InputEventReference event,
                    rcbe::core::InputManagerImplementation::PreviousEventReference previous
                ) {
                    auto x = GET_X_LPARAM(event.lparam);
                    auto y = GET_Y_LPARAM(event.lparam);
                    HDC hdc = *(main_window->getGraphicContext());
                    rcbe::visual::RGBAColor c(255, 255, 0, 255);
                    COLORREF color = static_cast<COLORREF>(c); // TODO: @sckorn fix color, it is now black instead of expected yellow.
                    
                    SetPixel(hdc, x, y, color); // SetPixel(HDC hdc, int x, int y, COLORREF color)
             }},
            {
                rcbe::core::InputEventType::key_press,
                [main_window](
                    rcbe::core::InputManagerImplementation &im, 
                    rcbe::core::InputManagerImplementation::InputEventReference event, 
                    rcbe::core::InputManagerImplementation::PreviousEventReference previous
                ) {
                 if (main_window->getInputManager()->getValue(rcbe::core::KeyboardEventType::enter)) {
                     rcbe::visual::texture_config tconf {};

                     tconf.component_order = decltype(tconf.component_order)::GBR;
                     rcbe::visual::Texture tex {
                         "external/awesomeface_texture/file/awesomeface_texture.tga",
                         rdmn::parse::tga::parse
                    };

                    const auto &pixels = tex.getPixels();
                    HDC hdc = *(main_window->getGraphicContext());
                    for (size_t i = 0; i < tex.getWidth(); ++i) {
                        for (size_t j = 0; j < tex.getHeight(); ++j) {
                            const auto pixel_color = pixels.at(i, j);
                            rcbe::visual::RGBAColor c(pixel_color);
                            COLORREF color = static_cast<COLORREF>(c);
                            SetPixel(hdc, i, j, color);
                        }
                    }
                 }
             }}};

        rcbe::core::EditorInputManager eim {std::move(handlers)};

        main_window->setInputManager(std::make_shared<rcbe::core::AbstractInputManager>(std::move(eim)));
        
        main_window->show();

        main_window->startWindowLoop();
    } catch (const std::exception& e) {
        std::cerr << "Exception thrown in main: " << e.what() << std::endl;
    }

    return 0; // (int)msg.wParam;
}