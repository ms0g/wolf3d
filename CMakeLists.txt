cmake_minimum_required(VERSION 3.23)
project(wolf3d CXX C)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB_RECURSE SRC "src/*")

SET(DEPS upng/upng.c upng/upng.h)

add_executable(${PROJECT_NAME} ${SRC} ${DEPS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
