cmake_minimum_required(VERSION 3.25.2)

project(pdif VERSION 2.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# add -Werror
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Mac needed variables (adapt according to your needs)
set(CMAKE_MACOSX_RPATH ON)
#set(CMAKE_SKIP_BUILD_RPATH FALSE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_definitions(-DPOINTERHOLDER_TRANSITION=-0)

enable_testing()

option(PDIF_BUILD_CLI "Build the command line interface" ON)
option(PDIF_BUILD_ENGINE_TESTS "Build the tests" ON)
option(PDIF_BUILD_CLI_TESTS "Build the tests" ON)

add_subdirectory(engine)

if(PDIF_BUILD_CLI)
    add_subdirectory(cli)
endif()