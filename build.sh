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

SWITCH_PKG_CONFIG="${PORTLIBS_PREFIX}/bin/${TOOL_PREFIX}pkg-config"

DEPS="sdl2 SDL2_image SDL2_mixer"
DEFINES="-D__unix__ -D_BSD_SOURCE -fexceptions"
ADDITIONAL_LINK_FLAGS="-specs=$DEVKITPRO/libnx/switch.specs"
DEPS_CFLAGS="$("$SWITCH_PKG_CONFIG" $DEPS --cflags)"
DEPS_LDFLAGS="$("$SWITCH_PKG_CONFIG" $DEPS --libs)"
DEPS_PLATFORM_LIBS="$(grep -oP -- '-l\K\S*' <<< "$DEPS_LDFLAGS" | paste -sd ";" -)"

cd "$BUILD_DIR"
cmake -G"Unix Makefiles" \
	-DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/switch.cmake" \
	-DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
	-DCMAKE_C_FLAGS="$CFLAGS $DEPS_CFLAGS $DEFINES" \
	-DCMAKE_CXX_FLAGS="$CXXFLAGS $DEPS_CFLAGS $DEFINES" \
	-DCMAKE_EXE_LINKER_FLAGS="$LDFLAGS $DEPS_LDFLAGS $ADDITIONAL_LINK_FLAGS" \
	-DPLATFORM_TARGET_LIBS:LIST="$DEPS_PLATFORM_LIBS" \
	-DDISABLE_SYSLIB_CHECKS:BOOL="ON" \
	-DUSE_SDL2_LIBS:BOOL="ON" -DSDL2_FORCE_GLES:BOOL="ON" \
	-DDISABLE_DEFAULT_CFLAGS:BOOL="ON" -DNO_NETWORK:BOOL="ON" \
	"$ROOT_DIR/supermariowar"

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
