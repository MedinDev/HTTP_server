# HTTP Server

A high-performance HTTP server implementation in C++17.

## Features

- **Asynchronous I/O**: Using non-blocking sockets and event-driven architecture.
- **Thread Pool**: Efficient request processing with a configurable thread pool.
- **HTTP/1.1 Support**: Basic support for GET, POST, PUT, DELETE methods.
- **Static File Serving**: Efficiently serves static files from a configured directory.
- **Routing**: Flexible routing system for dynamic endpoints.
- **Logging**: Thread-safe logging system.

## Project Structure

```
http-server/
├── include/        # Header files
├── src/            # Source files
├── tests/          # Unit and integration tests
├── examples/       # Usage examples
├── scripts/        # Build and utility scripts
├── config/         # Configuration files
└── www/            # Web root for static files
```

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## Run Server

```bash
# Run from build directory
./http_server

# Or run from project root
./build/http_server
```

## Run Examples

```bash
# Simple API Server
./build/simple_api

# Static File Server
./build/static_server
```

## Testing

```bash
cd build
ctest
```
