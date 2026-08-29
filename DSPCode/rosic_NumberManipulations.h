#ifndef ROSIC_NUMBERMANIPULATIONS_H
#define ROSIC_NUMBERMANIPULATIONS_H

#include "GlobalDefinitions.h"
#include <cmath>
#include <climits>
#include <algorithm>

namespace rosic
{
  INLINE int roundToInt(double x)
  {
    return static_cast<int>(std::round(x));
  }

  INLINE int truncateToInt(double x)
  {
    return static_cast<int>(x);
  }

  INLINE int floorInt(double x)
  {
    return static_cast<int>(std::floor(x));
  }

  INLINE int ceilInt(double x)
  {
    return static_cast<int>(std::ceil(x));
  }

  INLINE bool isEven(int x)
  {
    return (x & 1) == 0;
  }

  INLINE bool isOdd(int x)
  {
    return (x & 1) != 0;
  }

  INLINE double clip(double x, double min, double max)
  {
    return std::clamp(x, min, max);
  }

} // end namespace rosic

#endif // ROSIC_NUMBERMANIPULATIONS_H
