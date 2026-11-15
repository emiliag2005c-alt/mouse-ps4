#include "hid_runtime.h"

#include <limits.h>
#include <string.h>
#include "hid_adapter.h"

static int32_t clamp_add(int32_t current, int8_t delta)
{
    int64_t value = (int64_t)current + delta;
    if (value > INT16_MAX) return INT16_MAX;
    if (value < INT16_MIN) return INT16_MIN;
    return (int32_t)value;
}

void mapper_hid_runtime_reset(MapperHidRuntime *runtime)
{
    if (runtime != NULL) memset(runtime, 0, sizeof(*runtime));
}

bool mapper_hid_runtime_keyboard(MapperHidRuntime *runtime,
                                 const uint8_t *report, uint32_t length)
{
    if (runtime == NULL || report == NULL || length != 8) return false;
    memcpy(runtime->keyboard_report, report, 8);
    runtime->keyboard_valid = true;
    return true;
}

bool mapper_hid_runtime_mouse(MapperHidRuntime *runtime,
                              const uint8_t *report, uint32_t length)
{
    if (runtime == NULL || report == NULL || length != 3) return false;
    runtime->mouse_report[0] = report[0];
    runtime->mouse_report[1] = 0;
    runtime->mouse_report[2] = 0;
    runtime->pending_dx = clamp_add(runtime->pending_dx, (int8_t)report[1]);
    runtime->pending_dy = clamp_add(runtime->pending_dy, (int8_t)report[2]);
    runtime->mouse_valid = true;
    return true;
}

bool mapper_hid_runtime_sample(MapperHidRuntime *runtime, MapperInput *out)
{
    uint8_t mouse[3];
    if (runtime == NULL || out == NULL) return false;

    mouse[0] = runtime->mouse_valid ? runtime->mouse_report[0] : 0;
    mouse[1] = 0;
    mouse[2] = 0;

    if (!mapper_input_from_hid(runtime->keyboard_valid ? runtime->keyboard_report : NULL,
                               runtime->keyboard_valid ? 8u : 0u,
                               runtime->mouse_valid ? mouse : NULL,
                               runtime->mouse_valid ? 3u : 0u,
                               out)) {
        return false;
    }

    out->mouse_dx = (int16_t)runtime->pending_dx;
    out->mouse_dy = (int16_t)runtime->pending_dy;
    runtime->pending_dx = 0;
    runtime->pending_dy = 0;
    return true;
}
