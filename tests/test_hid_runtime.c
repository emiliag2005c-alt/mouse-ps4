#include "hid_runtime.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    MapperHidRuntime runtime;
    MapperInput input;
    const uint8_t keyboard_w[8] = {0, 0, 0x1a, 0, 0, 0, 0, 0};
    const uint8_t keyboard_up[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    const uint8_t mouse_a[3] = {1, 10, (uint8_t)-3};
    const uint8_t mouse_b[3] = {1, 5, 2};
    const uint8_t mouse_release[3] = {0, 0, 0};

    mapper_hid_runtime_reset(&runtime);
    assert(mapper_hid_runtime_keyboard(&runtime, keyboard_w, 8));
    assert(mapper_hid_runtime_mouse(&runtime, mouse_a, 3));
    assert(mapper_hid_runtime_mouse(&runtime, mouse_b, 3));
    assert(mapper_hid_runtime_sample(&runtime, &input));
    assert(input.keys['w'] && input.mouse_left);
    assert(input.mouse_dx == 15 && input.mouse_dy == -1);

    /* Relative motion is consumed once, while held keys/buttons persist. */
    assert(mapper_hid_runtime_sample(&runtime, &input));
    assert(input.keys['w'] && input.mouse_left);
    assert(input.mouse_dx == 0 && input.mouse_dy == 0);

    /* Disconnecting one interface must not leave its held state stuck. */
    mapper_hid_runtime_keyboard_disconnected(&runtime);
    assert(mapper_hid_runtime_sample(&runtime, &input));
    assert(!input.keys['w'] && input.mouse_left);

    mapper_hid_runtime_keyboard(&runtime, keyboard_w, 8);
    mapper_hid_runtime_mouse_disconnected(&runtime);
    assert(mapper_hid_runtime_sample(&runtime, &input));
    assert(input.keys['w'] && !input.mouse_left);
    assert(!input.mouse_dx && !input.mouse_dy);

    assert(mapper_hid_runtime_keyboard(&runtime, keyboard_up, 8));
    assert(mapper_hid_runtime_mouse(&runtime, mouse_release, 3));
    assert(mapper_hid_runtime_sample(&runtime, &input));
    assert(!input.keys['w'] && !input.mouse_left);

    mapper_hid_runtime_reset(&runtime);
    assert(mapper_hid_runtime_sample(&runtime, &input));
    assert(!input.keys['w'] && !input.mouse_left);
    assert(!input.mouse_dx && !input.mouse_dy);

    assert(!mapper_hid_runtime_keyboard(&runtime, keyboard_w, 7));
    assert(!mapper_hid_runtime_mouse(&runtime, mouse_a, 2));
    puts("HID runtime state tests passed");
    return 0;
}
