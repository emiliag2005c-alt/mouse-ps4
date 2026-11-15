#!/usr/bin/env sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD_DIR=$(mktemp -d)
trap 'rm -rf "$BUILD_DIR"' EXIT HUP INT TERM
CC=${CC:-cc}
CFLAGS="-std=c11 -Wall -Wextra -Werror -I$ROOT/include"

cd "$ROOT"

$CC $CFLAGS src/mapper.c tests/test_mapper.c -lm -o "$BUILD_DIR/test_mapper"
"$BUILD_DIR/test_mapper"

$CC $CFLAGS src/mapper.c src/hid_adapter.c tests/test_hid_adapter.c -o "$BUILD_DIR/test_hid_adapter"
"$BUILD_DIR/test_hid_adapter"

$CC $CFLAGS src/hid_adapter.c src/hid_runtime.c tests/test_hid_runtime.c -o "$BUILD_DIR/test_hid_runtime"
"$BUILD_DIR/test_hid_runtime"

$CC $CFLAGS src/mapper.c src/hid_adapter.c src/hid_runtime.c tests/test_hid_runtime_mapper.c -lm -o "$BUILD_DIR/test_hid_runtime_mapper"
"$BUILD_DIR/test_hid_runtime_mapper"

$CC $CFLAGS src/hid_adapter.c src/hid_runtime.c src/hid_transport.c tests/test_hid_transport.c -o "$BUILD_DIR/test_hid_transport"
"$BUILD_DIR/test_hid_transport"

$CC $CFLAGS src/ps4_pad_mapping.c tests/test_ps4_pad_mapping.c -o "$BUILD_DIR/test_ps4_pad_mapping"
"$BUILD_DIR/test_ps4_pad_mapping"

python3 tests/test_profile.py

printf '\nAll portable tests passed. This does not validate PS4 USB access or controller injection.\n'
