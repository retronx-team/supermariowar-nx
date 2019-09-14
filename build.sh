#!/bin/bash

set -eo pipefail

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source "$ROOT_DIR/_env.sh"

if [[ -z "$DEVKITPRO" ]]; then
	echo "env var DEVKITPRO is missing" 1>&2
	exit 1
fi

if [[ -z "$MAKE_OPTS" ]]; then
	MAKE_OPTS="-j$(nproc --all)"
fi

if [[ "$1" == "debug" ]]; then
	BUILD_DIR="$ROOT_DIR/build/debug"
	CMAKE_BUILD_TYPE="Debug"
	PROGRAM_SUFFIX="-debug"
else
	BUILD_DIR="$ROOT_DIR/build/release"
	CMAKE_BUILD_TYPE="Release"
	PROGRAM_SUFFIX=""
fi

OUT_DIR="$ROOT_DIR/out"

mkdir -p "$BUILD_DIR"

source "$DEVKITPRO/switchvars.sh"
DEFINES="-g -D__SWITCH__ -D__unix__ -D_BSD_SOURCE -fexceptions"
INCLUDES="-I$DEVKITPRO/libnx/include"
PLATFORM_TARGET_LIBS="nx;glad;EGL;glapi;drm_nouveau;vorbisidec;modplug;mpg123;m;opusfile;ogg;opus;png16;z;png;jpeg;webp"

cd "$BUILD_DIR"
cmake -G"Unix Makefiles" "$ROOT_DIR/supermariowar" \
	-DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
	-DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/switch.cmake" \
	-DCMAKE_C_FLAGS="$CFLAGS $CPPFLAGS $DEFINES $INCLUDES" \
	-DCMAKE_CXX_FLAGS="$CFLAGS $DEFINES $INCLUDES" \
	-DCMAKE_AR="$DEVKITPRO/devkitA64/bin/aarch64-none-elf-gcc-ar" \
	-DPLATFORM_TARGET_LIBS:LIST="$PLATFORM_TARGET_LIBS" \
	-DUSE_SDL2_LIBS:BOOL="ON" -DSDL2_FORCE_GLES:BOOL="ON" \
	-DDISABLE_DEFAULT_CFLAGS:BOOL="ON" -DNO_NETWORK:BOOL="ON"

make $MAKE_OPTS -- smw

mkdir -p "$OUT_DIR"

unzip -o -q "$ROOT_DIR/supermariowar/data.zip" -d "$OUT_DIR"
echo "Assets copied to $OUT_DIR"

cp -f "$BUILD_DIR/Binaries/$CMAKE_BUILD_TYPE/smw" "$OUT_DIR/$APP_NAME$PROGRAM_SUFFIX.elf"

"$DEVKITPRO/tools/bin/nacptool" --create "$APP_TITLE$PROGRAM_SUFFIX" "$APP_AUTHOR" "$APP_VERSION" "$OUT_DIR/$APP_NAME$PROGRAM_SUFFIX.nacp"
"$DEVKITPRO/tools/bin/elf2nro" "$OUT_DIR/$APP_NAME$PROGRAM_SUFFIX.elf" "$OUT_DIR/$APP_NAME$PROGRAM_SUFFIX.nro" \
	--nacp="$OUT_DIR/$APP_NAME$PROGRAM_SUFFIX.nacp" \
	--icon="$ROOT_DIR/icon.jpg"

echo ""
echo "Built $OUT_DIR/$APP_NAME$PROGRAM_SUFFIX.nro"
