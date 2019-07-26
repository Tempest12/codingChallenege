#Author Shane del Solar

cmake_minimum_required(VERSION 3.0)

project(pigConvert CXX)
set(BINARY_NAME pigConvert)

# Determine the type of build we'll be doing:
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)

if(${CMAKE_BUILD_TYPE} MATCHES "debug")
	#Do Nothing as this must have been set on the command line.
else(${CMAKE_BUILD_TYPE} MATCHES "debug")
	set(CMAKE_BUILD_TYPE RELEASE)
endif(${CMAKE_BUILD_TYPE} MATCHES "debug")

# Set compiler to the gnu c++ compiler:
set(CMAKE_CXX_COMPILER g++)

# Set some build flags real quick:
set(CMAKE_CXX_FLAGS_DEBUG -g)
set(CMAKE_CXX_FLAGS_RELEASE -O3)
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread")

# Find the right library:
if(${CMAKE_SYSTEM_NAME} MATCHES Linux)
	set(PIG_LIB_PATH "${CMAKE_SOURCE_DIR}/lib/linux/")
elseif(${CMAKE_SYSTEM_NAME} MATCHES Darwin)
	set(PIG_LIB_PATH "${CMAKE_SOURCE_DIR}/lib/macos/")
elseif(${CMAKE_SYSTEM_NAME} MATCHES Windows)
	set(PIG_LIB_PATH "${CMAKE_SOURCE_DIR}/lib/windows/")
elseif(${CMAKE_SYSTEM_NAME} MATCHES Linux)
	message(FATAL_ERROR "Platform doesn't appear to be linux, mac, or windows. We only have a copy of PigLatinConverter for those 3 OSes.")
endif(${CMAKE_SYSTEM_NAME} MATCHES Linux)

find_library(PIG_LIB PigLatinConverter ${PIG_LIB_PATH})

# Get sources:
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add exe target:
add_executable(${BINARY_NAME} ${SOURCES})

# Set header location:
include_directories(${CMAKE_SOURCE_DIR}/include)

# Link libraries:
target_link_libraries(${BINARY_NAME} ${PIG_LIB})