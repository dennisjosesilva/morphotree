cmake_minimum_required(VERSION 3.20)

project(morphotree)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_APPS OFF CACHE BOOL "Enable application buildings")
set(BUILD_TESTS OFF CACHE BOOL "Enable test building")

include_directories(include)
include_directories(stb)

find_package(OpenMP REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


list(REMOVE_ITEM PROJECT_SOURCE ${PROJECT_MAIN})

add_library(morphotreelib STATIC ${PROJECT_SOURCE})
target_link_libraries(morphotreelib OpenMP::OpenMP_CXX)


if(BUILD_APPS)
    set(PROJECT_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    add_executable(morphotreemain ${PROJECT_MAIN})
    target_link_libraries(morphotreemain morphotreelib)
endif()

if(BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0
    )

    FetchContent_MakeAvailable(Catch2)

    add_executable(tests tests/src/Test_Box.cpp)
    target_link_libraries(tests PRIVATE morphotreelib Catch2::Catch2WithMain)
endif()


