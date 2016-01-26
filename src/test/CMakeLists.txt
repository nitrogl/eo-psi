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

# Add executable "eopsisrv"
add_executable(eopsisrv eopsisrv.cpp)
target_link_libraries (eopsisrv "${LIBRARY_NAME}")
target_link_libraries (eopsisrv ntl gmpxx gmp cryptopp)
target_link_libraries (eopsisrv "${CMAKE_THREAD_LIBS_INIT}")
set_target_properties (eopsisrv PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add executable "eopsicli"
add_executable(eopsicli eopsicli.cpp)
target_link_libraries (eopsicli "${LIBRARY_NAME}")
target_link_libraries (eopsicli ntl gmpxx gmp cryptopp)
target_link_libraries (eopsicli "${CMAKE_THREAD_LIBS_INIT}")
set_target_properties (eopsicli PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# Copyright (C) 2015 Roberto Metere
