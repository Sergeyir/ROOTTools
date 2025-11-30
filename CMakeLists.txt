cmake_minimum_required(VERSION 3.2...4.1 FATAL_ERROR)

project(ROOTTools)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -pipe -O2 -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

find_package(ROOT QUIET)

if(${ROOT_FOUND})
   message(STATUS "ROOT package found")
   link_libraries(${ROOT_LIBRARIES})
elseif(EXISTS $ENV{ROOT_PATH}/bin/root)
   message(STATUS "ROOTTools: ROOT path found")
   set(ROOT_INCLUDE_DIRS $ENV{ROOT_PATH}/include)
   set(ROOT_root_CMD $ENV{ROOT_PATH}/bin/root)
   execute_process(COMMAND ${ROOT_root_CMD}-config --glibs OUTPUT_VARIABLE ROOT_LIB_FLAGS)
   string(STRIP ${ROOT_LIB_FLAGS} ROOT_LIB_FLAGS)
   set(CMAKE_SHARED_LINKER_FLAGS ${ROOT_LIB_FLAGS})
else()
   message(FATAL_ERROR "ROOT not found: install it via package manager or set environment variable ROOT_PATH if you installed it manually")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

add_library(TCanvasTools ${CMAKE_CURRENT_SOURCE_DIR}/src/TCanvasTools.cpp)
add_library(TF1Tools ${CMAKE_CURRENT_SOURCE_DIR}/src/TF1Tools.cpp)
add_library(ThrObj ${CMAKE_CURRENT_SOURCE_DIR}/src/ThrObj.cpp)
add_library(GUIDistrCutter2D ${CMAKE_CURRENT_SOURCE_DIR}/src/GUIDistrCutter2D.cpp)
