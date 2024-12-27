#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

unsigned int calculate_fan_speed(unsigned int temp,
                                 const unsigned int *temp_curve,
                                 const unsigned int *fan_curve,
                                 size_t curve_size);

#endif // UTIL_H