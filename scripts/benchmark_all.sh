#!/bin/bash

# Configuration
URL="http://localhost:8080/api/users"
REQUESTS=100000
CONCURRENCY=100
OUTPUT_DIR="benchmark_results"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
OUTPUT_FILE="${OUTPUT_DIR}/comparison_${TIMESTAMP}.csv"

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Print header
echo "Running benchmarks for High-Performance HTTP Server..."
echo "Target URL: $URL"
echo "Requests: $REQUESTS"
echo "Concurrency: $CONCURRENCY"
echo "---------------------------------------------------"

# Initialize results file
echo "Server,Requests/sec,Latency(ms),Transfer(KB/sec)" > "$OUTPUT_FILE"

# Function to run benchmark
run_benchmark() {
    local name=$1
    local cmd=$2
    
    echo "Benchmarking $name..."
    
    # Run ab and capture output
    # We use a temporary file to parse results
    local tmp_file=$(mktemp)
    
    if ! command -v ab &> /dev/null; then
        echo "Error: 'ab' (Apache Bench) is not installed."
        echo "Please install apache2-utils or httpd-tools."
        rm "$tmp_file"
        return 1
    fi
    
    ab -n $REQUESTS -c $CONCURRENCY -k $URL > "$tmp_file" 2>&1
    
    # Extract metrics
    local rps=$(grep "Requests per second:" "$tmp_file" | awk '{print $4}')
    local lat=$(grep "Time per request:" "$tmp_file" | grep "(mean)" | head -n 1 | awk '{print $4}')
    local trans=$(grep "Transfer rate:" "$tmp_file" | awk '{print $3}')
    
    echo "  RPS: $rps"
    echo "  Latency: $lat ms"
    echo "  Transfer: $trans KB/sec"
    
    # Save to CSV
    echo "$name,$rps,$lat,$trans" >> "$OUTPUT_FILE"
    
    rm "$tmp_file"
}

# Check if server is running
if ! curl -s "$URL" > /dev/null; then
    echo "Error: Server is not reachable at $URL"
    echo "Please start the server with: ./bin/HighPerformanceHttpServer"
    exit 1
fi

# Run benchmark for "This Server"
run_benchmark "This Server"

echo "---------------------------------------------------"
echo "Results saved to $OUTPUT_FILE"
echo "To compare with other servers, ensure they are running on the same port/endpoint or adjust the script."
