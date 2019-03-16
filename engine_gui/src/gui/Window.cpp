#include "gui/Window.h"

namespace rcbe::toolkit::gui
{

Window
Window::makeWindowFromFile(const std::string &path, const std::string &widget_id)
{
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(path);

  Gtk::Window *pd = nullptr;

  builder->get_widget(widget_id, pd);
  std::unique_ptr<Gtk::Window> ptr = nullptr;
  ptr.reset(pd);

  return Window(
    std::move(ptr),
    std::move(builder)
  );
}

Gtk::Window &Window::operator*()
{
  return *gtk_window;
}

/*void Window::show()
{
  g_signal_connect(gtk_window_ptr.get(), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(gtk_window_ptr.get());
}*/
}