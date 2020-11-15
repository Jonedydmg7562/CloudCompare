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


# CloudCompare example for standard plugins

# REPLACE ALL 'DeleteIsland' OCCURENCES BY YOUR PLUGIN NAME
# AND ADAPT THE CODE BELOW TO YOUR OWN NEEDS!

# Add an option to CMake to control whether we build this plugin or not
option( INSTALL_DELETE_ISLAND "Check to install example plugin" OFF )

if ( INSTALL_DELETE_ISLAND )
	# Name the plugin
    project( DeleteIsland )
    
    # load any subdirectories (see qAdditionalIO for an example)
    # add_subdirectory( LIB1 )
    
    include( ../../CMakePluginTpl.cmake )
    
    # set dependencies to necessary libraries (see qPCV for an example)
    # target_link_libraries( ${PROJECT_NAME} LIB1 )
    # include_directories( ${LIB1_INCLUDE_DIR} )
endif()

