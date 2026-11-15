# Stateful HID runtime bridge

This layer sits between a platform USB reader and the portable mapper.

- Keyboard and mouse interfaces may be polled independently.
- The latest keyboard report is retained, so a mouse packet does not erase held keys.
- Mouse button state is retained, while relative X/Y movement is accumulated and consumed once per mapper sample.
- Disconnect handling is explicit: call `mapper_hid_runtime_reset` so stale held inputs cannot remain active.
- The current implementation accepts USB HID Boot reports only (8-byte keyboard, 3-byte mouse).

## Deliberate boundaries

This module does **not** enumerate USB devices, claim interfaces, load PS4 system modules, hook controller APIs, or inject DualShock reports. Those operations require a PS4/GoldHEN-specific runtime and hardware validation. It also does not yet parse arbitrary HID report descriptors; non-Boot devices need a separate descriptor-driven decoder.

## Portable tests

```sh
cc -std=c11 -Wall -Wextra -Werror -Iinclude \
  src/hid_adapter.c src/hid_runtime.c tests/test_hid_runtime.c -o hid_runtime_test
./hid_runtime_test
```
