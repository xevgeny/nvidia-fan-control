#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <libconfig.h>

app_config_t *config_load(const char *config_file)
{
  app_config_t *config = malloc(sizeof(app_config_t));
  if (!config)
  {
    fprintf(stderr, "Error: failed to allocate memory for config\n");
    return NULL;
  }

  config_t cfg;
  config_init(&cfg);

  if (!config_read_file(&cfg, config_file))
  {
    fprintf(stderr, "Error: Failed to read config file %s. %s\n", config_file, config_error_text(&cfg));
    config_destroy(&cfg);
    config_free(config);
    return NULL;
  }

  // Load device ID
  int device_id;
  if (config_lookup_int(&cfg, "device_id", &device_id))
  {
    if (device_id < 0)
    {
      fprintf(stderr, "Error: device_id must be >= 0\n");
      config_destroy(&cfg);
      config_free(config);
      return NULL;
    }
    config->device_id = (unsigned int)device_id;
  }
  else
  {
    fprintf(stderr, "Error: device_id is required\n");
    config_destroy(&cfg);
    config_free(config);
    return NULL;
  }

  // Load update interval
  int interval;
  if (config_lookup_int(&cfg, "interval", &interval))
  {
    if (interval < 1 || interval > 60)
    {
      fprintf(stderr, "Error: interval must be between 1 and 60 seconds\n");
      config_destroy(&cfg);
      config_free(config);
      return NULL;
    }
    config->interval = (unsigned int)interval;
  }
  else
  {
    fprintf(stderr, "Error: interval is required\n");
    config_destroy(&cfg);
    config_free(config);
    return NULL;
  }

  // Load fan curve
  config_setting_t *fan_curve_setting = config_lookup(&cfg, "fan_curve");
  if (fan_curve_setting == NULL)
  {
    fprintf(stderr, "Error: fan_curve is required\n");
    config_destroy(&cfg);
    config_free(config);
    return NULL;
  }
  size_t fan_curve_size = config_setting_length(fan_curve_setting);
  if (fan_curve_size < 2)
  {
    fprintf(stderr, "Error: fan_curve must have at least 2 points\n");
    config_destroy(&cfg);
    config_free(config);
    return NULL;
  }

  unsigned int(*fan_curve)[2] = malloc(fan_curve_size * sizeof(*fan_curve));
  if (!fan_curve)
  {
    fprintf(stderr, "Error: failed to allocate memory for fan curve\n");
    config_destroy(&cfg);
    config_free(config);
    return NULL;
  }

  config->fan_curve = fan_curve;
  config->fan_curve_size = fan_curve_size;

  for (size_t i = 0; i < fan_curve_size; i++)
  {
    config_setting_t *pair = config_setting_get_elem(fan_curve_setting, i);
    if (pair)
    {
      if (config_setting_length(pair) != 2)
      {
        fprintf(stderr, "Error: fan curve point must have exactly 2 values (temperature and fan speed)\n");
        config_destroy(&cfg);
        config_free(config);
        return NULL;
      }
      int temp = config_setting_get_int_elem(pair, 0);
      int speed = config_setting_get_int_elem(pair, 1);
      if (temp < 0 || temp > 100 || speed < 0 || speed > 100)
      {
        fprintf(stderr, "Error: temperature and fan speed values must be between 0 and 100\n");
        config_destroy(&cfg);
        config_free(config);
        return NULL;
      }
      config->fan_curve[i][0] = (unsigned int)temp;
      config->fan_curve[i][1] = (unsigned int)speed;
    }
  }

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
  if (config)
  {
    free(config->fan_curve);
    free(config);
  }
}
