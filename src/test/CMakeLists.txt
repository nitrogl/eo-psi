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
target_link_libraries (zzrndgen "${LIBRARY_NAME}")
target_link_libraries (zzrndgen ntl gmpxx gmp cryptopp)
target_link_libraries (zzrndgen "${CMAKE_THREAD_LIBS_INIT}")
set_target_properties (zzrndgen PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add executable "hbtest"
add_executable (hbtest hbtest.cpp)
target_link_libraries (hbtest "${LIBRARY_NAME}")
target_link_libraries (hbtest ntl gmpxx gmp cryptopp)
target_link_libraries (hbtest "${CMAKE_THREAD_LIBS_INIT}")
set_target_properties (hbtest PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add executable "bptest"
add_executable (bptest bptest.cpp)
target_link_libraries (bptest "${LIBRARY_NAME}")
target_link_libraries (bptest ntl gmpxx gmp cryptopp)
target_link_libraries (bptest "${CMAKE_THREAD_LIBS_INIT}")
set_target_properties (bptest PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add executable "hspsisrv"
add_executable(hspsisrv hspsisrv.cpp)
target_link_libraries (hspsisrv "${LIBRARY_NAME}")
target_link_libraries (hspsisrv ntl gmpxx gmp cryptopp)
target_link_libraries (hspsisrv "${CMAKE_THREAD_LIBS_INIT}")
set_target_properties (hspsisrv PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add executable "hspsicli"
add_executable(hspsicli hspsicli.cpp)
target_link_libraries (hspsicli "${LIBRARY_NAME}")
target_link_libraries (hspsicli ntl gmpxx gmp cryptopp)
target_link_libraries (hspsicli "${CMAKE_THREAD_LIBS_INIT}")
set_target_properties (hspsicli PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# Copyright (C) 2015 Roberto Metere
