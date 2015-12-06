#!/bin/bash

mkdir -p build
cd build
cmake .. -DFL_LIBRARY:FILEPATH=/usr/lib/x86_64-linux-gnu/libfl.a
make

