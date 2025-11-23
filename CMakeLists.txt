# CMakeLists.txt --- CMake project settings
##############################################################################

# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# enable testing
enable_testing()

# project name and languages
project(onigpp C CXX)

if (MSVC)
    # replace "/MD" with "/MT" (building without runtime DLLs)
    set(CompilerFlags
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    foreach(CompilerFlags ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlags} "${${CompilerFlags}}")
    endforeach()
endif()

# We want flat layout
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

##############################################################################

# oniguruma
add_subdirectory(oniguruma)

# libonigpp.a
add_library(onigpp STATIC src/onigpp.cpp)
target_include_directories(onigpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/oniguruma/src)
target_link_libraries(onigpp PUBLIC onig)

# tests
add_subdirectory(tests)

# CTest tests
add_test(
    NAME test1
    COMMAND $<TARGET_FILE:onigpp_test>
    CONFIGURATIONS Debug
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
add_test(
    NAME test2
    COMMAND $<TARGET_FILE:onigpp_testw>
    CONFIGURATIONS Debug
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

##############################################################################
