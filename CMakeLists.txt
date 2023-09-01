cmake_minimum_required(VERSION 3.20)
project(soundengine)
set(CODE_COVERAGE ON)
#add cmake script to downlad conan-cmake integrations
include(cmake/conan_config.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/cmake-coverage.cmake)

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
