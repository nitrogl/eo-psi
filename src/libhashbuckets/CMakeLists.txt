#    Date : Dec 21, 2015
# Authors : Roberto Metere
#
# The library
add_library (${LIBRARY_NAME} SHARED
        ntlmiss.cpp
        exception.cpp
        outofboundex.cpp
        murmurhash3.cpp
        shabyte.cpp
        shastr.cpp
        shazzp.cpp
        simplebm.cpp
        strint.cpp
        indzzrndstream_retrocomp.cpp
        indzzrndstream.cpp
        zzrandgen.cpp
        zzprandgen.cpp
        strrandgen.cpp
        bytekeygen.cpp
        strzzpkeygen.cpp
        eopsimsg.cpp
        eopsiparty.cpp
        eopsiserver.cpp
        eopsiclient.cpp
        libhashbuckets.rc
        )

# Arrange some properties, differing between OSes
if (WIN32)
  set_target_properties (${LIBRARY_NAME}
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME ${LIBRARY_NAME}
        VERSION "${LIBRARY_VERSION_RELEASE}.${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}"
        SOVERSION "${LIBRARY_VERSION_RELEASE}"
        )
else(WIN32)
  set_target_properties (${LIBRARY_NAME}
        PROPERTIES
        OUTPUT_NAME ${LIBRARY_NAME}
        VERSION "${LIBRARY_VERSION_RELEASE}.${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}"
        SOVERSION "${LIBRARY_VERSION_RELEASE}"
        )
endif(WIN32)

if (UNIX)
install (
        TARGETS ${LIBRARY_NAME}
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
        )

# Installing headers for developers
install ( # main header (it should be enough to include)
        FILES "../lib${LIBRARY_NAME}.h"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
        )
install ( # the needed included files
        DIRECTORY "../lib${LIBRARY_NAME}/"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/lib${LIBRARY_NAME}"
        FILES_MATCHING REGEX "[a-z]+\\.(h|hpp)"
        )
endif (UNIX)

# Copyright (C) 2015 Roberto Metere
