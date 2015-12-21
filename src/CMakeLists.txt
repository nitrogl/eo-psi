#    Date : Dec 21, 2015
# Authors : Roberto Metere
#
# Main cmake file.
set (LIBRARY_NAME "hashbuckets")

# Set the makefile verbose
set (CMAKE_VERBOSE_MAKEFILE 1)

# Let the script choose the shared or static compilation method
option (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
include (CheckIncludeFiles)

# Minimum version and project name
cmake_minimum_required (VERSION 2.6)

# After an update of cmake 2.8.x to 2.8.4
# if (WIN32)
# 	set(CMAKE_RC_COMPILER windres)
# endif()

project (${LIBRARY_NAME})

# The version number
set (LIBRARY_VERSION_RELEASE "0")
set (LIBRARY_VERSION_MAJOR "1")
set (LIBRARY_VERSION_MINOR "alpha")

# configure a header file to pass some of the CMake settings
# to the source code
# configure_file (
#   "${PROJECT_SOURCE_DIR}/lib${LIBRARY_NAME}.h.in"
#   "${PROJECT_BINARY_DIR}/lib${LIBRARY_NAME}.h"
#   )

# Check headers
CHECK_INCLUDE_FILES (pthread.h HAVE_PTHREAD_H)
CONFIGURE_FILE("config.h.in" "${CMAKE_SOURCE_DIR}/config.h")
find_package (Threads)

# Compiler flags
set (OPTIMIZATION "0")
set (DEBUG_INFO_FLAG "-g")
set (PLATFORM_INDEPENDENT_CODE_FLAG "-fPIC")
if (BIT32)
  set (BIT32 "-m32") # cmake -DBIT32=1
endif(BIT32)

# Debug flags (Comment on dist)
# set (DEBUG_FLAG "-DDEBUG")
# set (OPTIMIZATION "0")

# Detect the compilation system
if (APPLE)
	message ("Apple system found.")
elseif (WIN32)
	message ("Windows system found.")
elseif (UNIX)
	message ("UNIX system found.")
else (UNIX)
	message ("WARNING: Unknown system.")
endif (APPLE)

# Code flags
if (CYGWIN)
	set (PLATFORM_INDEPENDENT_CODE_FLAG "")
endif (CYGWIN)
if (WIN32)
	set (PLATFORM_INDEPENDENT_CODE_FLAG "")
endif (WIN32)

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -O${OPTIMIZATION} ${BIT32} ${PLATFORM_INDEPENDENT_CODE_FLAG} ${DEBUG_FLAG} ${DEBUG_INFO_FLAG} ${ARCH_CFLAG}")

# Apple stuff
if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
  
  if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  endif()
endif()

# Make lib and test
add_subdirectory (lib${LIBRARY_NAME})
add_subdirectory (test)

# if (UNIX)
# install (
#         TARGETS ${LIBRARY_NAME}
#         DESTINATION "${CMAKE_LIBRARY_PATH}"
#         )
# 
# # Installing headers for developers
# install ( # main header (it should be enough to include)
#         FILES "../src/lib${LIBRARY_NAME}.h"
#         DESTINATION "${CMAKE_INCLUDE_PATH}"
#         )
# install ( # the needed included files
#         DIRECTORY "../src/"
#         DESTINATION "${CMAKE_INCLUDE_PATH}/lib${LIBRARY_NAME}"
#         FILES_MATCHING PATTERN "*.h"
#         )
# endif (UNIX)

# Uninstall target
set (CMAKE_UNINSTALL "cmake_uninstall.cmake")
configure_file(
	"${CMAKE_SOURCE_DIR}/../${CMAKE_UNINSTALL}.in"
	"${CMAKE_BINARY_DIR}/${CMAKE_UNINSTALL}"
	IMMEDIATE @ONLY
	)

add_custom_target (
	uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
	)

# Copyright (C) 2015 Roberto Metere
