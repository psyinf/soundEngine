project(soundEngine LANGUAGES CXX)
include(GNUInstallDirs)
add_code_coverage()
find_package(OpenAL REQUIRED)
find_package(ALSA)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(minimp3 REQUIRED)
#find_package(scope-lite REQUIRED)

if (ALSA_FOUND)
    set(ADDITIONAL_LIBS ALSA::ALSA)
endif()
add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} 
    PRIVATE
        "src/SoundEngine.cpp"
        "src/Source.cpp" 
        "src/Loader.cpp" 
        "src/loaders/LoadWave.cpp"
        "src/Buffer.cpp" 
        "src/BackgroundPlayerThreaded.cpp"
        "src/TaskBasedPlayer.cpp"
        "src/loaders/LoadMp3.cpp"

    PUBLIC
        "include/sndX/SoundEngine.hpp"
        "include/sndX/ALHelpers.hpp" 
        
        "include/sndX/Source.hpp" 
        "include/sndX/Loader.hpp" 
        "include/sndX/Types.hpp" 
        "include/sndX/Buffer.hpp"
        "include/sndX/ErrorTrace.hpp"
        "include/sndX/BackgroundPlayerInterface.hpp"
        "include/sndX/BackgroundPlayerThreaded.hpp"
        "include/sndX/BackgroundPlayer.hpp"
    
    PRIVATE        
        "src/loaders/LoadMp3.hpp"
        "src/loaders/LoadWave.hpp" 
)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        OpenAL::OpenAL
        spdlog::spdlog
        minimp3::minimp3
        nonstd::scope-lite
        ${ADDITIONAL_LIBS}
    PRIVATE
        fmt::fmt
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
    LINKER_LANGUAGE CXX
    CXX_STANDARD 23
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    DEBUG_POSTFIX "d"
)
add_compile_definitions(${PROJECT_NAME} USE_SPDLOG ON)
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

target_code_coverage(${PROJECT_NAME})
#install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}/
 #       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
