cmake_minimum_required(VERSION 3.8)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(PROJECT_BUILD_NUMBER $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(PROJECT_BUILD_NUMBER "45")
endif()

project(bulk VERSION 1.2.${PROJECT_BUILD_NUMBER})

# global
# set_property() - not work
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("-Wpedantic;-Wall;-Wextra")


# lib
add_subdirectory("./toolbox")


# app
add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(${PROJECT_NAME}
    toolbox
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)


# packing
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT xyz@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
