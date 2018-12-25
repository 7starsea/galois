CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(galois_library)


INCLUDE(ShannonCommon.cmake)
shannon_init_compiler_system()
shannon_init_boost_env()

if(WIN32)    
    find_package(Boost 1.57 COMPONENTS thread system filesystem atomic date_time chrono  REQUIRED)
else()
    find_package(Boost 1.57 COMPONENTS thread system filesystem atomic date_time chrono  REQUIRED)    
endif()
message(status "**Boost_LIBRARIES: ${Boost_LIBRARIES}")
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR} )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/..)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/..)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/..)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/..)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/..)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/..)

add_subdirectory(galois)

add_subdirectory(snappy)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(snappy_util)
    



