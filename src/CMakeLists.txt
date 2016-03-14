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

# After an update of cmake 2.8.x to 2.8.4
# if (WIN32)
#         set(CMAKE_RC_COMPILER windres)
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
CONFIGURE_FILE("config.h.in" "${CMAKE_SOURCE_DIR}/src/config.h")
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

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic -O${OPTIMIZATION} ${BIT32} ${PLATFORM_INDEPENDENT_CODE_FLAG} ${DEBUG_FLAG} ${DEBUG_INFO_FLAG} ${ARCH_CFLAG}")

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

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set (doxygenConfigFileIn "${PROJECT_SOURCE_DIR}/../Doxyfile.in")
  set (doxygenConfigFile "${CMAKE_BINARY_DIR}/Doxyfile")
  
  set (DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doc" CACHE PATH "Doxygen output directory")
  set (DOXYGEN_PROJECT_NUMBER "${LIBRARY_VERSION_RELEASE}.${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}" CACHE STRING "Doxygen project number")
  
  configure_file (${doxygenConfigFileIn} ${doxygenConfigFile} @ONLY)

  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${doxygenConfigFile}
    DEPENDS ${doxygenConfigFile}
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/../"
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
else (DOXYGEN_FOUND)
    message (STATUS "Documentation will not be built - Doxygen not found")
endif(DOXYGEN_FOUND)

# Copyright (C) 2015 Roberto Metere
