# Stateful HID runtime bridge

This layer sits between a platform USB reader and the portable mapper.

- Keyboard and mouse interfaces may be polled independently.
- The latest keyboard report is retained, so a mouse packet does not erase held keys.
- Mouse button state is retained, while relative X/Y movement is accumulated and consumed once per mapper sample.
- Disconnect handling is independent: call `mapper_hid_runtime_keyboard_disconnected` or `mapper_hid_runtime_mouse_disconnected` when that interface disappears. Use `mapper_hid_runtime_reset` to clear both.
- The current implementation accepts USB HID Boot reports only (8-byte keyboard, 3-byte mouse).

## Deliberate boundaries

This module does **not** enumerate USB devices, claim interfaces, load PS4 system modules, hook controller APIs, or inject DualShock reports. Those operations require a PS4/GoldHEN-specific runtime and hardware validation. It also does not yet parse arbitrary HID report descriptors; non-Boot devices need a separate descriptor-driven decoder.

## Portable tests

```sh
cc -std=c11 -Wall -Wextra -Werror -Iinclude \
  src/hid_adapter.c src/hid_runtime.c tests/test_hid_runtime.c -o hid_runtime_test
./hid_runtime_test

cc -std=c11 -Wall -Wextra -Werror -Iinclude \
  src/mapper.c src/hid_adapter.c src/hid_runtime.c tests/test_hid_runtime_mapper.c -o hid_runtime_mapper_test
./hid_runtime_mapper_test
```


## Platform transport contract

`include/hid_transport.h` defines a callback contract for a future platform USB backend. It distinguishes a fresh report, no new packet, device disconnect, and read error. No-data preserves held state; disconnect releases only the affected device. The portable polling bridge is tested with a fake transport in `tests/test_hid_transport.c`.

This is not a PS4 USB backend: device enumeration, interface claiming, endpoint setup, and actual transfers still need a GoldHEN-compatible implementation and real-console testing.
