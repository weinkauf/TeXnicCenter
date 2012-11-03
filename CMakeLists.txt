cmake_minimum_required (VERSION 2.8)
project (TeXnicCenter)

list (APPEND CMAKE_FIND_ROOT_PATH ${CMAKE_CURRENT_BINARY_DIR})
list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_definitions (-D_BIND_TO_CURRENT_CRT_VERSION)
add_definitions (-D_BIND_TO_CURRENT_MFC_VERSION)
add_definitions (-D_CRT_SECURE_NO_WARNINGS)
add_definitions (-D_SCL_SECURE_NO_WARNINGS)
add_definitions (-D_UNICODE)

set (OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_subdirectory (interfacewrapper)
add_subdirectory (mfcext)
add_subdirectory (msxmlcpp)
add_subdirectory (objects)
add_subdirectory (shared)
add_subdirectory (thirdparty)
add_subdirectory (app)
