#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "config.h"
#include "util.h"

void print_curve(unsigned int curve[][2], unsigned int size, unsigned int step)
{
  for (unsigned int i = 0; i < size; i += step)
    printf("Temperature: %3d°C, Fan Speed: %3d%%\n", curve[i][0], curve[i][1]);
}

void test_basic_curve()
{
  app_config_t config;
  unsigned int curve[][2] = {{30, 30}, {50, 60}, {70, 90}};
  config.fan_curve = curve;
  config.fan_curve_size = sizeof(curve) / sizeof(curve[0]);

  // Test exact points
  assert(calculate_fan_speed(30, &config) == 30);
  assert(calculate_fan_speed(50, &config) == 60);
  assert(calculate_fan_speed(70, &config) == 90);

  // Test interpolation
  assert(calculate_fan_speed(40, &config) == 45);
  assert(calculate_fan_speed(60, &config) == 75);

  // Test boundaries
  assert(calculate_fan_speed(20, &config) == 0);
  assert(calculate_fan_speed(80, &config) == 100);

  printf("✓ Basic curve tests passed\n");
}

int main()
{
  test_basic_curve();
  printf("✓ All tests passed!\n");
  return EXIT_SUCCESS;
}