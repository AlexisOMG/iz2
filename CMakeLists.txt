cmake_minimum_required(VERSION 3.16)
project(iz2)

include(ExternalProject)
find_package(Git REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic")


# Add static lib sort and search_files

add_library(sort_static_lib STATIC sort/implementation_sort.c)
add_library(search_files_static_lib STATIC search_files_continuously/implementation_search_files.c)


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

##
### End test definition ###
##

##
### Code coverage ###
##

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    include(CodeCoverage)
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_html(
            NAME coverage
            EXECUTABLE valgrind -q ctest
            BASE_DIRECTORY "${PROJECT_SOURCE_DIR}"
            EXCLUDE "build/*" "test/*" "src/main.c" "set_test_files.py")
endif()

##
### End code coverage ###
##

##
### Cppcheck ###
##

if(ENABLE_CPPCHECK)

    list(APPEND CPPCHECK_CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
            )

    ExternalProject_Add(
            cppcheck
            GIT_REPOSITORY      https://github.com/danmar/cppcheck.git
            GIT_TAG             1.79
            GIT_SHALLOW         1
            CMAKE_ARGS          ${CPPCHECK_CMAKE_ARGS}
            PREFIX              ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
            TMP_DIR             ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
            STAMP_DIR           ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
            DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/cppcheck/download
            SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/src
            BINARY_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/build
    )

    list(APPEND CPPCHECK_ARGS
            --enable=warning,style,performance,portability,unusedFunction
            --std=c11
            --std=c++11
            --verbose
            --error-exitcode=1
            --language=c
            -DMAIN=main
            -I ${CMAKE_SOURCE_DIR}/sort
            ${CMAKE_SOURCE_DIR}/search_files_continuously
            ${CMAKE_SOURCE_DIR}/sort/*.h
            ${CMAKE_SOURCE_DIR}/search_files_continuously/*.h
            ${CMAKE_SOURCE_DIR}/src/*.c
            )

    add_custom_target(
            check
            COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS}
            COMMENT "running cppcheck"
    )

endif()

##
### End cppcheck ###
##

add_executable(iz2 src/main.c)
target_include_directories(iz2 PUBLIC search_files_continuously)
target_include_directories(sort_static_lib PUBLIC sort)
target_include_directories(search_files_static_lib PUBLIC search_files_continuously sort)
target_link_libraries(search_files_static_lib PRIVATE sort_static_lib gcov)
target_link_libraries(iz2 PRIVATE search_files_static_lib)
