#ifndef RCBE_ENGINE_WINDOW_H
#define RCBE_ENGINE_WINDOW_H

#include <string>

#include "data_types/gui_types.h"

#include <gtk-3.0/gtk/gtk.h>

namespace rcbe::toolkit::gui
{
class Window
{
public:
  Window()
    :
    coordinates(data_types::gui::Point2{0, 0})
    , caption("Window")
  {};
  /**
   * since window caption is only required in window class
   * passing it by value with std::move seems like an option
   * */
  Window(std::size_t x, std::size_t y, const std::string &caption)
    :
    coordinates(data_types::gui::Point2{x, y})
    , caption(caption)
  {};

  Window(const data_types::gui::Point2 &coords, const std::string &caption)
    :
    coordinates(coords)
    , caption(caption)
  {};

  void show();
private:
  data_types::gui::Point2 coordinates;
  std::string caption;
};
}

#endif //RCBE_ENGINE_WINDOW_H
