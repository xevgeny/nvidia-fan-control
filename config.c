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
    fprintf(stderr, "Error reading config file %s: %s\n", config_file, config_error_text(&cfg));
    config_destroy(&cfg);
    return NULL;
  }

  int device_id;
  if (config_lookup_int(&cfg, "device_id", &device_id))
  {
    config->device_id = (unsigned int)device_id;
  }
  else
  {
    fprintf(stderr, "Error: device_id is required\n");
    config_destroy(&cfg);
    return NULL;
  }

  int interval;
  if (config_lookup_int(&cfg, "interval", &interval))
  {
    config->interval = (unsigned int)interval;
  }
  else
  {
    fprintf(stderr, "Error: interval is required\n");
    config_destroy(&cfg);
    return NULL;
  }

  config_setting_t *fan_curve_setting = config_lookup(&cfg, "fan_curve");
  if (fan_curve_setting != NULL)
  {
    size_t fan_curve_size = config_setting_length(fan_curve_setting);
    unsigned int(*fan_curve)[2] = realloc(config->fan_curve, fan_curve_size * sizeof(*config->fan_curve));
    if (!fan_curve)
    {
      fprintf(stderr, "Error: failed to allocate memory for fan curve\n");
      config_destroy(&cfg);
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
          free(config->fan_curve);
          free(config);
          return NULL;
        }
        config->fan_curve[i][0] = config_setting_get_int_elem(pair, 0);
        config->fan_curve[i][1] = config_setting_get_int_elem(pair, 1);
      }
    }
  }

  config_destroy(&cfg);
  return config;
}

void config_show(app_config_t *config)
{
  printf("-- Device ID: %d\n", config->device_id);
  printf("-- Interval: %d\n", config->interval);
  for (size_t i = 0; i < config->fan_curve_size; i++)
  {
    printf("-- Fan curve point: %d°C - %d%%\n",
           config->fan_curve[i][0],
           config->fan_curve[i][1]);
  }
}

int config_validate(app_config_t *config)
{
  if (config->device_id < 0)
  {
    fprintf(stderr, "Error: device_id must be >= 0\n");
    return ERROR;
  }

  if (config->interval < 1 || config->interval > 60)
  {
    fprintf(stderr, "Error: interval must be between 1 and 60\n");
    return ERROR;
  }

  for (size_t i = 0; i < config->fan_curve_size; i++)
  {
    if (config->fan_curve[i][0] < 0 || config->fan_curve[i][0] > 100)
    {
      fprintf(stderr, "Error: temperature must be between 0 and 100\n");
      return ERROR;
    }
    if (config->fan_curve[i][1] < 0 || config->fan_curve[i][1] > 100)
    {
      fprintf(stderr, "Error: fan speed must be between 0 and 100\n");
      return ERROR;
    }
  }

  return SUCCESS;
}

void config_free(app_config_t *config)
{
  if (config)
  {
    free(config->fan_curve);
    free(config);
  }
}
