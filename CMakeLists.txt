cmake_minimum_required(VERSION 3.5)

project(flygon C CXX)

enable_testing()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    # -O2 -g -DNDEBUG
    message("Setting build type to 'RelWithDebInfo' as none was specified")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

if(CMAKE_PROJECT_NAME STREQUAL "flygon")
  option(FLYGON_BUILD_EXAMPLES "Build flygon examples" ON)
  message("tests")
endif()

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(flygon/base)