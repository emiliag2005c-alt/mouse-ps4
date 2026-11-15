#include "hid_transport.h"

static bool poll_keyboard(MapperHidRuntime *runtime,
                          const MapperHidTransport *transport)
{
    uint8_t report[8];
    size_t length = 0;
    MapperHidReadStatus status;

    if (transport->read_keyboard == NULL) return true;
    status = transport->read_keyboard(transport->context, report,
                                      sizeof(report), &length);
    switch (status) {
    case MAPPER_HID_READ_NO_DATA:
        return true;
    case MAPPER_HID_READ_DISCONNECTED:
        mapper_hid_runtime_keyboard_disconnected(runtime);
        return true;
    case MAPPER_HID_READ_REPORT:
        return mapper_hid_runtime_keyboard(runtime, report, (uint32_t)length);
    case MAPPER_HID_READ_ERROR:
    default:
        return false;
    }
}

static bool poll_mouse(MapperHidRuntime *runtime,
                       const MapperHidTransport *transport)
{
    uint8_t report[3];
    size_t length = 0;
    MapperHidReadStatus status;

    if (transport->read_mouse == NULL) return true;
    status = transport->read_mouse(transport->context, report,
                                   sizeof(report), &length);
    switch (status) {
    case MAPPER_HID_READ_NO_DATA:
        return true;
    case MAPPER_HID_READ_DISCONNECTED:
        mapper_hid_runtime_mouse_disconnected(runtime);
        return true;
    case MAPPER_HID_READ_REPORT:
        return mapper_hid_runtime_mouse(runtime, report, (uint32_t)length);
    case MAPPER_HID_READ_ERROR:
    default:
        return false;
    }
}

bool mapper_hid_poll(MapperHidRuntime *runtime,
                     const MapperHidTransport *transport)
{
    if (runtime == NULL || transport == NULL) return false;
    if (!poll_keyboard(runtime, transport)) return false;
    if (!poll_mouse(runtime, transport)) return false;
    return true;
}
