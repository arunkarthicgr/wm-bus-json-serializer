#include "wm_bus_json.h"
#include <stdio.h>
#include <string.h>

/* Append plain string */
#define APPEND_STR(str)                                      \
    do {                                                     \
        int n = snprintf(ptr, remaining, "%s", str);        \
        if (n < 0 || (size_t)n >= remaining)                \
            return WM_JSON_ERR_BUFFER_TOO_SMALL;            \
        ptr += n;                                            \
        remaining -= n;                                     \
    } while (0)

/* Append formatted string */
#define APPEND_FMT(fmt, ...)                                 \
    do {                                                     \
        int n = snprintf(ptr, remaining, fmt, __VA_ARGS__); \
        if (n < 0 || (size_t)n >= remaining)                \
            return WM_JSON_ERR_BUFFER_TOO_SMALL;            \
        ptr += n;                                            \
        remaining -= n;                                     \
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

    /* Start JSON array and gateway object */
    APPEND_FMT(
        "[{\"gatewayId\":\"%s\",\"date\":\"%s\",\"deviceType\":\"%s\",",
        input->gateway_id,
        input->date,
        input->device_type
    );

    APPEND_FMT(
        "\"interval_minutes\":%u,\"total_readings\":%u,",
        input->interval_minutes,
        input->total_readings
    );

    /* Values object */
    APPEND_FMT(
        "\"values\":{\"device_count\":%u,\"readings\":[",
        input->device_count
    );

    /* Devices loop */
    for (uint8_t i = 0; i < input->device_count; i++) {
        const wm_device_reading_t *dev = &input->devices[i];

        APPEND_FMT(
            "{\"media\":\"%s\",\"meter\":\"%s\",\"deviceId\":\"%s\",\"unit\":\"%s\",\"data\":[",
            dev->media,
            dev->meter,
            dev->device_id,
            dev->unit
        );

        /* Data points loop */
        for (uint8_t j = 0; j < dev->data_count; j++) {
            const wm_data_point_t *dp = &dev->data[j];

            APPEND_FMT(
                "{\"timestamp\":\"%s\",\"meter_datetime\":\"%s\","
                "\"total_m3\":%.3f,\"status\":\"%s\"}",
                dp->timestamp,
                dp->meter_datetime,
                dp->total_value,
                dp->status
            );

            if (j + 1 < dev->data_count) {
                APPEND_STR(",");
            }
        }

        APPEND_STR("]}");

        if (i + 1 < input->device_count) {
            APPEND_STR(",");
        }
    }

    /* Close JSON structure */
    APPEND_STR("]}}]");

    if (written_size) {
        *written_size = output_size - remaining;
    }

    return WM_JSON_OK;
}
