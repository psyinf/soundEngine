cmake_minimum_required(VERSION 3.15...3.23)
project(tests LANGUAGES CXX)
find_package(Catch2 REQUIRED)
enable_testing()

add_executable(${PROJECT_NAME} tests.cpp)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE 
          Catch2::Catch2WithMain)

          
include(CTest)
include(Catch)
target_code_coverage(${PROJECT_NAME})

catch_discover_tests(
  ${PROJECT_NAME}
  TEST_PREFIX "unittests."
  REPORTER XML
  OUTPUT_DIR .
  OUTPUT_PREFIX "unittests."
  OUTPUT_SUFFIX .xml
)
