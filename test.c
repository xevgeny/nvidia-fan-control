#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

unsigned int calculate_fan_speed(unsigned int temp,
                                 const unsigned int *temp_curve,
                                 const unsigned int *fan_curve,
                                 size_t curve_size);

void test_basic_curve()
{
  const unsigned int temp_curve[] = {30, 50, 70};
  const unsigned int fan_curve[] = {30, 60, 90};
  const size_t curve_size = 3;

  // Test exact points
  assert(calculate_fan_speed(30, temp_curve, fan_curve, curve_size) == 30);
  assert(calculate_fan_speed(50, temp_curve, fan_curve, curve_size) == 60);
  assert(calculate_fan_speed(70, temp_curve, fan_curve, curve_size) == 90);

  // Test interpolation
  assert(calculate_fan_speed(40, temp_curve, fan_curve, curve_size) == 45);
  assert(calculate_fan_speed(60, temp_curve, fan_curve, curve_size) == 75);

  // Test boundaries
  assert(calculate_fan_speed(20, temp_curve, fan_curve, curve_size) == 30);
  assert(calculate_fan_speed(80, temp_curve, fan_curve, curve_size) == 100);

  printf("✓ Basic curve tests passed\n");
}

int main()
{
  test_basic_curve();
  printf("✓ All tests passed!\n");
  return EXIT_SUCCESS;
}