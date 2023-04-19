# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(morphotree_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(morphotree_FRAMEWORKS_FOUND_RELEASE "${morphotree_FRAMEWORKS_RELEASE}" "${morphotree_FRAMEWORK_DIRS_RELEASE}")

set(morphotree_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET morphotree_DEPS_TARGET)
    add_library(morphotree_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET morphotree_DEPS_TARGET
             PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${morphotree_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${morphotree_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:>
             APPEND)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### morphotree_DEPS_TARGET to all of them
conan_package_library_targets("${morphotree_LIBS_RELEASE}"    # libraries
                              "${morphotree_LIB_DIRS_RELEASE}" # package_libdir
                              "${morphotree_BIN_DIRS_RELEASE}" # package_bindir
                              "${morphotree_LIBRARY_TYPE_RELEASE}"
                              "${morphotree_IS_HOST_WINDOWS_RELEASE}"
                              morphotree_DEPS_TARGET
                              morphotree_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "morphotree"    # package_name
                              "${morphotree_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${morphotree_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Release ########################################
    set_property(TARGET morphotree::morphotree
                 PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Release>:${morphotree_OBJECTS_RELEASE}>
                 $<$<CONFIG:Release>:${morphotree_LIBRARIES_TARGETS}>
                 APPEND)

    if("${morphotree_LIBS_RELEASE}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET morphotree::morphotree
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     morphotree_DEPS_TARGET
                     APPEND)
    endif()

    set_property(TARGET morphotree::morphotree
                 PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Release>:${morphotree_LINKER_FLAGS_RELEASE}> APPEND)
    set_property(TARGET morphotree::morphotree
                 PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Release>:${morphotree_INCLUDE_DIRS_RELEASE}> APPEND)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET morphotree::morphotree
                 PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Release>:${morphotree_LIB_DIRS_RELEASE}> APPEND)
    set_property(TARGET morphotree::morphotree
                 PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Release>:${morphotree_COMPILE_DEFINITIONS_RELEASE}> APPEND)
    set_property(TARGET morphotree::morphotree
                 PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Release>:${morphotree_COMPILE_OPTIONS_RELEASE}> APPEND)

########## For the modules (FindXXX)
set(morphotree_LIBRARIES_RELEASE morphotree::morphotree)
