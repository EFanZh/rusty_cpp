# https://cmake.org/cmake/help/latest/policy/CMP0092.html
cmake_minimum_required(VERSION 3.15)
project(rusty_cpp)

include(FetchContent)
include(GoogleTest)

# Configuration.

# See https://github.com/microsoft/GSL/blob/master/CMakeLists.txt.

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(STANDALONE_PROJECT ON)
else()
    set(STANDALONE_PROJECT OFF)
endif()

# Add Google Test dependency.

# https://cmake.org/cmake/help/latest/module/FetchContent.html

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
    GIT_SHALLOW ON
    GIT_PROGRESS ON
)

if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

# Compile options.

if(MSVC)
    set(CMAKE_INCLUDE_SYSTEM_FLAG_C "/external:I")
    set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "/external:I")
endif()

# Configure library.

add_library(rusty_cpp INTERFACE)

if(STANDALONE_PROJECT)
    target_include_directories(rusty_cpp INTERFACE include)
else()
    target_include_directories(rusty_cpp SYSTEM INTERFACE include)
endif()

# Configure test.

if(STANDALONE_PROJECT)
    enable_testing()

    add_executable(
        rusty_cpp_tests
        "tests/rusty_cpp/iter/enumerate_tests.cpp"
        "tests/rusty_cpp/iter/iterable_tests.cpp"
        "tests/rusty_cpp/iter/map_tests.cpp"
        "tests/rusty_cpp/iter/utilities_tests.cpp"
        "tests/rusty_cpp/iter_tests.cpp"
    )

    set_target_properties(rusty_cpp_tests
                          PROPERTIES CXX_EXTENSIONS OFF
                                     CXX_STANDARD 17
                                     CXX_STANDARD_REQUIRED ON)

    if(MSVC)
        target_compile_options(
            rusty_cpp_tests
            PRIVATE
            "/W4"
            "/we4287" # 'operator': unsigned/negative constant mismatch
            "/we4365" # 'action': conversion from 'type_1' to 'type_2', signed/unsigned mismatch
            "/we4388" # signed/unsigned mismatch
            "/we4826" # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
            "/WX"
            "/experimental:external"
            "/external:W0"
        )
    else()
        target_compile_options(rusty_cpp_tests PRIVATE "-Wall" "-Wextra" "-Wpedantic" "-Wconversion" "-Werror")
    endif()

    target_link_libraries(rusty_cpp_tests PRIVATE rusty_cpp gtest_main)

    gtest_discover_tests(rusty_cpp_tests)
endif()
