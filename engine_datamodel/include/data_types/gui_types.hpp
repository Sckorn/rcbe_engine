#ifndef RCBE_ENGINE_GUI_TYPES_H
#define RCBE_ENGINE_GUI_TYPES_H

#include <cstring>
#include <rcbe_engine_common/file_utils.h>

namespace rcbe::data_types::gui
{
/**
 * Simple 2d point for windows coordinate
 *
 * */
template <typename ValueType>
class Point2
{
public:
  Point2(ValueType &&x, ValueType &&y)
  :
  _x(x)
  , _y(y)
  {}
  ~Point2() = default;

  Point2(const Point2 &other) = default;
  Point2 &operator=(const Point2 &other) = default;

  Point2(ValueType &&other) = default;
  Point2 &operator=(ValueType &&) = default;

private:
  ValueType _x;
  ValueType _y;
};
}

#endif //RCBE_ENGINE_GUI_TYPES_H
