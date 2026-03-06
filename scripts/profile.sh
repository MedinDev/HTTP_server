#!/bin/bash

# Ensure we are in the project root
cd "$(dirname "$0")/.."

# Build with profiling flags
echo "Building with Profile configuration..."
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Profile ..
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

cd ..

# Check if valgrind and ab are installed
if ! command -v valgrind &> /dev/null; then
    echo "Valgrind not found. Please install it to use this script."
    exit 1
fi

if ! command -v ab &> /dev/null; then
    echo "Apache Bench (ab) not found. Please install it to use this script."
    exit 1
fi

echo "Starting server with Valgrind (callgrind)..."
valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./build/bin/HighPerformanceHttpServer &
SERVER_PID=$!

# Wait for server to start
echo "Waiting for server to start..."
sleep 5

echo "Running benchmark requests..."
ab -n 1000 -c 10 http://localhost:8080/

echo "Stopping server..."
kill $SERVER_PID

# Wait for server to exit and valgrind to finish writing
wait $SERVER_PID

echo "Generating profile report..."
if command -v gprof &> /dev/null; then
    if [ -f "gmon.out" ]; then
        gprof ./build/bin/HighPerformanceHttpServer gmon.out > profile.txt
        echo "GProf report generated: profile.txt"
    else
        echo "gmon.out not found (did the server run long enough?)"
    fi
else
    echo "gprof not found, skipping gprof report."
fi

echo "Generating Callgrind annotation..."
if command -v callgrind_annotate &> /dev/null; then
    callgrind_annotate callgrind.out > callgrind_report.txt
    echo "Callgrind report generated: callgrind_report.txt"
else
    echo "callgrind_annotate not found."
fi

echo "Profiling complete."
