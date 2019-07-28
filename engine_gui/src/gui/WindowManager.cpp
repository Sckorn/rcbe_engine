#include "gui/WindowManager.h"
#include "common/utils/file_utils.h"

#include <iostream>

namespace rcbe::toolkit::gui
{

void WindowManager::simpleHandler()
{
  const auto &main_widget_first_child = config.widgets.children.front();
  std::cout << "Trying to open widget " << main_widget_first_child.id << std::endl;

  auto main_widget_first_child_path = rcbe::common::makeStringPathFromParts(
    {
      app_path,
      config.widgets_dir_path,
      main_widget_first_child.file_name
    }
  );

  std::cout << main_widget_first_child_path << std::endl;

  auto builder_widget = makeWindowFromFile(main_widget_first_child_path, main_widget_first_child.id);

  windows.insert({
                   main_widget_first_child.id,
                    builder_widget.second
                 });

  windows.at(main_widget_first_child.id)->show();

  std::cout << "Simple handler " << std::endl;
}

void windowManagerWorker(std::string app_path, config::WindowManagerConfig config, Gtk::Main kit)
{
  WindowManager manager(app_path, config, std::move(kit));
  auto ret = manager.start();
  std::cout << "Window manager worker thread stopped " << ret << std::endl;
}

void WindowManager::init()
{
  auto main_widget_path = rcbe::common::makeStringPathFromParts(
    {
      app_path,
      config.widgets_dir_path,
      config.widgets.file_name
    }
  );

  auto builder_and_widget = makeWindowFromFile(main_widget_path, config.widgets.id);

  std::cout << main_widget_path << std::endl;
  windows.insert({
    config.widgets.id,
    builder_and_widget.second
  });

  Gtk::Button * btn = nullptr;
  builder_and_widget.first->get_widget("main_button", btn);
  if(btn)
  {
    btn->signal_clicked().connect(sigc::mem_fun(*this, &rcbe::toolkit::gui::WindowManager::simpleHandler));
  }
}

int WindowManager::start()
{
  auto &main_window = windows.at(config.widgets.id);
  main_window->show();
  kit.run(*main_window);
  return 0;
}

std::pair<Glib::RefPtr<Gtk::Builder>, std::shared_ptr<Gtk::Window>>
WindowManager::makeWindowFromFile(const std::string &path, const std::string &widget_id)
{
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(path);

  Gtk::Window *pd = nullptr;

  builder->get_widget(widget_id, pd);
  std::shared_ptr<Gtk::Window> ptr = nullptr;
  ptr.reset(pd);

  return {builder, ptr};
}
}