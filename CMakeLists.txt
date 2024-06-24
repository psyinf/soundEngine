cmake_minimum_required(VERSION 3.22)
project(soundengine)
set(CMAKE_CXX_STANDARD 23)  
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CODE_COVERAGE ON)

include(cmake/PreventInSourceBuilds.cmake)
include(cmake/configure_analysis.cmake)
include(cmake/stacktrace.cmake)
include(cmake/setup_cpm.cmake)
include(cmake/deps_cpm.cmake)

add_subdirectory(soundengine)
add_subdirectory(sandbox)

install(DIRECTORY
		data
	DESTINATION
		bin
	COMPONENT
		${RUNTIME_COMPONENT}
)


