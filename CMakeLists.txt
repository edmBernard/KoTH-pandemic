project(koth-snake)

#**************************************************************************************************
# General cMake settings **************************************************************************
cmake_minimum_required(VERSION 3.5)

#**************************************************************************************************
# Find Package ************************************************************************************
find_package(cxxopts REQUIRED)
find_package(fmt CONFIG REQUIRED)


#**************************************************************************************************
# Include *****************************************************************************************
include_directories(${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/bots)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

#**************************************************************************************************
# Set compiler ************************************************************************************
set(CMAKE_CXX_FLAGS "-fPIC -fpermissive -std=c++17")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#**************************************************************************************************
# Set variable ************************************************************************************
SET(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/bots/basic/basic.cpp
)


#**************************************************************************************************
# Make configuration ******************************************************************************
add_executable(koth-pandemic ${SOURCES})
set_property(TARGET koth-pandemic PROPERTY CXX_STANDARD 17)
target_link_libraries(koth-pandemic -pthread -lstdc++fs fmt::fmt)

#**************************************************************************************************
# Build Tests *************************************************************************************
# option(BUILD_UNIT_TESTS "Build the unit tests" ON)
#   if(BUILD_UNIT_TESTS)
#   enable_testing()

#   SET(TEST_SOURCES
#     ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-main.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-board.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-states.cpp
#   )

#   add_executable(unit-test ${TEST_SOURCES} ${SOURCES})
#   set_property(TARGET unit-test PROPERTY CXX_STANDARD 17)
#   target_link_libraries(unit-test PRIVATE -pthread -lstdc++fs)

#   add_test(NAME "unit_test_default"
#     COMMAND unit-test
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#   )
# endif()

#**************************************************************************************************
# Build Benchmark *********************************************************************************
# option(BUILD_BENCHMARK "Build the benchmark" ON)
# if(BUILD_BENCHMARK)
#   enable_testing()

#   SET(BENCHMARK_SOURCES
#     ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/main.cpp
#   )

#   add_executable(benchmark ${BENCHMARK_SOURCES} ${SOURCES})
#   set_property(TARGET benchmark PROPERTY CXX_STANDARD 17)
#   target_link_libraries(benchmark PRIVATE -pthread -lstdc++fs)

#   add_test(NAME "benchmark_default"
#     COMMAND benchmark
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#   )
# endif()
