#include "hid_adapter.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    const uint8_t keyboard[8] = {0x03, 0, 0x1a, 0x04, 0x2c, 0, 0, 0};
    const uint8_t mouse[3] = {0x05, 12, (uint8_t)-7};
    MapperInput input;

    assert(mapper_input_from_hid(keyboard, sizeof(keyboard), mouse, sizeof(mouse), &input));
    assert(input.keys['w'] && input.keys['W']);
    assert(input.keys['a'] && input.keys['A']);
    assert(input.keys[MAP_KEY_SPACE]);
    assert(input.keys[MAP_KEY_CTRL] && input.keys[MAP_KEY_SHIFT]);
    assert(input.mouse_left && !input.mouse_right && input.mouse_middle);
    assert(input.mouse_dx == 12 && input.mouse_dy == -7);
    assert(!mapper_input_from_hid(keyboard, 7, mouse, sizeof(mouse), &input));
    assert(!mapper_input_from_hid(keyboard, sizeof(keyboard), mouse, 2, &input));
    puts("HID adapter tests passed");
    return 0;
}
