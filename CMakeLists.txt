project(libcmdplus)
cmake_minimum_required(VERSION 3.25)

include(CMakePackageConfigHelpers)

set(ConfigPackageLocation lib/cmake/libcmdplus)

if(NOT ${CMAKE_CXX_COMPILER} MATCHES "MSVC")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} \
    -fPIC \
    -Wall "
)
endif()

set(LIBV "1")
set(Upstream_VERSION 1.0.0)

configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/libcmdplusConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/libcmdplusConfig.cmake"
    INSTALL_DESTINATION ${ConfigPackageLocation}
)

add_subdirectory(src)
