
#include <NGP_GamePad.h>
#include <NGP_Types.h>

DECLSPEC double NGPCALL clamp(double v, double low, double high) {
  return v < low ? low : v > high ? high : v;
}

DECLSPEC double NGPCALL normalize_axis_double(double d) {
  return (d >= 0 ? d / NGP_THUMBSTICK_AXIS_MAX : -(d / NGP_THUMBSTICK_AXIS_MIN));
}

DECLSPEC NGP_Vector2 NGPCALL NormalizeAxis(NGP_Vector2 v) {
  double x = normalize_axis_double(v.X);
  double y = normalize_axis_double(v.Y);
  NGP_Vector2 v2 = {clamp(x, -1.0, 1.0), clamp(y, -1.0, 1.0)};
  return v2;
}

const NGP_Color NGP_Red = {255, 0, 0};
const NGP_Color NGP_Green = {0, 255, 0};
const NGP_Color NGP_Blue = {0, 0, 255};
const NGP_Color NGP_Purple = {150, 100, 255};