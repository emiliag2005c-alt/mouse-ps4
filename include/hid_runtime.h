#ifndef PS4_HID_RUNTIME_H
#define PS4_HID_RUNTIME_H

#include <stdbool.h>
#include <stdint.h>
#include "mapper.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Stateful bridge for independently polled Boot Keyboard and Boot Mouse
 * interfaces. USB transport and PS4 controller injection stay platform-specific. */
typedef struct MapperHidRuntime {
    uint8_t keyboard_report[8];
    uint8_t mouse_report[3];
    int32_t pending_dx;
    int32_t pending_dy;
    bool keyboard_valid;
    bool mouse_valid;
} MapperHidRuntime;

void mapper_hid_runtime_reset(MapperHidRuntime *runtime);
void mapper_hid_runtime_keyboard_disconnected(MapperHidRuntime *runtime);
void mapper_hid_runtime_mouse_disconnected(MapperHidRuntime *runtime);
bool mapper_hid_runtime_keyboard(MapperHidRuntime *runtime,
                                 const uint8_t *report, uint32_t length);
bool mapper_hid_runtime_mouse(MapperHidRuntime *runtime,
                              const uint8_t *report, uint32_t length);
/* Builds a snapshot of held inputs. Relative mouse motion is consumed once. */
bool mapper_hid_runtime_sample(MapperHidRuntime *runtime, MapperInput *out);

#ifdef __cplusplus
}
#endif

#endif
