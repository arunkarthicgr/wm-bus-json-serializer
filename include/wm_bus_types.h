#ifndef WM_BUS_TYPES_H
#define WM_BUS_TYPES_H

#include <stdint.h>

/* Configuration limits */
#define MAX_DEVICES     4
#define MAX_DATA_POINTS 4
#define MAX_STR_LEN     32

/* Single meter data point */
typedef struct {
    char timestamp[20];        /* YYYY-MM-DD HH:MM */
    char meter_datetime[20];   /* YYYY-MM-DD HH:MM */
    float total_value;         /* Meter total value */
    char status[8];            /* OK, ERROR */
} wm_data_point_t;

/* One device reading */
typedef struct {
    char media[MAX_STR_LEN];      /* water, electricity */
    char meter[MAX_STR_LEN];      /* meter model */
    char device_id[MAX_STR_LEN];  /* device identifier */
    char unit[8];                 /* m3, kWh */

    uint8_t data_count;
    wm_data_point_t data[MAX_DATA_POINTS];
} wm_device_reading_t;

/* Gateway-level data */
typedef struct {
    char gateway_id[MAX_STR_LEN];
    char date[11];               /* YYYY-MM-DD */
    char device_type[MAX_STR_LEN];

    uint16_t interval_minutes;
    uint16_t total_readings;

    uint8_t device_count;
    wm_device_reading_t devices[MAX_DEVICES];
} wm_gateway_data_t;

#endif /* WM_BUS_TYPES_H */
