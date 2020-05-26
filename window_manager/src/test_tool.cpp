#include <future>

#include <data_types/system/WindowConfig.hpp>
#include <core/WindowManager.hpp>

#include <common/utils/json_utils.hpp>

int main(int argc, char * argv[]) {
    rcbe::core::WindowManager manager { true };
    rcbe::core::WindowConfig config = rcbe::utils::readFromFile<rcbe::core::WindowConfig>("datamodel/data/system/default_window_config.json");

    auto window = manager.create_window(std::move(config));

    auto window_handler = window->start_window_loop_aync();

    window->map_window();

    window_handler.wait();
    return 0;

}

