#include <rcbe-engine/core/WindowManager.hpp>
#include <rcbe-engine/utils/output_utils.hpp>
#include <rcbe-engine/datamodel/system/window_config.hpp>
#include <rcbe-engine/core/AbstractInputManager.hpp>
#include <rcbe-engine/core/EditorInputManager.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>

#include <rcbe-engine/parsers/tga/tga_parser.hpp>

#include <boost/log/trivial.hpp>

int main(int argc, char* argv[]) {
    try {
        rcbe::utils::setup_logging();
        rcbe::core::WindowManager wm { true };

        rcbe::core::window_config window_conf;
        window_conf.caption = "Testing TGA";
        window_conf.position = rcbe::math::Vector2d { 0.0, 0.0 };
        window_conf.background_color = rcbe::visual::RGBAColor { 0.0, 0.0, 0.0, 1.0 };
        window_conf.editor = true;
        window_conf.fullscreen = false;
        window_conf.input_scheme = "";
        window_conf.size = rcbe::core::IntegralDimensions {1240, 720 };
        window_conf.process_input = true;
        window_conf.type = decltype(window_conf.type)::drawing_window;

        auto window = wm.createWindow(std::move(window_conf));

        rcbe::core::EditorInputManager::HandlerCollection handlers = {
                {
                    rcbe::core::InputEventType::button_press, [window](
                        rcbe::core::InputManagerImplementation& im,
                        rcbe::core::InputManagerImplementation::InputEventReference event,
                        rcbe::core::InputManagerImplementation::PreviousEventReference previous
                    ) {
                        int x=event.xbutton.x, y=event.xbutton.y;
                        BOOST_LOG_TRIVIAL(debug) << "Mouse button pressed " << x << " " << y;

                        auto optional_gc = window->getGraphicContext();

                        const auto red_pixel = 0xFF0000; // for some strange reason if specified with alpha channel (even zeroed out)
                        // it turns pixels to black;
                        // TODO: figure out what to do with the above

                        if (optional_gc) {
                            XSetForeground(window->getRenderingContext()->getDisplay(), *optional_gc, red_pixel);

                            XDrawPoint(
                                    window->getRenderingContext()->getDisplay(),
                                    window->getRenderingContext()->getDrawable(),
                                    *optional_gc, x, y);
                        }
                    }
                },
                {
                    rcbe::core::InputEventType::key_press, [window](
                        rcbe::core::InputManagerImplementation& im,
                        rcbe::core::InputManagerImplementation::InputEventReference event,
                        rcbe::core::InputManagerImplementation::PreviousEventReference previous
                    ) {
                        if (window->getInputManager()->getValue(rcbe::core::KeyboardEventType::enter)) {
                            BOOST_LOG_TRIVIAL(debug) << "Key pressed";

                            rcbe::visual::texture_config tconf {};

                            tconf.component_order = decltype(tconf.component_order)::GBR;
                            rcbe::visual::Texture tex {
                                "external/awesomeface_texture/file/awesomeface_texture.tga",
                                rdmn::parse::tga::parse
                            };
                            const auto &pixels = tex.getPixels();
                            auto optional_gc = window->getGraphicContext();

                            if (optional_gc) {
                                for (size_t i = 0; i < tex.getWidth(); ++i) {
                                    for (size_t j = 0; j < tex.getHeight(); ++j) {
                                        const auto pixel_color = pixels.at(i, j);

                                        XSetForeground(window->getRenderingContext()->getDisplay(), *optional_gc, pixel_color);
                                        XDrawPoint(window->getRenderingContext()->getDisplay(), window->getRenderingContext()->getDrawable(), *optional_gc, i, j);
                                    }
                                }
                            }
                        }
                    }
                }
        };

        rcbe::core::EditorInputManager eim { std::move(handlers) };

        window->setInputManager(std::make_shared<rcbe::core::AbstractInputManager>(std::move(eim)));

        window->show();

        auto window_handle = window->startWindowLoopAsync();

        window_handle.wait();
    } catch (const std::exception &e) {
        BOOST_LOG_TRIVIAL(error) << "Exception thrown in main: " << e.what();
        return 1;
    }
    return 0;
}