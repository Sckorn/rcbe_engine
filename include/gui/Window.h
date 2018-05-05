#ifndef RCBE_ENGINE_WINDOW_H
#define RCBE_ENGINE_WINDOW_H

#include <string>
#include <memory>

#include "data_types/gui_types.h"

#include <gtk/gtk.h>

namespace rcbe::toolkit::gui
{

enum class WindowType
{
  REGULAR = static_cast<int>(GTK_WINDOW_TOPLEVEL),
  POPUP = static_cast<int>(GTK_WINDOW_POPUP)
};

class Window
{
public:
  Window()
  :
  coordinates({0, 0})
  , dimensions({200, 200})
  , caption("Window")
  , gtk_window_ptr(std::move(std::unique_ptr<GtkWidget>(gtk_window_new(type))))
  , type(GTK_WINDOW_TOPLEVEL)
  {
    initWindow();
  };
  /**
   * since window caption is only required in window class
   * passing it by value with std::move seems like an option
   * */
  Window(
    std::size_t x,
    std::size_t y,
    std::size_t width,
    std::size_t height,
    const std::string &caption,
    GtkWindowType type = GTK_WINDOW_TOPLEVEL
  )
  :
  coordinates({x, y})
  , dimensions({width, height})
  , caption(caption)
  , gtk_window_ptr(std::move(std::unique_ptr<GtkWidget>(gtk_window_new(type))))
  , type(type)
  {
    initWindow();
  };

  //TODO: pass cpation and points as value with move (or rvalue ref maybe)
  Window(
    const data_types::gui::Point2 &coords,
    const data_types::gui::Point2 &dimensions,
    const std::string &caption,
    GtkWindowType type = GTK_WINDOW_TOPLEVEL
  )
  :
  coordinates(coords)
  , dimensions(dimensions)
  , caption(caption)
  , gtk_window_ptr(std::move(std::unique_ptr<GtkWidget>(gtk_window_new(type))))
  , type(type)
  {
    initWindow();
  };

  void show();

private:

  void initWindow();

  GtkWindowType type;

  std::unique_ptr<GtkWidget> gtk_window_ptr;

  data_types::gui::Point2 coordinates;
  data_types::gui::Point2 dimensions;
  std::string caption;
};
}

#endif //RCBE_ENGINE_WINDOW_H
