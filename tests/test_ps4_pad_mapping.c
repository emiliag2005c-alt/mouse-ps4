#include "ps4_pad_mapping.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_all_mapped_buttons(void)
{
    MapperReport input;
    MapperPs4PadState output;

    memset(&input, 0, sizeof(input));
    input.buttons = MAP_BTN_CROSS | MAP_BTN_CIRCLE | MAP_BTN_SQUARE |
                    MAP_BTN_TRIANGLE | MAP_BTN_L1 | MAP_BTN_R1 |
                    MAP_BTN_L3 | MAP_BTN_R3 | MAP_BTN_OPTIONS;
    input.triggers = MAP_TRIGGER_L2 | MAP_TRIGGER_R2;
    input.left_x = -127;
    input.left_y = 127;
    input.right_x = 64;
    input.right_y = -64;

    assert(mapper_report_to_ps4_state(&input, &output));
    assert(output.buttons == (MAPPER_PS4_BTN_CROSS | MAPPER_PS4_BTN_CIRCLE |
           MAPPER_PS4_BTN_SQUARE | MAPPER_PS4_BTN_TRIANGLE |
           MAPPER_PS4_BTN_L1 | MAPPER_PS4_BTN_R1 | MAPPER_PS4_BTN_L3 |
           MAPPER_PS4_BTN_R3 | MAPPER_PS4_BTN_OPTIONS));
    assert(output.l2_analog == 255u);
    assert(output.r2_analog == 255u);
    assert(output.left_x == -127 && output.left_y == 127);
    assert(output.right_x == 64 && output.right_y == -64);
}

static void test_released_triggers_and_zeroed_output(void)
{
    MapperReport input;
    MapperPs4PadState output;

    memset(&input, 0, sizeof(input));
    memset(&output, 0xA5, sizeof(output));

    assert(mapper_report_to_ps4_state(&input, &output));
    assert(output.buttons == 0u);
    assert(output.l2_analog == 0u && output.r2_analog == 0u);
    assert(output.left_x == 0 && output.left_y == 0);
    assert(output.right_x == 0 && output.right_y == 0);
}

static void test_null_arguments(void)
{
    MapperReport input;
    MapperPs4PadState output;

    memset(&input, 0, sizeof(input));
    assert(!mapper_report_to_ps4_state(NULL, &output));
    assert(!mapper_report_to_ps4_state(&input, NULL));
}

int main(void)
{
    test_all_mapped_buttons();
    test_released_triggers_and_zeroed_output();
    test_null_arguments();
    puts("PS4 pad mapping tests passed");
    return 0;
}
