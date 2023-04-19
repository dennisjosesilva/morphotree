########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(morphotree_FIND_QUIETLY)
    set(morphotree_MESSAGE_MODE VERBOSE)
else()
    set(morphotree_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/morphotreeTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${morphotree_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(morphotree_VERSION_STRING "0.0.1")
set(morphotree_INCLUDE_DIRS ${morphotree_INCLUDE_DIRS_RELEASE} )
set(morphotree_INCLUDE_DIR ${morphotree_INCLUDE_DIRS_RELEASE} )
set(morphotree_LIBRARIES ${morphotree_LIBRARIES_RELEASE} )
set(morphotree_DEFINITIONS ${morphotree_DEFINITIONS_RELEASE} )

# Only the first installed configuration is included to avoid the collision
foreach(_BUILD_MODULE ${morphotree_BUILD_MODULES_PATHS_RELEASE} )
    message(${morphotree_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


