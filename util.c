#include "util.h"

unsigned int calculate_fan_speed(unsigned int temp,
                                 const unsigned int *temp_curve,
                                 const unsigned int *fan_curve,
                                 size_t curve_size)
{
  if (temp < temp_curve[0])
    return 0;

  if (temp > temp_curve[curve_size - 1])
    return 100;

  for (size_t i = 0; i < curve_size - 1; i++)
  {
    if (temp >= temp_curve[i] && temp <= temp_curve[i + 1])
    {
      // Linear interpolation: y = y1 + (x - x1) * (y2 - y1) / (x2 - x1)
      float t = (float)(temp - temp_curve[i]) / (float)(temp_curve[i + 1] - temp_curve[i]);
      float speed = fan_curve[i] + t * (fan_curve[i + 1] - fan_curve[i]);
      return (unsigned int)(speed + 0.5f); // Round to nearest integer
    }
  }

  return fan_curve[0]; // This should never happen
}