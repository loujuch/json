cmake_minimum_required(VERSION 3.5)

project(Json)

set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)

foreach(test_file IN LISTS TEST_SOURCES)
	get_filename_component(test_program ${test_file} NAME_WE ABSOLUTE)
	add_executable(${test_program} ${test_file})
endforeach(test_file)
