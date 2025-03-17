# Include targets declared in events_generator.
add_subdirectory(events_generator)

# Add custom command to generate events.h.
add_custom_command(
    OUTPUT events.h
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/events_generator/generate.sh ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} events
    DEPENDS events_generator/generate.sh
            generate
            events_generator/events_header.h
            events_generator/analyzer.out
    COMMENT "Generating wlr/events.h"
    VERBATIM
)

# Add custom command to generate listener.h.
add_custom_command(
    OUTPUT listener.h
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/events_generator/generate.sh ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} listener
    DEPENDS events_generator/generate.sh
            generate
            events_generator/listener_header.h
            events_generator/analyzer.out
    COMMENT "Generating wlr/listener.h"
    VERBATIM
)

# Must consume the generated OUTPUT's by a target in the same scope.
add_custom_target(generated-events-headers DEPENDS events.h listener.h)
