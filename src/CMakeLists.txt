include(AICxxProject)

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

# Check for required dependencies.
pkg_check_modules(tinywl_deps REQUIRED IMPORTED_TARGET "wlroots-0.19" wayland-server xkbcommon)

# Add custom command to generate XDG_SHELL_PROTOCOL_H.
set(XDG_SHELL_PROTOCOL_H "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.h")
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

add_executable(tinywl tinywl.c)
add_dependencies(tinywl xdg-shell-protocol)
target_link_libraries(tinywl PkgConfig::tinywl_deps) # PRIVATE ${AICXX_OBJECTS_LIST})

# Required define when using the unstable (git) version of wlroots.
target_compile_definitions(tinywl PUBLIC WLR_USE_UNSTABLE)

# This directory contains the generated xdg-shell-protocol.h header.
target_include_directories(tinywl PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
