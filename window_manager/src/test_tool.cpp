#include <future>

#include <rcbe-engine/datamodel/system/window_config.hpp>
#include <core/WindowManager.hpp>

#include <rcbe-engine/utils/json_utils.hpp>

// TODO: this can be deleted

int main(int argc, char * argv[]) {
    rcbe::core::WindowManager manager { true };
    rcbe::core::window_config config = rcbe::utils::read_from_file<rcbe::core::window_config>(
            "datamodel/data/system/default_window_config.json");

    auto window = manager.create_window(std::move(config));

    auto window_handler = window->start_window_loop_aync();

    window->show();

    window_handler.wait();
    return 0;

}

