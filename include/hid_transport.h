#ifndef PS4_HID_TRANSPORT_H
#define PS4_HID_TRANSPORT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "hid_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

/* A platform backend reports whether a fresh HID packet arrived.
 * NO_DATA preserves the last held state; DISCONNECTED clears that device. */
typedef enum MapperHidReadStatus {
    MAPPER_HID_READ_ERROR = -1,
    MAPPER_HID_READ_NO_DATA = 0,
    MAPPER_HID_READ_REPORT = 1,
    MAPPER_HID_READ_DISCONNECTED = 2
} MapperHidReadStatus;

typedef MapperHidReadStatus (*MapperHidReadFn)(
    void *context, uint8_t *report, size_t capacity, size_t *actual_length);

typedef struct MapperHidTransport {
    void *context;
    MapperHidReadFn read_keyboard;
    MapperHidReadFn read_mouse;
} MapperHidTransport;

/* Polls both interfaces once. This is only a transport seam: it does not
 * enumerate USB or provide a PS4-specific implementation. */
bool mapper_hid_poll(MapperHidRuntime *runtime,
                     const MapperHidTransport *transport);

#ifdef __cplusplus
}
#endif

#endif
