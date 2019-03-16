#ifndef RCBE_ENGINE_GUI_TYPES_H
#define RCBE_ENGINE_GUI_TYPES_H

#include <cstring>

namespace rcbe::data_types::gui
{
/**
 * Simple 2d point for windows coordinate
 *
 * */
struct Point2
{
  std::size_t x;
  std::size_t y;
};
}

#endif //RCBE_ENGINE_GUI_TYPES_H
