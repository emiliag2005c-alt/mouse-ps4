#include "hid_adapter.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
int main(void)
{
    const uint8_t keyboard[8] = {0x23, 0, 0x1a, 0x04, 0x2c, 0x08, 0x29, 0x14};
    const uint8_t mouse[3] = {0x07, 8, (uint8_t)-4};
    MapperInput input;
    MapperConfig config;
    MapperState state;
    MapperReport report;
    assert(mapper_input_from_hid(keyboard, 8, mouse, 3, &input));
    mapper_default_config(&config);
    mapper_reset(&state);
    mapper_step_profile(&state, &config, &input, &report, MAP_PROFILE_GOW_RAGNAROK);
    assert(report.left_y == -127);
    assert(report.buttons & MAP_BTN_CROSS);
    assert(report.buttons & MAP_BTN_OPTIONS);
    assert(report.buttons & MAP_BTN_L1);
    assert(report.buttons & MAP_BTN_R1);
    assert(report.buttons & MAP_BTN_L3);
    assert(report.buttons & MAP_BTN_R3);
    assert(report.triggers & MAP_TRIGGER_L2);
    assert(report.triggers & MAP_TRIGGER_R2);
    puts("HID-to-GoW mapper integration tests passed");
    return 0;
}
