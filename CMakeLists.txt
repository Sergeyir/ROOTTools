cmake_minimum_required(VERSION 3.2...4.1 FATAL_ERROR)

project(ROOTTools)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -pipe -O2 -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

find_package(ROOT QUIET)

# Checking whether ROOT exists and setting includes and libraries for compiler
if(${ROOT_FOUND}) # checking via cmake
   message(STATUS "ROOT package found: ${ROOT_USE_FILE}")
   link_libraries(${ROOT_LIBRARIES})
   set(CMAKE_CXX_STANDARD ${ROOT_CXX_STANTARD})
elseif(DEFINED ENV{ROOT_PATH} AND EXISTS "$ENV{ROOT_PATH}/bin/root") #checking via ROOT_PATH
   message(STATUS "ROOT path found: $ENV{ROOT_PATH}")
   set(ROOT_PATH $ENV{ROOT_PATH})
else()
   find_program(ROOT_CONFIG NAMES root-config)
   if(ROOT_CONFIG)
      execute_process(COMMAND ${ROOT_CONFIG} --prefix OUTPUT_VARIABLE ROOT_PATH)
      string(STRIP ${ROOT_PATH} ROOT_PATH)
      message(STATUS "ROOT found via root-config in ${ROOT_PATH}")
   else()
      message(FATAL_ERROR "ROOT not found: install it via package manager, or source /path/to/root/bin/thisroot.sh in your profile if you installed it manually and set ROOT_PATH")
   endif()
endif()
if (DEFINED ROOT_PATH) # for manually installed ROOT
   set(ROOT_INCLUDE_DIRS "${ROOT_PATH}/include")
   set(ROOT_root_CMD "${ROOT_PATH}/bin/root")
   execute_process(COMMAND ${ROOT_root_CMD}-config --glibs OUTPUT_VARIABLE ROOT_LIB_FLAGS)
   execute_process(COMMAND ${ROOT_root_CMD}-config --cxxstandard OUTPUT_VARIABLE ROOT_CXX_STANDARD)
   string(STRIP ${ROOT_LIB_FLAGS} ROOT_LIB_FLAGS)
   string(STRIP ${ROOT_CXX_STANDARD} ROOT_CXX_STANDARD)
   set(CMAKE_CXX_STANDARD ${ROOT_CXX_STANDARD})
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

add_library(HistTools ${CMAKE_CURRENT_SOURCE_DIR}/src/HistTools.cpp)
add_library(TCanvasTools ${CMAKE_CURRENT_SOURCE_DIR}/src/TCanvasTools.cpp)
add_library(TF1Tools ${CMAKE_CURRENT_SOURCE_DIR}/src/TF1Tools.cpp)
#add_library(ThrObj ${CMAKE_CURRENT_SOURCE_DIR}/src/ThrObj.cpp)
add_library(GUIDistrCutter2D ${CMAKE_CURRENT_SOURCE_DIR}/src/GUIDistrCutter2D.cpp)
add_library(GUIFit ${CMAKE_CURRENT_SOURCE_DIR}/src/GUIFit.cpp)
