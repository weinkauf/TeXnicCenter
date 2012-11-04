project (interfacewrapper)

include (UseMFC)

set (PROJECT_CPP InterfaceWrapper.cpp stdafx.cpp)
set (PROJECT_H stdafx.h InterfaceWrapper.h InterfaceWrapperTools.h
  InterfaceWrapper.inl)

add_library (${PROJECT_NAME} ${PROJECT_CPP} ${PROJECT_H})

# Setup precompiled headers
if (MSVC)
  set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -Yustdafx.h)
  set_source_files_properties (stdafx.cpp PROPERTIES COMPILE_FLAGS -Yc)
endif (MSVC)
