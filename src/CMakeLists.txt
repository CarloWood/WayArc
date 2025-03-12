include(AICxxProject)

add_executable(hello_test hello_test.cxx)
target_link_libraries(hello_test PRIVATE ${AICXX_OBJECTS_LIST})
