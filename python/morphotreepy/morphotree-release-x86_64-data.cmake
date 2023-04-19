########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(morphotree_COMPONENT_NAMES "")
set(morphotree_FIND_DEPENDENCY_NAMES "")

########### VARIABLES #######################################################################
#############################################################################################
set(morphotree_PACKAGE_FOLDER_RELEASE "/home/dennis/.conan2/p/morphb75acff3f35fd/p")
set(morphotree_BUILD_MODULES_PATHS_RELEASE )


set(morphotree_INCLUDE_DIRS_RELEASE "${morphotree_PACKAGE_FOLDER_RELEASE}/include")
set(morphotree_RES_DIRS_RELEASE )
set(morphotree_DEFINITIONS_RELEASE )
set(morphotree_SHARED_LINK_FLAGS_RELEASE )
set(morphotree_EXE_LINK_FLAGS_RELEASE )
set(morphotree_OBJECTS_RELEASE )
set(morphotree_COMPILE_DEFINITIONS_RELEASE )
set(morphotree_COMPILE_OPTIONS_C_RELEASE )
set(morphotree_COMPILE_OPTIONS_CXX_RELEASE )
set(morphotree_LIB_DIRS_RELEASE "${morphotree_PACKAGE_FOLDER_RELEASE}/lib")
set(morphotree_BIN_DIRS_RELEASE "${morphotree_PACKAGE_FOLDER_RELEASE}/bin")
set(morphotree_LIBRARY_TYPE_RELEASE SHARED)
set(morphotree_IS_HOST_WINDOWS_RELEASE 0)
set(morphotree_LIBS_RELEASE morphotree)
set(morphotree_SYSTEM_LIBS_RELEASE )
set(morphotree_FRAMEWORK_DIRS_RELEASE )
set(morphotree_FRAMEWORKS_RELEASE )
set(morphotree_BUILD_DIRS_RELEASE )
set(morphotree_NO_SONAME_MODE_RELEASE FALSE)


# COMPOUND VARIABLES
set(morphotree_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${morphotree_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${morphotree_COMPILE_OPTIONS_C_RELEASE}>")
set(morphotree_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${morphotree_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${morphotree_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${morphotree_EXE_LINK_FLAGS_RELEASE}>")


set(morphotree_COMPONENTS_RELEASE )