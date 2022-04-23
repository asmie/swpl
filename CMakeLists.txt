# CMakeLists file written by Piotr "asmie" Olszewski
# Project: swpl

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(swpl)

# Versioning
set (SWPL_VERSION_MAJOR 0)
set (SWPL_VERSION_MINOR 0)
set (SWPL_VERSION_REV 1)
set (SWPL_VERSION ${SWPL_VERSION_MAJOR}.${SWPL_VERSION_MINOR}.${SWPL_VERSION_REV} )

# Configuration
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# Compilation options
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pipe)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g3 -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -g0 -Os")

    set(CMAKE_EXE_LINKER_FLAGS "-z now -s")
endif()

# Checking time
include (CheckIncludeFileCXX)
include (CheckCXXSymbolExists)

check_include_file_cxx (cstdlib SWPL_SYSTEM_HAVE_CSTDLIB)
check_include_file_cxx (cstdio SWPL_SYSTEM_HAVE_CSTDIO)
check_include_file_cxx (iostream SWPL_SYSTEM_HAVE_IOSTREAM)
check_include_file_cxx (cstring SWPL_SYSTEM_HAVE_CSTRING)
check_include_file_cxx (cstdarg SWPL_SYSTEM_HAVE_CSTDARG)
check_include_file_cxx (atomic SWPL_SYSTEM_HAVE_ATOMIC)
check_include_file_cxx (fstream SWPL_SYSTEM_HAVE_FSTREAM)
check_include_file_cxx (exception SWPL_SYSTEM_HAVE_EXCEPTION)
check_include_file_cxx (regex SWPL_SYSTEM_HAVE_REGEX)
check_include_file_cxx (unordered_map SWPL_SYSTEM_HAVE_UNORDERED_MAP)

check_include_file_cxx (fcntl.h SWPL_SYSTEM_HAVE_FCNTL_H)
check_include_file_cxx (io.h SWPL_SYSTEM_HAVE_IO_H)
check_include_file_cxx (unistd.h SWPL_SYSTEM_HAVE_UNISTD_H)

check_cxx_symbol_exists (EXIT_SUCCESS cstdlib SWPL_SYSTEM_HAVE_EXIT_SUCCESS)
check_cxx_symbol_exists (memcpy cstring SWPL_SYSTEM_HAVE_MEMCPY)
check_cxx_symbol_exists (vsyslog syslog.h SWPL_SYSTEM_HAVE_VSYSLOG)
check_cxx_symbol_exists (vprintf cstdio SWPL_SYSTEM_HAVE_VPRINTF)
check_cxx_symbol_exists (fflush cstdio SWPL_SYSTEM_HAVE_FFLUSH)


# Configuration file
configure_file (
  "${PROJECT_SOURCE_DIR}/src/swpl_config.h.in"
  "generated/swpl_config.h"
  )

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  file(GLOB OS_FILES src/osdep/linux/*.cpp)
  set(ADDITIONAL_LIBRARIES "")
  add_compile_definitions(OS_BUILD_LINUX)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  file(GLOB OS_FILES src/osdep/windows/*.cpp)
  set(ADDITIONAL_LIBRARIES wsock32)
  add_compile_definitions(OS_BUILD_WINDOWS)
endif()

file(GLOB SRC_FILES src/config/*.cpp src/core/*.cpp )

# Find source files
file(GLOB SOURCES src/main.cpp ${SRC_FILES} ${OS_FILES} )

# Include header files
include_directories(src ${CMAKE_CURRENT_BINARY_DIR}/generated)

# Find packages
# # Threads
find_package(Threads REQUIRED)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Threads::Threads ${ADDITIONAL_LIBRARIES})

# Install executable
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})


# Testing time
enable_testing()

# Try to find GTest
find_package(GTest)

if(GTEST_FOUND)
	include_directories(src ${CMAKE_CURRENT_BINARY_DIR}/generated ${GTEST_INCLUDE_DIR})
	file(GLOB TEST_SOURCES tests/unit/*.cpp ${OS_FILES} ${SRC_FILES} )
	add_executable(${PROJECT_NAME}_unit ${TEST_SOURCES})
	target_link_libraries(${PROJECT_NAME}_unit PUBLIC gtest pthread ${CMAKE_DL_LIBS} ${ADDITIONAL_LIBRARIES})
	add_test(${PROJECT_NAME}_unit_test ${PROJECT_NAME}_unit)
endif(GTEST_FOUND)

