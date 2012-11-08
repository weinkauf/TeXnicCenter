set (APPLICATION_NAME TeXnicCenter)
set (YEAR "1999-2012")

set (MAJOR_VERSION 2)
set (MINOR_VERSION 0)
set (VERSION_PATCH 0)
# uncomment the next line if not applicable
set (VERSION_EXTRA "Beta 1")

set (_COPYRIGHT "${YEAR} The TeXnicCenter Team")
# place a copyright sign infront of the banner
set (COPYRIGHT "\\xA9 ${_COPYRIGHT}")
set (LEGAL_COPYRIGHT "Copyright ${COPYRIGHT}")
set (COPYRIGHT_ONLY "Copyright \\xA9 ${YEAR}")

# do not edit after this line

include (InstallRequiredSystemLibraries)

set (SHORT_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}")

set (VERSION_STRING ${SHORT_VERSION})

if (NOT VERSION_EXTRA STREQUAL "")
  set (VERSION_STRING "${VERSION_STRING} ${VERSION_EXTRA}")
endif (NOT VERSION_EXTRA STREQUAL "")

if (NOT ${VERSION_PATCH} EQUAL 0)
  set (SHORT_VERSION "${SHORT_VERSION}.${VERSION_PATCH}")
endif (NOT ${VERSION_PATCH} EQUAL 0)

set (VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${VERSION_PATCH}")

set (CPACK_PACKAGE_NAME ${APPLICATION_NAME})
set (CPACK_PACKAGE_VERSION_MAJOR ${MAJOR_VERSION})
set (CPACK_PACKAGE_VERSION_MINOR ${MINOR_VERSION})
set (CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set (CPACK_PACKAGE_VERSION ${SHORT_VERSION})
