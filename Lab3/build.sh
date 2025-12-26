#!/bin/bash
echo "Building project..."
mkdir -p build
cd build
cmake ..
make -j$(nproc)
echo "Build complete. Run ./run.sh to start."
