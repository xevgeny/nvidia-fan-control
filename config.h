#ifndef CONFIG_H
#define CONFIG_H

// Temperature limits (°C)
#define MIN_TEMP 0
#define MAX_TEMP 100

// Fan speed limits (%)
#define MIN_FAN_SPEED 0
#define MAX_FAN_SPEED 100

// Update interval limits (seconds)
#define MIN_INTERVAL 1
#define MAX_INTERVAL 10 

/**
 * @struct app_config_t
 * @brief Configuration structure for the NVIDIA fan control application
 *
 * This structure holds all the configuration parameters needed to control
 * the GPU fan, including device identification, update frequency, and
 * the fan speed curve data.
 */
typedef struct
{
    /** @brief GPU device index (0-based) as reported by nvidia-smi */
    unsigned int device_id;       

    /** @brief Update interval in seconds (1-60) for fan speed adjustments */
    unsigned int interval;        

    /** @brief Array of [temperature, fan_speed] pairs defining the fan curve
     *  Each pair contains:
     *  - Temperature (0-100°C)
     *  - Fan speed (0-100%)
     */
    unsigned int (*fan_curve)[2]; 

    /** @brief Number of points in the fan curve array */
    unsigned int fan_curve_size; 
} app_config_t;

app_config_t *config_load(const char *config_file);
void config_show(app_config_t *config);
void config_free(app_config_t *config);

#endif // CONFIG_H