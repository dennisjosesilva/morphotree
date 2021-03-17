cmake_minimum_required(VERSION 3.12)

project(morphotreepy)

# include(../morphotree.cmake)

include_directories("include")

# add_definitions("-std=c++17")

set(CMAKE_CXX_STANDARD 14 CACHE STRING "C++ version selection")  # or 11, 14, 17, 20
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # optional, ensure standard is supported
set(CMAKE_CXX_EXTENSIONS OFF)  # optional, keep compiler extensionsn off

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(pybind11 REQUIRED)

#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#    add_compile_options(-Wnoc++11-narrowing )
#endif()

pybind11_add_module(morphotreepy 
  src/pointpy.cpp  
  src/ctbuilderpy.cpp 
  src/mtreepy.cpp
  src/core/hqueuepy.cpp
  src/tree/treeOfShapes/kgridpy.cpp
  src/tree/treeOfShapes/order_imagepy.cpp
  src/tree/treeOfShapes/tospy.cpp
  src/attributes/attributeComputerpy.cpp
  src/attributes/bitquads/quadCountComputerpy.cpp
  src/attributes/bitquads/quadCountTreeOfShapesComputerpy.cpp
  src/attributes/smoothnessContourComputerpy.cpp
  src/module.cpp)
  
target_link_libraries(morphotreepy PRIVATE ${CONAN_LIBS})