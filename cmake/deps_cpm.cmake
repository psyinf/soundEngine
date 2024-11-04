include (cmake/setup_cpm.cmake)

CPMAddPackage(
    NAME Catch2

    GITHUB_REPOSITORY "catchorg/Catch2"
    GIT_TAG v3.5.2
     OPTIONS
    "CATCH_BUILD_TESTING OFF"
    "CATCH_BUILD_EXAMPLES OFF"
    "CATCH_BUILD_EXTRA_TESTS OFF"
    "CATCH_BUILD_FUZZERS OFF"
)

CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 10.2.1
    OPTIONS
    "FMT_DOC OFF"
    "FMT_TEST OFF"
    "FMT_INSTALL OFF"
    "FMT_PEDANTIC OFF"
    "FMT_WERROR OFF"
    "FMT_USE_CCACHE OFF"
    "FMT_USE_FIND_PACKAGE OFF"
    "FMT_USE_CONFIG OFF"
    "FMT_USE_PKG_CONFIG OFF"
    "FMT_USE_CONAN OFF"
    "FMT_USE_VCPKG OFF"
    "FMT_USE_ASAN OFF"
    "FMT_USE_UBSAN OFF"
    "FMT_USE_MSAN OFF"
    "FMT_USE_TSAN OFF"
    "FMT_USE_CODE_ANALYSIS OFF"
    "FMT_USE_CPPCHECK OFF"
)


CPMAddPackage(
    NAME spdlog

    GITHUB_REPOSITORY gabime/spdlog
    GIT_TAG v1.13.0
    OPTIONS
    "SPDLOG_BUILD_EXAMPLES OFF"
    "SPDLOG_BUILD_TESTS OFF"
    "SPDLOG_BUILD_BENCH OFF"
    "SPDLOG_BUILD_SHARED OFF"
    "SPDLOG_FMT_EXTERNAL ON"
)

CPMAddPackage(
    NAME OpenAL
    GITHUB_REPOSITORY kcat/openal-soft
    GIT_TAG 1.23.1

    OPTIONS
    "ALSOFT_UTILS OFF"
    "ALSOFT_EXAMPLES OFF"
    "ALSOFT_TESTS OFF"
    "ALSOFT_INSTALL OFF"
    "ALSOFT_CONFIG OFF"
)


CPMAddPackage(
    NAME pgf
    GITHUB_REPOSITORY psyinf/pgfoundation
    GIT_TAG features
)

CPMAddPackage(
    NAME minimp3
    GITHUB_REPOSITORY lieff/minimp3
   
    GIT_TAG master
)
if(minimp3_ADDED)
add_library(minimp3 INTERFACE)
target_sources(minimp3 
    PUBLIC
    ${minimp3_SOURCE_DIR}/minimp3.h
    ${minimp3_SOURCE_DIR}/minimp3_ex.h

)
target_include_directories(minimp3 INTERFACE ${minimp3_SOURCE_DIR})
add_library(minimp3::minimp3 ALIAS minimp3)
endif()

CPMAddPackage(
    NAME scope_lite
    GITHUB_REPOSITORY martinmoene/scope-lite
    GIT_TAG v0.2.0
)


if (NOT_SUBPROJECT AND NOT ($ENV{GITHUB_ACTIONS}))
CPMAddPackage(
    NAME data
    GITHUB_REPOSITORY psyinf/resources-sound
    GIT_TAG main

    OPTIONS "INSTALL_PATH ${CMAKE_INSTALL_BINDIR}"
)
endif()
