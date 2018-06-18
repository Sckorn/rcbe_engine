#ifndef RCBE_ENGINE_WINDOWMANAGER_H
#define RCBE_ENGINE_WINDOWMANAGER_H

#include <stack>
#include <include/config/WindowManagerConfig.h>

#include "gui/Window.h"
#include "gui/window_utils.h"

#include <gtkmm/main.h>

namespace rcbe::toolkit::gui
{

void windowManagerWorker(std::string app_path, config::WindowManagerConfig config, Gtk::Main kit);

class WindowManager
{
public:
  WindowManager() = delete;
  WindowManager(
    const std::string &app_path,
    const config::WindowManagerConfig &config,
    Gtk::Main &&kit
  )
  :
  config(config),
  kit(std::move(kit)),
  app_path(app_path)
  {
    init();
  };

  WindowManager(const WindowManager &source) = delete;
  WindowManager &operator=(const WindowManager &source) = delete;

  int start();

private:

  void init();

  std::string app_path;

  config::WindowManagerConfig config;

  std::unordered_map<std::string, std::shared_ptr<Gtk::Window>> windows;

  Gtk::Main kit;
};
}

#endif //RCBE_ENGINE_WINDOWMANAGER_H
