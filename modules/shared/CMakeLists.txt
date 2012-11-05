project (shared)

include (UseMFC)

set (PROJECT_CPP
  stdafx.cpp
  TeXnicCenter.cpp
  TeXnicCenter.def
)

set (PROJECT_H
  resource.h
  stdafx.h
  targetver.h
  TeXnicCenter.h
)

set (PROJECT_RES
  res/TeXnicCenter.rc2
  TeXnicCenter.rc
)

if (CMAKE_MFC_FLAG EQUAL 2)
  add_definitions (-D_AFXDLL)
endif (CMAKE_MFC_FLAG EQUAL 2)

add_library (${PROJECT_NAME} SHARED ${PROJECT_CPP} ${PROJECT_H} ${PROJECT_RES})

# Setup precompiled headers
if (MSVC)
  set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -Yustdafx.h)
  set_source_files_properties (stdafx.cpp PROPERTIES COMPILE_FLAGS -Ycstdafx.h)
endif (MSVC)
