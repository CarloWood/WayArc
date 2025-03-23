#! /bin/bash

# This script should be called from src/wlr/CMakeLists.txt with the following arguments (refering to src/wlr).
CURRENT_SOURCE_DIR="$1"
CURRENT_BINARY_DIR="$2"
EVENTS_CONTAINER_NAME="$3"
EVENTS_CONTAINER="$4"
SIGNAL_NAMES_SEQ="$5"
SIGNAL_NAME_DATA_TYPE_SEQ="$6"

#echo "Running generate_EventsContainer.sh ${CURRENT_SOURCE_DIR} ${CURRENT_BINARY_DIR} ${EVENTS_CONTAINER_NAME} ${EVENTS_CONTAINER} ${SIGNAL_NAMES_SEQ} ${SIGNAL_NAME_DATA_TYPE_SEQ}"

echo "// THIS FILE IS GENERATED - DO NOT EDIT." > "${CURRENT_BINARY_DIR}/${EVENTS_CONTAINER}.h"
echo -e "//\n// To examine this file; run:\n//\n//  ${CURRENT_SOURCE_DIR}/events_generator/show.sh ${CURRENT_BINARY_DIR}/${EVENTS_CONTAINER}.h\n" >> "${CURRENT_BINARY_DIR}/${EVENTS_CONTAINER}.h"
sed -e 's/@EVENTS_CONTAINER@/'"${EVENTS_CONTAINER}"'/g;s/@EVENTS_CONTAINER_NAME@/'"${EVENTS_CONTAINER_NAME}"'/g;s/@SIGNAL_NAMES_SEQ@/'"${SIGNAL_NAMES_SEQ}"'/g;s/@SIGNAL_NAME_DATA_TYPE_SEQ@/'"${SIGNAL_NAME_DATA_TYPE_SEQ}"'/' "${CURRENT_SOURCE_DIR}/events_generator/EventsGenerator.templ" >> "${CURRENT_BINARY_DIR}/${EVENTS_CONTAINER}.h"
