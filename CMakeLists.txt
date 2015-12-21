#    Date : Dec 21, 2015
# Authors : Roberto Metere

add_subdirectory (src)

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
  "${CMAKE_SOURCE_DIR}/${CMAKE_UNINSTALL}.in"
  "${CMAKE_BINARY_DIR}/${CMAKE_UNINSTALL}"
  IMMEDIATE @ONLY
  )

add_custom_target (
  uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
  )
