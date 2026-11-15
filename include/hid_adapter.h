#ifndef PS4_HID_ADAPTER_H
#define PS4_HID_ADAPTER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "mapper.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Convert standard USB HID Boot Keyboard (8-byte) and Boot Mouse
 * (3-byte) reports into the platform-neutral mapper input structure.
 * This module has no PS4 SDK dependency and does not read USB itself. */
bool mapper_input_from_hid(const uint8_t *keyboard_report,
                           size_t keyboard_length,
                           const uint8_t *mouse_report,
                           size_t mouse_length,
                           MapperInput *out);

#ifdef __cplusplus
}
#endif

#endif
