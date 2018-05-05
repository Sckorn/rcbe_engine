#ifndef RCBE_ENGINE_WINDOW_H
#define RCBE_ENGINE_WINDOW_H

#include <string>
#include <memory>

#include "data_types/gui_types.h"

#include <gtkmm/window.h>

namespace rcbe::toolkit::gui
{

enum class WindowType
{
  REGULAR = 0,
  POPUP = 1
};

class Window : public Gtk::Window
{
public:
  Window()
  :
  Window({0, 0}, {200, 200}, "Window")
  {};
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
    WindowType type = WindowType::REGULAR
  )
  :
  Window({x, y}, {width, height}, caption, type)
  {};

  //TODO: pass cpation and points as value with move (or rvalue ref maybe)
  Window(
    const data_types::gui::Point2 &coords,
    const data_types::gui::Point2 &dimensions,
    const std::string &caption,
    WindowType type = WindowType::REGULAR
  )
  :
  coordinates(coords)
  , dimensions(dimensions)
  , caption(caption)
  , type(type)
  {
    initWindow();
  };

  //void show();

private:

  void initWindow();

  WindowType type;

  data_types::gui::Point2 coordinates;
  data_types::gui::Point2 dimensions;
  std::string caption;
};
}

#endif //RCBE_ENGINE_WINDOW_H
