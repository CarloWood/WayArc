include(AICxxProject)

# Test for wl::SignalServer.
add_executable(signal-server signal-server.cxx)
target_include_directories(signal-server PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(signal-server
  PRIVATE
    ${AICXX_OBJECTS_LIST}
    PkgConfig::WaylandDeps
)
