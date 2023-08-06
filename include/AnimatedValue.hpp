#pragma once

#include <functional>
#include <string>

struct AnimatedValue
{
  double startValue, endValue;
  std::function<void(double value)> onValueChanged;
  std::function<double(double start, double end, double tNorm)> easingFunction = Linear;

  static double Linear(double start, double end, double tNorm)
  {
    return start + (end - start) * tNorm;
  }

  static double EaseInOutQuad(double start, double end, double tNorm)
  {
    if (tNorm < 0.5) return start + (end - start) * 2 * tNorm * tNorm;
    else return start + (end - start) * (1 - 2 * (1 - tNorm) * (1 - tNorm));
  }
};