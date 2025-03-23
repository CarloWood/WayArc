#! /bin/bash

# This script should be called from src/wlr/CMakeLists.txt with the following arguments (refering to src/wlr).
CURRENT_SOURCE_DIR="$1"
CURRENT_BINARY_DIR="$2"

INPUT_FILE="${CURRENT_SOURCE_DIR}/events_generator/analyzer.out"
OUTPUT_FILE="${CURRENT_BINARY_DIR}/wlr_events_containers.txt"

if [[ ! -e ${OUTPUT_FILE} || (${OUTPUT_FILE} -ot $0) || (${OUTPUT_FILE} -ot ${INPUT_FILE}) ]]; then
  echo "(Re)generating \"${OUTPUT_FILE}\"..."
  
  # Truncate or create the output file
  > "${OUTPUT_FILE}"
  
  # Process input file and generate output
  awk -F', ' '
    BEGIN {
      prev_component = "";
      events = "";
      event_types = "";
    }
    
    {
      # Extract the component name by removing "wlr_" prefix
      component = $1;
      sub(/^wlr_/, "", component);
      
      # Store the event name and type
      event = $2;
      type = $3;
      if (type ~ /^wlr_/) {
        type = "struct " type;
      } else if (type == "nullptr_t") {
        type = "std::nullptr_t";
      }
      
      # If we encounter a new component, output the previous one
      if (prev_component != "" && prev_component != component) {
        print prev_component "," events "," event_types > "'"${OUTPUT_FILE}"'";
        events = "";
        event_types = "";
      }
      
      # Add this event to the list
      if (events == "") {
        events = "(" event ")";
      } else {
        events = events "(" event ")";
      }
      
      # Add this event+type pair to the list
      if (event_types == "") {
        event_types = "((" event ", " type "))";
      } else {
        event_types = event_types "((" event ", " type "))";
      }
      
      # Update the previous component
      prev_component = component;
    }
    
    END {
      # Output the last component
      if (prev_component != "") {
        print prev_component "," events "," event_types > "'"${OUTPUT_FILE}"'";
      }
    }
  ' "${INPUT_FILE}"
fi
