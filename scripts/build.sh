#!/bin/bash
set -e

mkdir -p build
cd build
cmake ..
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu)
