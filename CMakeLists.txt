# FetchContent was added to 3.14.
cmake_minimum_required(VERSION 3.14...3.31)

project(WayArc
  LANGUAGES C CXX
  DESCRIPTION "Wayland Compositor inspired by fluxbox."
)

# Compile all subdirectories with the same standard.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#==============================================================================
# Begin of gitache configuration.

set(GITACHE_PACKAGES libcwd_r)

# Is libfarmhash installed somewhere in a default path?
#find_library(
#  FARMHASH farmhash
#DOC "Whether or not libfarmhash is installed locally."
#)
# If not, use gitache to get it.
#if(NOT FARMHASH)
#  list(APPEND GITACHE_PACKAGES farmhash)
#endif()

include(cwm4/cmake/StableGitache)
# End of gitache configuration.

#==============================================================================
# This project uses aicxx modules.
include(cwm4/cmake/AICxxProject NO_POLICY_SCOPE)

#==============================================================================
# OPTIONS

# End of OPTIONS section.

#==============================================================================

# Exit if someone tries to contaminate the source directory with an in-source build.
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Please do out-of-source builds."
                      "Cleanup: \"rm -rf CMake CMakeCache.txt CMakeFiles/\"")
endif ()

#==============================================================================
# CONFIG FILES

# Tell cwds that we want to use another namespace.
set(NamespaceDebug debug_cwds)

# Tell cwds/sys.h that we have a config.h.
#add_definitions(-DHAVE_CONFIG_H)
#include_directories(${top_objdir})
# End of CONFIG FILES section.

#==============================================================================

# Declare aicxx submodules.
include(AICxxSubmodules)

# Extra clean up before removing directories.
add_custom_target(WayArc-maintainer-clean-extra
  COMMAND rm -f CMakeDoxygenDefaults.cmake CMakeDoxyfile.in
)

# Get pkg_check_modules and pkg_get_variable.
find_package(PkgConfig REQUIRED)

#==============================================================================
# Check for required dependencies.
pkg_check_modules(WaylandDeps REQUIRED IMPORTED_TARGET "wlroots-0.19" wayland-server xkbcommon pixman-1)

#add_subdirectory(filelock-task)
#add_subdirectory(resolver-task)
#add_subdirectory(socket-task)
#add_subdirectory(xmlrpc-task)
#add_subdirectory(dbus-task)
#add_subdirectory(xcb-task)
#add_subdirectory(block-task)

add_subdirectory(src)
add_subdirectory(tests)
