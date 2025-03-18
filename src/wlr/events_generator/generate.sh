#! /bin/bash

# This script should be called from src/wlr/CMakeLists.txt with the following arguments (refering to src/wlr).
CURRENT_SOURCE_DIR="$1"
CURRENT_BUILD_DIR="$2"

# BASE should be either "events" or "listener".
BASE="$3"

echo "// THIS FILE IS GENERATED - DO NOT EDIT." > "${CURRENT_BUILD_DIR}"/${BASE}.h
cat ${CURRENT_SOURCE_DIR}/events_generator/${BASE}_header.h >> "${CURRENT_BUILD_DIR}"/${BASE}.h
${CURRENT_BUILD_DIR}/events_generator/generate --${BASE} ${CURRENT_SOURCE_DIR}/events_generator/analyzer.out >> "${CURRENT_BUILD_DIR}"/${BASE}.h
