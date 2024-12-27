#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <nvml.h>
#include <assert.h>
#include <libconfig.h>
#include "config.h"
#include "util.h"

volatile sig_atomic_t is_running = 1;

void sig_handler(int signal)
{
    is_running = 0;
}

void fan_control_start()
{
    while (is_running)
    {
        printf(".");
        sleep(1);
    }
    printf("\nShutting down gracefully...\n");
}

void fan_control_stop()
{
    is_running = 0;
}

int main()
{
    // Load configuration file
    app_config_t *config = config_load("example.cfg");
    if (!config)
        return EXIT_FAILURE;
    config_show(config);
    if (config_validate(config) != SUCCESS)
        return EXIT_FAILURE;

    // Register signal handlers
    signal(SIGINT, sig_handler);  // Ctrl+C
    signal(SIGTERM, sig_handler); // Termination request
    signal(SIGABRT, sig_handler); // Abort

    // Start fan control
    fan_control_start();
    config_free(config);
    return EXIT_SUCCESS;
}