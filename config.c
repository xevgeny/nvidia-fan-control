#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <libconfig.h>

app_config_t *config_load(const char *config_file)
{
  config_t cfg;
  config_init(&cfg);

  if (!config_read_file(&cfg, config_file))
  {
    fprintf(stderr, "Error: Failed to read config file %s. %s\n", config_file, config_error_text(&cfg));
    config_destroy(&cfg);
    return NULL;
  }

  // Read device ID
  int device_id;
  if (!config_lookup_int(&cfg, "device_id", &device_id))
  {
    fprintf(stderr, "Error: device_id is required\n");
    config_destroy(&cfg);
    return NULL;
  }
  if (device_id < 0)
  {
    fprintf(stderr, "Error: device_id must be >= 0\n");
    config_destroy(&cfg);
    return NULL;
  }

  // Read update interval
  int interval;
  if (!config_lookup_int(&cfg, "interval", &interval))
  {
    fprintf(stderr, "Error: interval is required\n");
    config_destroy(&cfg);
    return NULL;
  }
  if (interval < MIN_INTERVAL || interval > MAX_INTERVAL)
  {
    fprintf(stderr, "Error: interval must be between %d and %d seconds\n", MIN_INTERVAL, MAX_INTERVAL);
    config_destroy(&cfg);
    return NULL;
  }

  // Read fan curve setting
  config_setting_t *fan_curve_setting = config_lookup(&cfg, "fan_curve");
  if (fan_curve_setting == NULL)
  {
    fprintf(stderr, "Error: fan_curve is required\n");
    config_destroy(&cfg);
    return NULL;
  }

  size_t fan_curve_size = config_setting_length(fan_curve_setting);
  if (fan_curve_size < 2)
  {
    fprintf(stderr, "Error: fan_curve must have at least 2 points\n");
    config_destroy(&cfg);
    return NULL;
  }

  unsigned int(*fan_curve)[2] = malloc(fan_curve_size * sizeof(*fan_curve));
  if (!fan_curve)
  {
    fprintf(stderr, "Error: failed to allocate memory for fan curve\n");
    config_destroy(&cfg);
    return NULL;
  }

  int prev_temp = 0;
  int prev_speed = 0;

  // Validate fan curve
  for (size_t i = 0; i < fan_curve_size; i++)
  {
    config_setting_t *pair = config_setting_get_elem(fan_curve_setting, i);
    if (pair)
    {
      if (config_setting_length(pair) != 2)
      {
        fprintf(stderr, "Error: fan curve point must have exactly 2 values (temperature and fan speed)\n");
        config_destroy(&cfg);
        free(fan_curve);
        return NULL;
      }
      int temp = config_setting_get_int_elem(pair, 0);
      int speed = config_setting_get_int_elem(pair, 1);
      if (temp < MIN_TEMP || temp > MAX_TEMP || speed < MIN_FAN_SPEED || speed > MAX_FAN_SPEED)
      {
        fprintf(stderr,
                "Error: temperature must be between %d and %d°C, fan speed must be between %d and %d%%\n",
                MIN_TEMP, MAX_TEMP, MIN_FAN_SPEED, MAX_FAN_SPEED);
        config_destroy(&cfg);
        free(fan_curve);
        return NULL;
      }
      if (temp < prev_temp)
      {
        fprintf(stderr, "Error: temperature must be in ascending order\n");
        config_destroy(&cfg);
        free(fan_curve);
        return NULL;
      }
      if (speed < prev_speed)
      {
        fprintf(stderr, "Error: fan speed must be in ascending order\n");
        config_destroy(&cfg);
        free(fan_curve);
        return NULL;
      }
      fan_curve[i][0] = (unsigned int)temp;
      fan_curve[i][1] = (unsigned int)speed;
      prev_temp = temp;
      prev_speed = speed;
    }
  }

  // Initialize config
  app_config_t *config = malloc(sizeof(app_config_t));
  if (!config)
  {
    fprintf(stderr, "Error: failed to allocate memory for config\n");
    config_destroy(&cfg);
    return NULL;
  }

  config->device_id = (unsigned int)device_id;
  config->interval = (unsigned int)interval;
  config->fan_curve = fan_curve;
  config->fan_curve_size = fan_curve_size;

  config_destroy(&cfg);
  return config;
}

void config_show(app_config_t *config)
{
  printf("-- Device ID: %3d\n", config->device_id);
  printf("-- Interval : %3d\n", config->interval);
  for (size_t i = 0; i < config->fan_curve_size; i++)
    printf("-- Fan curve point: %3d°C - %3d%%\n", config->fan_curve[i][0], config->fan_curve[i][1]);
}

void config_free(app_config_t *config)
{
  free(config->fan_curve);
  free(config);
}
