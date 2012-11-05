project (interfacewrapper)

include (UseMFC)

set (PROJECT_CPP stdafx.cpp InterfaceWrapper.cpp)
set (PROJECT_H stdafx.h InterfaceWrapper.h InterfaceWrapperTools.h
  InterfaceWrapper.inl)

if (CMAKE_MFC_FLAG EQUAL 2)
  add_definitions (-D_AFXDLL)
endif (CMAKE_MFC_FLAG EQUAL 2)

add_library (${PROJECT_NAME} ${PROJECT_CPP} ${PROJECT_H})

# Setup precompiled headers
if (MSVC)
  set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -Yustdafx.h)
  set_source_files_properties (stdafx.cpp PROPERTIES COMPILE_FLAGS -Ycstdafx.h)
endif (MSVC)
