# USB HID adapter (portable layer)

This branch adds a small, SDK-independent adapter between standard USB HID Boot Keyboard/Boot Mouse reports and the existing MapperInput API.

## Scope

- Decodes the standard 8-byte Boot Keyboard report.
- Decodes the standard 3-byte Boot Mouse report.
- Converts letter usages, Space, Escape, Ctrl and Shift into mapper keys.
- Preserves left/right/middle mouse buttons and signed relative motion.
- Does not claim USB enumeration, PS4 controller injection, or in-game compatibility. Those require separate PS4/GoldHEN integration and real-hardware validation.

## Build the portable test

```sh
cc -std=c11 -Wall -Wextra -Werror -Iinclude src/mapper.c src/hid_adapter.c tests/test_hid_adapter.c -lm -o hid_adapter_test
./hid_adapter_test
```

## Relationship to ProCon

ProCon is a technical reference for USB/HID architecture. This adapter is an independent implementation of the standard Boot Keyboard/Mouse report format; it does not copy ProCon source files. Before any direct code reuse, verify and comply with the upstream repository license.
