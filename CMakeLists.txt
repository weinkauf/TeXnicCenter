cmake_minimum_required (VERSION 2.8)
project (TeXnicCenter)

list (APPEND CMAKE_FIND_ROOT_PATH ${CMAKE_CURRENT_BINARY_DIR})
list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CPACK_COMPONENTS_ALL Runtime)
set (CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
set (CPACK_COMPONENT_LIBRARIES_GROUP "Development")
set (CPACK_COMPONENT_HEADERS_GROUP "Development")

if (APPLE)
  set (CPACK_GENERATOR DragNDrop)
elseif (UNIX)
  set (CPACK_GENERATOR DEB)
else (APPLE)
  set (CPACK_GENERATOR ZIP)
endif (APPLE)

include (TeXnicCenter)
include (CPack)

add_definitions (-D_BIND_TO_CURRENT_CRT_VERSION)
add_definitions (-D_BIND_TO_CURRENT_MFC_VERSION)
add_definitions (-D_CRT_SECURE_NO_WARNINGS)
add_definitions (-D_SCL_SECURE_NO_WARNINGS)
add_definitions (-D_UNICODE)

add_definitions (-D_WIN32_WINNT=0x0601)
add_definitions (-D_WIN32_WINDOWS=0x0410)
add_definitions (-D_WIN32_IE=0x0700)

set (OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})

if (MSVC)
  string (REPLACE "/Zm1000" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif (MSVC)

add_subdirectory (manual)
add_subdirectory (modules)
add_subdirectory (thirdparty)
add_subdirectory (app)
add_subdirectory (setup)
