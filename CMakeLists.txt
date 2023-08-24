cmake_minimum_required(VERSION 3.20)
project(sagan)

#add cmake script to downlad conan-cmake integrations
include(cmake/conan_config.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/coverage.cmake)

conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
				    SETTINGS ${settings})

install(DIRECTORY
		data
	DESTINATION
		${CMAKE_INSTALL_BINDIR}
	COMPONENT
		${RUNTIME_COMPONENT}
)

enable_testing()
add_subdirectory(src)
add_subdirectory(test)