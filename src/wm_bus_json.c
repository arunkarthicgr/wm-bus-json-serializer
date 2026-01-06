#include "wm_bus_json.h"
#include <stdio.h>
#include <string.h>

/* Helper macro to safely append to buffer */
#define APPEND(fmt, ...)                                      \
    do {                                                      \
        int n = snprintf(ptr, remaining, fmt, __VA_ARGS__);  \
        if (n < 0 || (size_t)n >= remaining)                  \
            return WM_JSON_ERR_BUFFER_TOO_SMALL;             \
        ptr += n;                                             \
        remaining -= n;                                      \
    } while (0)

wm_json_status_t wm_serialize_to_json(
    const wm_gateway_data_t *input,
    char *output,
    size_t output_size,
    size_t *written_size
) {
    if (!input || !output || output_size == 0) {
        return WM_JSON_ERR_INVALID_INPUT;
    }

    char *ptr = output;
    size_t remaining = output_size;

    /* Outer array and gateway object */
    APPEND("[{\"gatewayId\":\"%s\",\"date\":\"%s\",\"deviceType\":\"%s\",",
           input->gateway_id,
           input->date,
           input->device_type);

    APPEND("\"interval_minutes\":%u,\"total_readings\":%u,",
           input->interval_minutes,
           input->total_readings);

    /* Values object */
    APPEND("\"values\":{\"device_count\":%u,\"readings\":[",
           input->device_count);

    /* Device loop */
    for (uint8_t i = 0; i < input->device_count; i++) {
        wm_device_reading_t *dev = &input->devices[i];

        APPEND("{\"media\":\"%s\",\"meter\":\"%s\",\"deviceId\":\"%s\",\"unit\":\"%s\",\"data\":[",
               dev->media,
               dev->meter,
               dev->device_id,
               dev->unit);

        /* Data points */
        for (uint8_t j = 0; j < dev->data_count; j++) {
            wm_data_point_t *dp = &dev->data[j];

            APPEND("{\"timestamp\":\"%s\",\"meter_datetime\":\"%s\","
                   "\"total_m3\":%.3f,\"status\":\"%s\"}",
                   dp->timestamp,
                   dp->meter_datetime,

