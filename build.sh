#!/bin/bash
set -e

cd desktop

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

cd ..
mkdir -p release

find desktop/build -type f \( -name "pocketwalker.exe" -o -name "pocketwalker" -o -name "*.pdb" \) -exec cp {} release/ \;

echo "Finished building pocketwalker, the executable is located in ./release"