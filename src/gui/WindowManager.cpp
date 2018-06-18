#include "gui/WindowManager.h"
#include "common/utils/file_utils.h"
#include <iostream>

namespace rcbe::toolkit::gui
{
void windowManagerWorker(std::string app_path, config::WindowManagerConfig config, Gtk::Main kit)
{
  WindowManager manager(app_path, config, std::move(kit));
  auto ret = manager.start();
  std::cout << "Worker thread stopped " << ret << std::endl;
}

void WindowManager::init()
{
  auto main_widget_path = rcbe::common::makeStringPathFromParts(
    app_path,
    config.widgets_dir_path,
    config.widgets.file_name
  );

  std::cout << main_widget_path << std::endl;
  main_window = rcbe::toolkit::gui::makeWindowFromFile(main_widget_path);
}

int WindowManager::start()
{
  main_window->show();
  kit.run(*main_window);
  return 0;
}
}