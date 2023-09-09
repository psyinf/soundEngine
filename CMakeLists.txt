cmake_minimum_required(VERSION 3.22)
project(soundengine)

set(CODE_COVERAGE ON)

#add cmake script to downlad conan-cmake integrations
include(cmake/conan_config.cmake)
include(cmake/PreventInSourceBuilds.cmake)
#TODO put into cmake file
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
include(cmake/cmake-coverage.cmake)
else ()
message("No code coverage for ${CMAKE_CXX_COMPILER_ID}")
function (add_code_coverage)
endfunction ()

function (target_code_coverage TARGET)
endfunction()

endif()


INCLUDE (CheckIncludeFiles)
SET(CMAKE_CXX_STANDARD 23)
CHECK_INCLUDE_FILES ("stacktrace" USE_STACKTRACES)
add_definitions(-DUSE_STACKTRACES=${USE_STACKTRACES})

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
