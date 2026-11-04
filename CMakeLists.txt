cmake_minimum_required(VERSION 3.14)
project(image_manips LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
option(BUILD_TESTS "Build tests for the image manips functions (off by default)" OFF)

FILE(GLOB PROJECT_SOURCES libs/*.cpp)
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE libs/include)

function(create_processor_lvl count folder prefix)
    add_executable(${prefix}my_image_processor_lvl${count} ${folder}/${prefix}level${count}.cpp)
    target_include_directories(${prefix}my_image_processor_lvl${count} PRIVATE libs/include)
    target_link_libraries(${prefix}my_image_processor_lvl${count} PRIVATE ${PROJECT_NAME})
endfunction()

set(levels 1 2 3)
foreach(X IN LISTS levels)
    message(STATUS "Generating level ${X}")
    create_processor_lvl(${X} src "")
    create_processor_lvl(${X} examples "example_")
endforeach()

if(BUILD_TESTS)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(
        image_testing
        test/tests.cpp
        libs/image_manips.cpp
    )
    target_include_directories( 
        image_testing
        PRIVATE libs/include
    )
    target_link_libraries(
        image_testing
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(image_testing)
endif()