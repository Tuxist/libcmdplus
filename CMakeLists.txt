project(libcmdplus)
cmake_minimum_required(VERSION 3.25)


set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} \
    -fPIC \
    -fsanitize=address "
)

set(LIBV "1")
set(Upstream_VERSION 1.0.0)

add_subdirectory(src)
