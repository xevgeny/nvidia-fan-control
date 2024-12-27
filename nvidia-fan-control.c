#include "config.h"
#include "util.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <nvml.h>
#include <assert.h>
#include <libconfig.h>

#define DEBUG 0

volatile sig_atomic_t is_running = 1;

void sig_handler(int signal)
{
  is_running = 0;
}

void reset_fan_policy(nvmlDevice_t device, unsigned int num_fans)
{
  nvmlReturn_t res;
  for (unsigned int i = 0; i < num_fans; i++)
  {
    res = nvmlDeviceSetFanControlPolicy(device, i, NVML_FAN_POLICY_TEMPERATURE_CONTINOUS_SW);
    if (res != NVML_SUCCESS)
      fprintf(stderr, "Error: failed to reset fan policy to NVML_FAN_POLICY_TEMPERATURE_CONTINOUS_SW. %s\n", nvmlErrorString(res));
  }
  printf("Fan control policy reset to NVML_FAN_POLICY_TEMPERATURE_CONTINOUS_SW.\n");
}

void fan_control_start(app_config_t *config)
{
  nvmlReturn_t res;
  nvmlDevice_t device;
  unsigned int num_fans, temp, fan_speed;

  res = nvmlInit();
  if (res != NVML_SUCCESS)
  {
    fprintf(stderr, "Error: failed to initialize NVML. %s\n", nvmlErrorString(res));
    return;
  }

  res = nvmlDeviceGetHandleByIndex(config->device_id, &device);
  if (res != NVML_SUCCESS)
  {
    fprintf(stderr, "Error: failed to get device handle. %s\n", nvmlErrorString(res));
    nvmlShutdown();
    return;
  }

  res = nvmlDeviceGetNumFans(device, &num_fans);
  if (res != NVML_SUCCESS)
  {
    fprintf(stderr, "Error: failed to get number of fans. %s\n", nvmlErrorString(res));
    nvmlShutdown();
    return;
  }

  if (DEBUG)
  {
    nvmlFanControlPolicy_t policy;
    res = nvmlDeviceGetFanControlPolicy_v2(device, 0, &policy);
    if (res != NVML_SUCCESS)
    {
      fprintf(stderr, "Error: failed to get fan control policy. %s\n", nvmlErrorString(res));
      nvmlShutdown();
      return;
    }
    printf("Current fan control policy: %s\n", policy == NVML_FAN_POLICY_TEMPERATURE_CONTINOUS_SW ? "NVML_FAN_POLICY_TEMPERATURE_CONTINOUS_SW" : "NVML_FAN_POLICY_MANUAL");
  }

  // Set fan control policy to manual
  for (unsigned int i = 0; i < num_fans; i++)
  {
    res = nvmlDeviceSetFanControlPolicy(device, i, NVML_FAN_POLICY_MANUAL);
    if (res != NVML_SUCCESS)
    {
      fprintf(stderr, "Error: failed to set fan control policy to NVML_FAN_POLICY_MANUAL. %s\n", nvmlErrorString(res));
      nvmlShutdown();
      return;
    }
  }
  printf("Fan control policy set to NVML_FAN_POLICY_MANUAL. Running...\n");

  while (is_running)
  {
    res = nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temp);
    if (res != NVML_SUCCESS)
    {
      fprintf(stderr, "Error: failed to get temperature. %s\n", nvmlErrorString(res));
      break;
    }

    fan_speed = calculate_fan_speed(temp, config);

    for (unsigned int i = 0; i < num_fans; i++)
    {
      res = nvmlDeviceSetFanSpeed_v2(device, i, fan_speed);
      if (res != NVML_SUCCESS)
      {
        fprintf(stderr, "Error: failed to set fan speed. %s\n", nvmlErrorString(res));
        break;
      }
    }

    if (DEBUG)
      printf("Temperature: %3d°C, Fan Speed: %3d%%\n", temp, fan_speed);

    sleep(config->interval);
  }

  reset_fan_policy(device, num_fans);
  nvmlShutdown();
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Provide a config file. Usage: %s example.cfg\n", argv[0]);
    return EXIT_FAILURE;
  }

  // Load configuration file
  app_config_t *config = config_load(argv[1]);
  if (!config)
    return EXIT_FAILURE;
  config_show(config);

  // Register signal handlers
  signal(SIGINT, sig_handler);  // Ctrl+C
  signal(SIGTERM, sig_handler); // Termination request
  signal(SIGABRT, sig_handler); // Abort

  // Start fan control
  fan_control_start(config);
  config_free(config);
  return EXIT_SUCCESS;
}