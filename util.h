#ifndef UTIL_H
#define UTIL_H

#include "config.h"

/**
 * Calculates fan speed percentage based on temperature using linear interpolation
 * between points on a fan curve.
 *
 * @param temp    Current temperature value
 * @param config  App configuration
 *                fan_curve is an array of [temperature, speed] points
 *                fan_curve_size is the number of points in the curve
 *
 * @return Fan speed as a percentage (0-100)
 *         Returns 0% if temp is below first point
 *         Returns 100% if temp is above last point
 */
unsigned int calculate_fan_speed(unsigned int temp, const app_config_t *config);

#endif // UTIL_H
