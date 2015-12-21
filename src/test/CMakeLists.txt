#    Date : Dec 21, 2015
# Authors : Roberto Metere
#
# Test cmake file.

# Make sure the compiler can find include files from the "hashbuckets" library.
include_directories (
  "${PROJECT_SOURCE_DIR}/"
  "../"
  )

# Make sure the linker can find the library once it is built.
link_directories (
  "${PROJECT_BINARY_DIR}/"
  )


# Add executable "zzrndgen"
add_executable(zzrndgen zzrndgen.cpp)
target_link_libraries (zzrndgen ntl)
set_target_properties (zzrndgen PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add executable "hbtest"
add_executable (hbtest hbtest.cpp)
target_link_libraries (hbtest "${LIBRARY_NAME}")
target_link_libraries (hbtest ntl gmpxx gmp cryptopp)
target_link_libraries (hbtest "${CMAKE_THREAD_LIBS_INIT}")
set_target_properties (hbtest PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# Copyright (C) 2015 Roberto Metere
