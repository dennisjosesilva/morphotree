project(morphotree)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

include_directories(../include)
file(GLOB_RECURSE PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE PROJECT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.hpp")

set(PROJECT_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
list(REMOVE_ITEM PROJECT_SOURCE ${PROJECT_MAIN})

add_library(morphotree ${PROJECT_SOURCE})
target_include_directories(morphotree PUBLIC ../include)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/../include"
        DESTINATION "${CMAKE_INSTALL_PREFIX}"
        FILES_MATCHING PATTERN "*.hpp")
install(TARGETS morphotree)