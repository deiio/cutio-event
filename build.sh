#!/bin/bash

SOURCE_DIR=$(pwd)
BUILD_DIR=${BUILD_DIR:-./build/debug}
mkdir -p "${BUILD_DIR}"
cmake -B "${BUILD_DIR}"
cmake --build "${BUILD_DIR}"
