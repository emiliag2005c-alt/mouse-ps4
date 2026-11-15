#include "hid_adapter.h"

#include <string.h>

static bool keyboard_has_usage(const uint8_t *report, uint8_t usage)
{
    size_t i;
    for (i = 2; i < 8; ++i) {
        if (report[i] == usage) return true;
    }
    return false;
}

bool mapper_input_from_hid(const uint8_t *keyboard_report,
                           size_t keyboard_length,
                           const uint8_t *mouse_report,
                           size_t mouse_length,
                           MapperInput *out)
{
    size_t i;

    if (out == NULL) return false;
    memset(out, 0, sizeof(*out));

    if (keyboard_report != NULL) {
        if (keyboard_length < 8) return false;
        /* HID usages 0x04..0x1d represent A..Z. */
        for (i = 0; i < 26; ++i) {
            const uint8_t usage = (uint8_t)(0x04 + i);
            if (keyboard_has_usage(keyboard_report, usage)) {
                out->keys[(unsigned char)('a' + i)] = true;
                out->keys[(unsigned char)('A' + i)] = true;
            }
        }
        if (keyboard_has_usage(keyboard_report, 0x2c))
            out->keys[MAP_KEY_SPACE] = true;
        if (keyboard_has_usage(keyboard_report, 0x29))
            out->keys[27] = true; /* Escape */
        /* Modifier byte: left/right Ctrl are bits 0/4; Shift bits 1/5. */
        if ((keyboard_report[0] & 0x11u) != 0)
            out->keys[MAP_KEY_CTRL] = true;
        if ((keyboard_report[0] & 0x22u) != 0)
            out->keys[MAP_KEY_SHIFT] = true;
    }

    if (mouse_report != NULL) {
        if (mouse_length < 3) return false;
        out->mouse_left = (mouse_report[0] & 0x01u) != 0;
        out->mouse_right = (mouse_report[0] & 0x02u) != 0;
        out->mouse_middle = (mouse_report[0] & 0x04u) != 0;
        out->mouse_dx = (int8_t)mouse_report[1];
        out->mouse_dy = (int8_t)mouse_report[2];
    }
    return true;
}
