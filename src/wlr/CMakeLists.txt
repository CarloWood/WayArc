# Make sure that ${CMAKE_CURRENT_BINARY_DIR}/wlr_events_containers.txt exists and is up to date.
execute_process(
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/events_generator/generate-events-container-list.sh "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Cause an automatic re-configuration if the dependencies of ${CMAKE_CURRENT_BINARY_DIR}/wlr_events_containers.txt are changed.
set_property(
  DIRECTORY
  APPEND
  PROPERTY CMAKE_CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/events_generator/generate-events-container-list.sh
  ${CMAKE_CURRENT_SOURCE_DIR}/events_generator/analyzer.out
)

file(READ ${CMAKE_CURRENT_BINARY_DIR}/wlr_events_containers.txt WLR_EVENTS_CONTAINERS_CONTENT)
string(REGEX REPLACE "[\n]+" ";" WLR_EVENTS_CONTAINERS "${WLR_EVENTS_CONTAINERS_CONTENT}")

set(EVENTS_DEPENDENCIES "")

function(snake_to_pascal snake_string output_var)
  # Split the string by underscores.
  string(REPLACE "_" ";" parts "${snake_string}")

  # Initialize the result.
  set(pascal_string "")

  # Process each part.
  foreach(part ${parts})
    # Capitalize first letter of each part (except first part if lowercase pascal case is desired).
    string(SUBSTRING "${part}" 0 1 first_char)
    string(TOUPPER "${first_char}" first_char_upper)
    string(SUBSTRING "${part}" 1 -1 rest_of_part)
    string(CONCAT part_pascal "${first_char_upper}${rest_of_part}")

    # Append to result.
    string(APPEND pascal_string "${part_pascal}")
  endforeach()

  # Set the output variable in the parent scope.
  set(${output_var} "${pascal_string}" PARENT_SCOPE)
endfunction()

foreach(EVENTS_CONTAINER_DATA ${WLR_EVENTS_CONTAINERS})
  message(STATUS "EVENTS_CONTAINER_DATA = \"${EVENTS_CONTAINER_DATA}\".")
  # Replace the first two commas with semicolons.
  string(REGEX REPLACE "^([^,]*),([^,]*),(.*)$" "\\1;\\2;\\3" SPLIT_DATA "${EVENTS_CONTAINER_DATA}")
  message(STATUS "SPLIT_DATA = \"${SPLIT_DATA}\".")
  # Extract each part from the list.
  list(GET SPLIT_DATA 0 EVENTS_CONTAINER_NAME)
  list(GET SPLIT_DATA 1 SIGNAL_NAMES)
  list(GET SPLIT_DATA 2 SIGNAL_NAME_DATA_TYPES)
  # Generate the class name using PascalCase.
  snake_to_pascal(${EVENTS_CONTAINER_NAME} EVENTS_CONTAINER)

  add_custom_command(
    OUTPUT ${EVENTS_CONTAINER}.h
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/events_generator/generate_EventsContainer.sh "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" "${EVENTS_CONTAINER_NAME}" "${EVENTS_CONTAINER}" '${SIGNAL_NAMES}' '${SIGNAL_NAME_DATA_TYPES}'
    DEPENDS events_generator/generate_EventsContainer.sh
            events_generator/EventsGenerator.templ
    COMMENT "Generating wlr/${EVENTS_CONTAINER}.h"
  )

  list(APPEND EVENTS_DEPENDENCIES "${EVENTS_CONTAINER}.h")
endforeach()

# Must consume the generated OUTPUT's by a target in the same scope.
add_custom_target(generated-events-headers DEPENDS ${EVENTS_DEPENDENCIES})
