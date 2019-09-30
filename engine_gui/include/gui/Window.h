#ifndef RCBE_ENGINE_WINDOW_H
#define RCBE_ENGINE_WINDOW_H

#include <string>
#include <memory>

#include <data_types/gui_types.hpp>

#include <gtkmm/window.h>
#include <gtkmm/builder.h>

namespace rcbe::toolkit::gui
{

class Window
{
public:

  static Window
  makeWindowFromFile(const std::string &path, const std::string &widget_id);

  Window(
    std::unique_ptr<Gtk::Window> &&gtk_window,
    Glib::RefPtr<Gtk::Builder> gtk_builder
  )
  :
  gtk_window(std::move(gtk_window))
  , gtk_builder(std::move(gtk_builder))
  {}

  Gtk::Window &operator*();

  //void show(Gtk::Main &kit);

private:

  std::unique_ptr<Gtk::Window> gtk_window;
  Glib::RefPtr<Gtk::Builder> gtk_builder;
};
}

#endif //RCBE_ENGINE_WINDOW_H
