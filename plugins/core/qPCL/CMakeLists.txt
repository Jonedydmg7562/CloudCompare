cmake_minimum_required(VERSION 3.0)

#设置输出目录
if( WIN32 )
	#set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${BITS}")
	#set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${BITS}")
	#自动添加release和debug目录
	# set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
	# set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
	#自己设定特定目录
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/plugins")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  "${CMAKE_BINARY_DIR}/Release/plugins")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${CMAKE_BINARY_DIR}/Release/plugins")
	
elseif( UNIX )
	set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${BITS}/${CMAKE_BUILD_TYPE}/Release/plugins")
	set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${BITS}/${CMAKE_BUILD_TYPE}/Release/plugins")
endif()

option( INSTALL_QPCL_PLUGIN "Check to install qPCL plugin" OFF )

# CloudCompare 'PCL' bridge plugin
if (INSTALL_QPCL_PLUGIN)
    #requires PCL
    find_package(PCL REQUIRED)
    
    #PCL bridge library
    add_subdirectory(PclUtils)
    
    #CloudCompare 'qPCLIO' I/O plugin (to load PCD files)
    add_subdirectory(PclIO)
    
    project( QPCL_PLUGIN )
    
    #documentation
    add_subdirectory(doc)
    
    include_directories( ${QPCL_PLUGIN_UTILS_LIB_SOURCE_DIR}/filters )
    include_directories( ${QPCL_PLUGIN_UTILS_LIB_BINARY_DIR} )
    include_directories( ${PCL_INCLUDE_DIRS} )
    
    include( ../../CMakePluginTpl.cmake )
    
    target_link_libraries(${PROJECT_NAME} QPCL_PLUGIN_UTILS_LIB)
    
    link_directories( ${PCL_LIBRARY_DIRS} )
    add_definitions( ${PCL_DEFINITIONS} )
    
    #import PCL dlls (if any, WIN32 only)
    include( ExportPCLDlls.cmake )
    export_PCL_dlls( ${CLOUDCOMPARE_DEST_FOLDER} )
endif()

