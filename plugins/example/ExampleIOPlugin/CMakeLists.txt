cmake_minimum_required( VERSION 3.0 )

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

option( INSTALL_EXAMPLE_IO_PLUGIN "Check to install example I/O plugin" OFF )

if (INSTALL_EXAMPLE_IO_PLUGIN)
	project( ExampleIOPlugin )
	
	# Set to include this I/O plugin in ccViewer
	set( CC_IS_IO_PLUGIN 1 )

	include_directories( src )
	
	add_subdirectory( src )
	
	include( ../../CMakePluginTpl.cmake )    
endif()

