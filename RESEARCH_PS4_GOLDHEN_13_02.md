# Research: GoldHEN plugin, USB HID and Pad APIs (PS4 13.02 target)

Research snapshot: 2026-11-14.

This note records upstream public headers and examples found for the PS4 Input Mapper. It is a reference map, not a claim that the current project has been built or validated on firmware 13.02.

## 1. GoldHEN plugin SDK

Upstream repository:
https://github.com/GoldHEN/GoldHEN_Plugins_SDK

Useful paths:
- `include/GoldHEN.h`: `sys_sdk_cmd`, `sys_sdk_version`, `sys_sdk_proc_info`, `sys_sdk_proc_rw`, PRX load/unload declarations and process info structures.
- `include/Patcher.h`: `Patcher_Construct`, `Patcher_Install_Patch`, `Patcher_Restore_Patch`, `Patcher_Destroy`.
- `include/Common.h`: umbrella includes for GoldHEN SDK utilities and OpenOrbis libkernel.
- `README.md`: SDK build integration; requires OpenOrbis PS4 Toolchain and LLVM 10+.

Important distinction: `plugin_load`, `plugin_unload`, `module_start`, and `module_stop` are demonstrated by GoldHEN plugin templates and loader conventions; they are not declarations in `GoldHEN.h`. The plugin loader resolves `plugin_load` and `plugin_unload` with `sceKernelDlsym`.

Example:
https://github.com/GoldHEN/GoldHEN_Plugins_Repository/blob/main/plugin_src/plugin_template/source/main.c

## 2. USB device access / libSceUsbd

OpenOrbis header:
https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain/blob/master/include/orbis/Usbd.h

Associated libusb structure declarations:
https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain/blob/master/include/orbis/_types/libusb.h

The Usbd header documents that `libSceUsbd.sprx` must be loaded. Declared functions include:

- Lifecycle: `sceUsbdInit()`, `sceUsbdExit()`.
- Enumeration: `sceUsbdGetDeviceList()`, `sceUsbdFreeDeviceList()`.
- Open/close: `sceUsbdOpen()`, `sceUsbdClose()`, `sceUsbdOpenDeviceWithVidPid()`.
- Descriptors: `sceUsbdGetDeviceDescriptor()`, `sceUsbdGetActiveConfigDescriptor()`, `sceUsbdGetConfigDescriptor()`, `sceUsbdGetDescriptor()`.
- Interfaces: `sceUsbdClaimInterface()`, `sceUsbdReleaseInterface()`, `sceUsbdSetInterfaceAltSetting()`.
- Transfers: `sceUsbdControlTransfer()`, `sceUsbdInterruptTransfer()`, `sceUsbdBulkTransfer()`.
- Async transfers: `sceUsbdAllocTransfer()`, `sceUsbdSubmitTransfer()`, `sceUsbdCancelTransfer()`, event handling helpers.

For keyboard/mouse HID, the likely starting path is enumerate -> inspect device/config/interface/endpoint descriptors -> claim the HID interface -> perform interrupt IN reads -> decode the report descriptor or support Boot Protocol. This is a development plan inferred from the declared USB API, not a verified PS4 implementation.

Dependency caveat: `Usbd.h` includes `<libusb.h>`, while this toolchain repository also has PS4-side libusb structure declarations under `include/orbis/_types/libusb.h`. Confirm the exact include resolution and link/runtime dependencies in the selected OpenOrbis toolchain before compiling.

## 3. DualShock / Pad API and confirmed button masks

OpenOrbis headers:
- https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain/blob/master/include/orbis/Pad.h
- https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain/blob/master/include/orbis/_types/pad.h

The `Pad.h` declarations include:
- `scePadInit`
- `scePadOpen` / `scePadOpenExt`
- `scePadClose`
- `scePadRead`
- `scePadReadState`
- `scePadSetVibration`, `scePadSetLightBar`, and information APIs.

The `_types/pad.h` enum confirms these button masks:

| Button | Mask |
|---|---:|
| L3 | 0x0002 |
| R3 | 0x0004 |
| OPTIONS | 0x0008 |
| UP | 0x0010 |
| RIGHT | 0x0020 |
| DOWN | 0x0040 |
| LEFT | 0x0080 |
| L2 | 0x0100 |
| R2 | 0x0200 |
| L1 | 0x0400 |
| R1 | 0x0800 |
| TRIANGLE | 0x1000 |
| CIRCLE | 0x2000 |
| CROSS | 0x4000 |
| SQUARE | 0x8000 |
| TOUCH_PAD | 0x100000 |

These are confirmed from the public OpenOrbis header, not values guessed from memory. The GoldHEN gamepad_helper plugin's `plugin_src/gamepad_helper/include/pad.h` independently repeats the same masks and demonstrates how they are applied to `ScePadData.buttons`.

Gamepad helper example:
https://github.com/GoldHEN/GoldHEN_Plugins_Repository/blob/main/plugin_src/gamepad_helper/include/pad.h
https://github.com/GoldHEN/GoldHEN_Plugins_Repository/blob/main/plugin_src/gamepad_helper/source/main.c

## 4. GoldHEN plugin example for Pad hooks

The public `gamepad_helper` source demonstrates:
- `plugin_load` / `plugin_unload` lifecycle.
- loading `libScePad.sprx` with `sys_dynlib_load_prx`.
- `HOOK32(scePadRead)` and `HOOK32(scePadReadState)`.
- reading the original implementations through `scePadReadExt` / `scePadReadStateExt`.
- modifying the returned `ScePadData.buttons` mask.

Source:
https://github.com/GoldHEN/GoldHEN_Plugins_Repository/blob/main/plugin_src/gamepad_helper/source/main.c

This is useful evidence for the Pad-hook pattern, but it is not a ready-made virtual controller backend. It also uses patching of internal Pad functions; that path needs careful compatibility and crash-safety review.

## 5. Critical unknowns for firmware 13.02

The public headers provide declarations, but do not prove that every symbol is exported/usable in the exact GoldHEN + firmware 13.02 runtime. Before enabling hardware tests, verify on the target:
1. GoldHEN build/version actually running on 13.02 and plugin loader availability.
2. `libSceUsbd.sprx` load path and exported symbols.
3. USB enumeration and interface claiming with the actual keyboard/mouse.
4. Endpoint polling behavior, report protocol and report descriptor parsing.
5. A safe, supported route to feed a virtual DS4 report to games.

The OpenOrbis `Pad.h` also lists some virtual-device-related symbols as not yet reversed in comments. Do not assume `scePadVirtualDeviceAddDevice` / `InsertData` are usable merely because names appear in a header.

**Status:** Button masks are verified against the public header. Plugin lifecycle conventions, Pad hook examples and USB API declarations are located. Firmware 13.02 symbol availability, physical HID polling and virtual-controller injection remain unverified.
