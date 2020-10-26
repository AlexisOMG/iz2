cmake_minimum_required(VERSION 3.16)
project(iz2)

include(ExternalProject)
find_package(Git REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic")


add_library(sort_static_lib STATIC sort/implementation_sort.c)
add_library(search_files_static_lib STATIC search_files/implementation_search_files.c)

##
### Test definitions ###
##

configure_file(CMakeList.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

enable_testing()
add_subdirectory(test)

add_executable(iz2 src/main.c)
target_include_directories(iz2 PUBLIC sort search_files)
target_include_directories(sort_static_lib PUBLIC sort)
target_include_directories(search_files_static_lib PUBLIC search_files)
target_link_libraries(search_files_static_lib PRIVATE sort_static_lib)
target_link_libraries(iz2 PRIVATE search_files_static_lib)