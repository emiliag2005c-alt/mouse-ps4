#include "hid_transport.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct FakeUsb {
    MapperHidReadStatus keyboard_status;
    MapperHidReadStatus mouse_status;
    uint8_t keyboard[8];
    uint8_t mouse[3];
} FakeUsb;

static MapperHidReadStatus read_keyboard(void *context, uint8_t *out,
                                         size_t capacity, size_t *length)
{
    FakeUsb *fake = (FakeUsb *)context;
    if (fake->keyboard_status == MAPPER_HID_READ_REPORT) {
        assert(capacity >= sizeof(fake->keyboard));
        memcpy(out, fake->keyboard, sizeof(fake->keyboard));
        *length = sizeof(fake->keyboard);
    }
    return fake->keyboard_status;
}

static MapperHidReadStatus read_mouse(void *context, uint8_t *out,
                                      size_t capacity, size_t *length)
{
    FakeUsb *fake = (FakeUsb *)context;
    if (fake->mouse_status == MAPPER_HID_READ_REPORT) {
        assert(capacity >= sizeof(fake->mouse));
        memcpy(out, fake->mouse, sizeof(fake->mouse));
        *length = sizeof(fake->mouse);
    }
    return fake->mouse_status;
}

int main(void)
{
    FakeUsb fake;
    MapperHidRuntime runtime;
    MapperHidTransport transport;
    MapperInput input;

    memset(&fake, 0, sizeof(fake));
    mapper_hid_runtime_reset(&runtime);
    transport.context = &fake;
    transport.read_keyboard = read_keyboard;
    transport.read_mouse = read_mouse;

    fake.keyboard_status = MAPPER_HID_READ_REPORT;
    fake.keyboard[2] = 0x1a; /* W */
    fake.mouse_status = MAPPER_HID_READ_REPORT;
    fake.mouse[0] = 1;
    fake.mouse[1] = 4;
    assert(mapper_hid_poll(&runtime, &transport));
    assert(mapper_hid_runtime_sample(&runtime, &input));
    assert(input.keys['w'] && input.mouse_left && input.mouse_dx == 4);

    /* NO_DATA means held state remains; no duplicate relative motion. */
    fake.keyboard_status = MAPPER_HID_READ_NO_DATA;
    fake.mouse_status = MAPPER_HID_READ_NO_DATA;
    assert(mapper_hid_poll(&runtime, &transport));
    assert(mapper_hid_runtime_sample(&runtime, &input));
    assert(input.keys['w'] && input.mouse_left && input.mouse_dx == 0);

    /* Disconnects release only the affected interface. */
    fake.keyboard_status = MAPPER_HID_READ_DISCONNECTED;
    fake.mouse_status = MAPPER_HID_READ_NO_DATA;
    assert(mapper_hid_poll(&runtime, &transport));
    assert(mapper_hid_runtime_sample(&runtime, &input));
    assert(!input.keys['w'] && input.mouse_left);

    fake.keyboard_status = MAPPER_HID_READ_NO_DATA;
    fake.mouse_status = MAPPER_HID_READ_DISCONNECTED;
    assert(mapper_hid_poll(&runtime, &transport));
    assert(mapper_hid_runtime_sample(&runtime, &input));
    assert(!input.keys['w'] && !input.mouse_left);

    fake.keyboard_status = MAPPER_HID_READ_ERROR;
    assert(!mapper_hid_poll(&runtime, &transport));

    puts("HID transport adapter tests passed");
    return 0;
}
