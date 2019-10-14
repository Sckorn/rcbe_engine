#ifndef RCBE_POINT_HPP
#define RCBE_POINT_HPP

namespace rcbe::gui
{
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

#endif