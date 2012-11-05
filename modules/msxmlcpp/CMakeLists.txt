project (msxmlcpp)

include (UseMFC)

set (PROJECT_CPP
  StdAfx.cpp
  DebugTools.cpp
  DOMXML.cpp
  Helpers.cpp
  MSXMLCPP.cpp
  SAXXML.cpp
  Stream.cpp
  XMLDOMDocumentEx.cpp
  XMLDOMPropertyStorage.cpp
)

set (PROJECT_H
  DebugTools.h
  DOMXML.h
  DOMXML_raw.h
  Helpers.h
  msxmlcpp.h
  MsXmlCppDbg.h
  resource.h
  SAXXML.h
  SAXXML_raw.h
  StdAfx.h
  Stream.h
  XMLDOMDocumentEx.h
  XMLDOMPropertyStorage.h
)

if (CMAKE_MFC_FLAG EQUAL 2)
  add_definitions (-D_AFXDLL)
endif (CMAKE_MFC_FLAG EQUAL 2)

add_library (${PROJECT_NAME} ${PROJECT_CPP} ${PROJECT_H})

# Setup precompiled headers
if (MSVC)
  set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -YuStdAfx.h)
  set_source_files_properties (StdAfx.cpp PROPERTIES COMPILE_FLAGS -YcStdAfx.h)
endif (MSVC)

target_link_libraries (${PROJECT_NAME} msxml6 msxml2)
