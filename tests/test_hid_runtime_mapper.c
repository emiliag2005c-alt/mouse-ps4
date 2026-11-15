#include "hid_runtime.h"
#include "mapper.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    MapperHidRuntime runtime;
    MapperInput input;
    MapperConfig config;
    MapperState state;
    MapperReport output;
    const uint8_t keyboard[8] = {0, 0, 0x1a, 0, 0, 0, 0, 0};
    const uint8_t mouse[3] = {1, 0, 0};

    mapper_hid_runtime_reset(&runtime);
    mapper_default_config(&config);
    mapper_reset(&state);

    assert(mapper_hid_runtime_keyboard(&runtime, keyboard, 8));
    assert(mapper_hid_runtime_mouse(&runtime, mouse, 3));
    assert(mapper_hid_runtime_sample(&runtime, &input));
    mapper_step_profile(&state, &config, &input, &output, MAP_PROFILE_GOW_RAGNAROK);

    assert(output.left_y == -127);
    assert(output.buttons & MAP_BTN_R1);
    puts("HID runtime to GoW mapper test passed");
    return 0;
}
