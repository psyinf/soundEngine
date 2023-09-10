project(soundEngine LANGUAGES CXX)
include(GNUInstallDirs)
add_code_coverage()
find_package(OpenAL REQUIRED)
find_package(ALSA)
if (ALSA_FOUND)
    set(ADDITIONAL_LIBS ALSA::ALSA)
endif()
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} 
    PRIVATE
        "SoundEngine.h"
        "SoundEngine.cpp"
        "ALHelpers.h" 
        "LoadWave.h" 
        "Source.h" "Source.cpp" 
         
        "Types.h" 
        "Loader.h" "Loader.cpp" 
        "Buffer.h" "Buffer.cpp" "ErrorTrace.h")

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        OpenAL::OpenAL
        ${ADDITIONAL_LIBS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
    LINKER_LANGUAGE CXX
    CXX_STANDARD 23
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    DEBUG_POSTFIX "d"
)
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
    EXPORT ${PROJECT_NAME}-config
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

target_code_coverage(${PROJECT_NAME})
#install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}/
 #       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
