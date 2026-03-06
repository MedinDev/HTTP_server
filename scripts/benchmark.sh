#!/bin/bash
# Needs apache2-utils (ab)
ab -n 100000 -c 100 -k http://localhost:8080/
