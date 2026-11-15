#include "ps4_pad_mapping.h"

#include <stddef.h>
#include <string.h>

bool mapper_report_to_ps4_state(const MapperReport *input,
                                MapperPs4PadState *output)
{
    if (input == NULL || output == NULL) return false;

    memset(output, 0, sizeof(*output));

    if ((input->buttons & MAP_BTN_L3) != 0)
        output->buttons |= MAPPER_PS4_BTN_L3;
    if ((input->buttons & MAP_BTN_R3) != 0)
        output->buttons |= MAPPER_PS4_BTN_R3;
    if ((input->buttons & MAP_BTN_OPTIONS) != 0)
        output->buttons |= MAPPER_PS4_BTN_OPTIONS;
    if ((input->buttons & MAP_BTN_L1) != 0)
        output->buttons |= MAPPER_PS4_BTN_L1;
    if ((input->buttons & MAP_BTN_R1) != 0)
        output->buttons |= MAPPER_PS4_BTN_R1;
    if ((input->buttons & MAP_BTN_TRIANGLE) != 0)
        output->buttons |= MAPPER_PS4_BTN_TRIANGLE;
    if ((input->buttons & MAP_BTN_CIRCLE) != 0)
        output->buttons |= MAPPER_PS4_BTN_CIRCLE;
    if ((input->buttons & MAP_BTN_CROSS) != 0)
        output->buttons |= MAPPER_PS4_BTN_CROSS;
    if ((input->buttons & MAP_BTN_SQUARE) != 0)
        output->buttons |= MAPPER_PS4_BTN_SQUARE;

    output->l2_analog = (input->triggers & MAP_TRIGGER_L2) != 0 ? 255u : 0u;
    output->r2_analog = (input->triggers & MAP_TRIGGER_R2) != 0 ? 255u : 0u;

    output->left_x = input->left_x;
    output->left_y = input->left_y;
    output->right_x = input->right_x;
    output->right_y = input->right_y;

    return true;
}
