#include "gui/window_utils.h"

#include <gtkmm/builder.h>

namespace rcbe::toolkit::gui
{
std::shared_ptr<Gtk::Window> makeWindowFromFile(const std::string &path)
{
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(path);

  Gtk::Window *pd = nullptr;

  builder->get_widget("main_window", pd);
  std::shared_ptr<Gtk::Window> ptr = nullptr;
  ptr.reset(pd);
  return ptr;
}
}