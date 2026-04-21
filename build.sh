#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
STATIC=0

if [[ "$*" == *"--static"* ]]; then
  STATIC=1
fi

cd "$SCRIPT_DIR/desktop"
rm -rf build

if [ "$STATIC" -eq 1 ]; then
  cmake -B build/release -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
    -DCMAKE_MAKE_PROGRAM="$MINGW_PREFIX/bin/ninja.exe" \
    -DCMAKE_C_COMPILER="$MINGW_PREFIX/bin/gcc.exe" \
    -DCMAKE_CXX_COMPILER="$MINGW_PREFIX/bin/g++.exe" \
    -DVCPKG_TARGET_TRIPLET=x64-mingw-static \
    -DVCPKG_HOST_TRIPLET=x64-mingw-static \
    -DCMAKE_EXE_LINKER_FLAGS="-static-libgcc -static-libstdc++ -static -lpthread"
else
  cmake --preset=release
fi

cmake --build build/release

cp -r "$SCRIPT_DIR/desktop/build/release/release/." "$SCRIPT_DIR/release/"

echo -e "\nFinished building pocketwalker, the executable is located in ./release"