cmake_minimum_required(VERSION 3.22)
project(soundengine)

set(CODE_COVERAGE ON)

#add cmake script to downlad conan-cmake integrations
include(cmake/conan_config.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/configure_analysis.cmake)


SET(CMAKE_CXX_STANDARD 23)
#setup conditional compilation of advanced compiler features
INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES ("stacktrace" USE_STACKTRACES)
if(USE_STACKTRACES)
add_definitions(-DUSE_STACKTRACES=${USE_STACKTRACES})
endif()
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
				    SETTINGS ${settings})

install(DIRECTORY
		data
	DESTINATION
		bin
	COMPONENT
		${RUNTIME_COMPONENT}
)


add_subdirectory(soundengine)
add_subdirectory(sandbox)
