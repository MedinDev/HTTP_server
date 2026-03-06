#!/bin/bash
# Needs perf tools installed
perf record -g ./build/http_server
perf report
