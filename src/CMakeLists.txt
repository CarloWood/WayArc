include(AICxxProject)

#==============================================================================
# Generate XDG_SHELL_PROTOCOL_H

set(XDG_SHELL_PROTOCOL_H "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.h")

# Find wayland-protocols package and get its pkgdatadir.
pkg_get_variable(WAYLAND_PROTOCOLS_PKGDATADIR wayland-protocols pkgdatadir)

if (NOT WAYLAND_PROTOCOLS_PKGDATADIR)
  message(FATAL_ERROR "Could not find wayland-protocols' variable pkgdatadir.")
endif ()

# Find wayland-scanner executable path.
pkg_get_variable(WAYLAND_SCANNER_EXECUTABLE wayland-scanner wayland_scanner)

if (NOT WAYLAND_SCANNER_EXECUTABLE)
  message(FATAL_ERROR "Could not find wayland-scanner's variable wayland_scanner.")
endif ()

# Add custom command to generate XDG_SHELL_PROTOCOL_H.
set(XDG_SHELL_XML ${WAYLAND_PROTOCOLS_PKGDATADIR}/stable/xdg-shell/xdg-shell.xml)
add_custom_command(
    OUTPUT ${XDG_SHELL_PROTOCOL_H}
    COMMAND ${WAYLAND_SCANNER_EXECUTABLE} server-header ${XDG_SHELL_XML} ${XDG_SHELL_PROTOCOL_H}
    DEPENDS ${XDG_SHELL_XML}
    COMMENT "Generating xdg-shell-protocol.h"
    VERBATIM
)

# Create a target that depends on the generated file.
add_custom_target(xdg-shell-protocol DEPENDS ${XDG_SHELL_PROTOCOL_H})

# This directory contains generated headers.
add_subdirectory(wlr)

#==============================================================================
# tinywl

add_executable(tinywl tinywl.c)
add_dependencies(tinywl xdg-shell-protocol)
target_link_libraries(tinywl PkgConfig::WaylandDeps) # PRIVATE ${AICXX_OBJECTS_LIST})

# Required define when using the unstable (git) version of wlroots.
target_compile_definitions(tinywl PUBLIC WLR_USE_UNSTABLE)

# This directory contains the generated xdg-shell-protocol.h header.
target_include_directories(tinywl PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

#==============================================================================
# tinywl++

add_executable(tinywl++ tinywl.cpp wlr/Scene.cxx)
add_dependencies(tinywl++ xdg-shell-protocol generated-events-headers)
target_link_libraries(tinywl++
  PRIVATE
    ${AICXX_OBJECTS_LIST}
    PkgConfig::WaylandDeps
)

if (OptionEnableLibcwd)
  target_sources(tinywl++ PRIVATE logging.cxx)
endif ()

# Required define when using the unstable (git) version of wlroots.
target_compile_definitions(tinywl++ PUBLIC WLR_USE_UNSTABLE)

target_include_directories(tinywl++
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}         # This directory contains the generated xdg-shell-protocol.h header.
    ${CMAKE_CURRENT_SOURCE_DIR}         # So that we can include "wl/Listener.h" from "wlr/events.h".
)

#==============================================================================
# WayArc

add_executable(WayArc WayArc.cxx)

if (OptionEnableLibcwd)
  target_sources(WayArc PRIVATE logging.cxx)
endif ()

add_dependencies(WayArc xdg-shell-protocol generated-events-headers)
target_link_libraries(WayArc
  PRIVATE
    ${AICXX_OBJECTS_LIST}
    PkgConfig::WaylandDeps
)

# Required define when using the unstable (git) version of wlroots.
target_compile_definitions(WayArc PUBLIC WLR_USE_UNSTABLE)

# This directory contains the generated xdg-shell-protocol.h header.
target_include_directories(WayArc PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

#==============================================================================
# cwformat

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_executable(cwformat cwformat.cpp)

# Include LLVM directories.
target_compile_definitions(cwformat PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(cwformat PRIVATE ${LLVM_INCLUDE_DIRS})

# Find the libraries that correspond to the LLVM components that we wish to use.
llvm_map_components_to_libnames(llvm_libs support)
# Link against LLVM libraries.
target_link_libraries(cwformat
  PRIVATE
    ${llvm_libs}
)
