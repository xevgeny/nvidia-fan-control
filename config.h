#ifndef CONFIG_H
#define CONFIG_H

#define SUCCESS 0
#define ERROR -1

typedef struct
{
  unsigned int device_id;       // GPU device id
  unsigned int interval;        // update interval in seconds
  unsigned int (*fan_curve)[2]; // 2D array for temperature and fan duty in percentage
  unsigned int fan_curve_size;  // number of points in the curve
} app_config_t;

app_config_t *config_load(const char *config_file);
void config_show(app_config_t *config);
int config_validate(app_config_t *config);
void config_free(app_config_t *config);

#endif // CONFIG_H