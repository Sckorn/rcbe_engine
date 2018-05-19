#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <gtkmm/main.h>

#include "config/GlobalEngineConfig.h"
#include "config/config.h"
#include "gui/Window.h"
#include "common/utils/file_utils.h"
#include "gui/window_utils.h"

int main(int argc, char * argv[])
{
  using namespace rcbe;
  namespace po = boost::program_options;
  po::options_description desc("Allowed options");
  desc.add_options()
        ("help,h", "produce help message")
        ("config,c", po::value<std::string>(), "provide configuration file");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    std::cout << desc << std::endl;
    return 0;
  }

  try
  {
    config::GlobalEngineConfig conf;

    if(vm.count("config"))
    {
      auto config_file_path = vm["config"].as<std::string>();
      conf = config::utils::readFromFile<config::GlobalEngineConfig>(config_file_path);
      conf.app_base_path = boost::filesystem::path(
        argv[0]
      ).parent_path().string();
      std::cout << std::boolalpha << conf.debug << std::endl;
    }

    if(conf.toolkit_gui_config_path.empty())
    {
      std::cerr << "GUI config path is not specified!" << std::endl;
      return 1;
    }

    auto gui_config_file_path = common::makeStringPathFromParts(
      conf.app_base_path,
      conf.toolkit_gui_config_path
      );

    std::cout << gui_config_file_path << std::endl;

    auto gui_config = config::utils::readFromFile<config::ToolkitGUIConfig>(gui_config_file_path);

    auto main_widget_path = common::makeStringPathFromParts(
      conf.app_base_path,
      gui_config.widgets_dir_path,
      gui_config.main_window_widget_file_name
    );

    std::cout << main_widget_path << std::endl;

    Gtk::Main kit(argc, argv);

    rcbe::toolkit::gui::Window main_window(
      0, 0,
      500, 500,
      conf.application_name
    );

    auto main_widget = rcbe::toolkit::gui::makeWindowFromFile(main_widget_path);
    main_widget->show();
    kit.run(*main_widget);
  }
  catch (const std::exception &exc)
  {
    std::cerr << "Exception in main(): " << exc.what() << std::endl;
    return 1;
  }

  return 0;
}