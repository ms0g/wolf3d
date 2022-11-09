cmake_minimum_required(VERSION 3.23)
project(cube C CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB_RECURSE SRC "src/*")
file(GLOB_RECURSE LIB "lib/*")

add_executable(${PROJECT_NAME} ${SRC} ${LIB})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
