
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(test_toolbox test_toolbox.cpp)

set_target_properties(test_toolbox PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(test_toolbox
    ${Boost_LIBRARIES}
    toolbox
)

add_test(test_toolbox test_toolbox)
