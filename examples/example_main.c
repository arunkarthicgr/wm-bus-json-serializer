#include <stdio.h>
#include <string.h>
#include "wm_bus_json.h"

int main(void) {
    wm_gateway_data_t gateway;

    /* Clear structure */
    memset(&gateway, 0, sizeof(gateway));

    /* Gateway metadata */
    snprintf(gateway.gateway_id, sizeof(gateway.gateway_id), "gateway_1234");
    snprintf(gateway.date, sizeof(gateway.date), "1970-01-01");
    snprintf(gateway.device_type, sizeof(gateway.device_type), "stromleser");
    gateway.interval_minutes = 15;
    gateway.total_readings = 1;

    /* Device */
    gateway.device_count = 1;
    wm_device_reading_t *dev = &gateway.devices[0];

    snprintf(dev->media, sizeof(dev->media), "water");
    snprintf(dev->meter, sizeof(dev->meter), "waterstarm");
    snprintf(dev->device_id, sizeof(dev->device_id), "stromleser_50898527");
    snprintf(dev->unit, sizeof(dev->unit), "m3");

    /* Data point */
    dev->data_count = 1;
    wm_data_point_t *dp = &dev->data[0];

    snprintf(dp->timestamp, sizeof(dp->timestamp), "1970-01-01 00:00");
    snprintf(dp->meter_datetime, sizeof(dp->meter_datetime), "1970-01-01 00:00");
    dp->total_value = 107.752f;
    snprintf(dp->status, sizeof(dp->status), "OK");

    /* Output buffer */
    char json_buffer[1024];
    size_t written = 0;

    wm_json_status_t status = wm_serialize_to_json(
        &gateway,
        json_buffer,
        sizeof(json_buffer),
        &written
    );

    if (status == WM_JSON_OK) {
        printf("JSON output (%zu bytes):\n%s\n", written, json_buffer);
    } else {
        printf("JSON serialization failed (error %d)\n", status);
    }

    return 0;
}
