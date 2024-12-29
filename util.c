#include "util.h"
#include <stdlib.h>

unsigned int calculate_fan_speed(unsigned int temp, const app_config_t *config)
{
  const unsigned int(*fan_curve)[2] = config->fan_curve;
  const size_t curve_size = config->fan_curve_size;
  if (temp < fan_curve[0][0])
    return 0;
  if (temp > fan_curve[curve_size - 1][0])
    return 100;
  for (size_t i = 0; i < curve_size - 1; i++)
  {
    if (temp >= fan_curve[i][0] && temp <= fan_curve[i + 1][0])
    {
      // Linear interpolation: y = y1 + (x - x1) * (y2 - y1) / (x2 - x1)
      float t = (float)(temp - fan_curve[i][0]) / (float)(fan_curve[i + 1][0] - fan_curve[i][0]);
      float speed = fan_curve[i][1] + t * (fan_curve[i + 1][1] - fan_curve[i][1]);
      return (unsigned int)(speed + 0.5f); // round to the nearest integer
    }
  }
  return fan_curve[0][1]; // this should never happen
}