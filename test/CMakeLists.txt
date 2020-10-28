include_directories(${PROJECT_SOURCE_DIR}/search_files_continuously ${PROJECT_SOURCE_DIR}/sort)
file(GLOB sources "${PROJECT_SOURCE_DIR}/search_files_continuously/*.c")
file(GLOB sources "${PROJECT_SOURCE_DIR}/sort/*.c")
file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.c")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/main.c")

file(GLOB tests "${PROJECT_SOURCE_DIR}/test/*.cpp")
list(REMOVE_ITEM tests "${PROJECT_SOURCE_DIR}/test/main.cpp")

foreach(file ${tests})
    set(name)
    get_filename_component(name ${file} NAME_WE)
    add_executable("${name}_tests"
            ${sources}
            ${file}
            "${PROJECT_SOURCE_DIR}/test/main.cpp")
    target_link_libraries("${name}_tests" gtest_main search_files_lib)
    add_test(NAME ${name} COMMAND "${name}_tests")
endforeach()
