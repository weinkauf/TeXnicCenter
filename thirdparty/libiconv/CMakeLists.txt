project (libiconv)

add_definitions (-DLIBDIR=)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include)

if (MSVC)
  add_definitions (-wd4018)
  add_definitions (-wd4090)
  add_definitions (-wd4244)
  add_definitions (-wd4267)
  add_definitions (-wd4273)
endif (MSVC)

set (PROJECT_C lib/iconv.c libcharset/lib/localcharset.c lib/relocatable.c)
set (PROJECT_H include/iconv.h)

add_library (${PROJECT_NAME} ${PROJECT_C} ${PROJECT_H})
