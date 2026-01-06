#ifndef WM_BUS_JSON_H
#define WM_BUS_JSON_H

#include <stddef.h>
#include "wm_bus_types.h"

/* JSON serialization status codes */
typedef enum {
    WM_JSON_OK = 0,
    WM_JSON_ERR_BUFFER_TOO_SMALL,
    WM_JSON_ERR_INVALID_INPUT
} wm_json_status_t;

/**
 * @brief Serialize gateway data into JSON
 *
 * @param input         Pointer to gateway data structure
 * @param output        Caller-provided output buffer
 * @param output_size   Size of output buffer in bytes
 * @param written_size  Number of bytes written to output buffer
 *
 * @return wm_json_status_t
 */
wm_json_status_t wm_serialize_to_json(
    const wm_gateway_data_t *input,
    char *output,
    size_t output_size,
    size_t *written_size
);

#endif /* WM_BUS_JSON_H */
