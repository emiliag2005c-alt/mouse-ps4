#ifndef PS4_PAD_MAPPING_H
#define PS4_PAD_MAPPING_H

#include <stdbool.h>
#include <stdint.h>
#include "mapper.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Button masks verified against OpenOrbis include/orbis/_types/pad.h.
 * These are bit values only; this struct is NOT ScePadData or a native ABI. */
enum MapperPs4ButtonMask {
    MAPPER_PS4_BTN_L3       = 0x0002u,
    MAPPER_PS4_BTN_R3       = 0x0004u,
    MAPPER_PS4_BTN_OPTIONS  = 0x0008u,
    MAPPER_PS4_BTN_L2       = 0x0100u,
    MAPPER_PS4_BTN_R2       = 0x0200u,
    MAPPER_PS4_BTN_L1       = 0x0400u,
    MAPPER_PS4_BTN_R1       = 0x0800u,
    MAPPER_PS4_BTN_TRIANGLE = 0x1000u,
    MAPPER_PS4_BTN_CIRCLE   = 0x2000u,
    MAPPER_PS4_BTN_CROSS    = 0x4000u,
    MAPPER_PS4_BTN_SQUARE   = 0x8000u
};

typedef struct MapperPs4PadState {
    uint32_t buttons;
    uint8_t l2_analog;
    uint8_t r2_analog;
    int8_t left_x;
    int8_t left_y;
    int8_t right_x;
    int8_t right_y;
} MapperPs4PadState;

/* Converts normalized mapper output to PS4 button masks and digital
 * trigger pressure (0 or 255). It does not construct or inject ScePadData. */
bool mapper_report_to_ps4_state(const MapperReport *input,
                                MapperPs4PadState *output);

#ifdef __cplusplus
}
#endif

#endif
