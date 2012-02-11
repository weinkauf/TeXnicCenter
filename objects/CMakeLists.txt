project (objects)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${CMAKE_CURRENT_BINARY_DIR})

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/CMakeTmp/dummy.cpp "")

set (PROJECT_IDL
  DocumentWizard.idl
  Project.idl
  ProjectWizard.idl
  TeXnicCenterObjects.idl
)

set (PROJECT_IDL_GENERATED
  "${CMAKE_CURRENT_BINARY_DIR}/\$(IntDir)/DocumentWizard_i.c"
  "${CMAKE_CURRENT_BINARY_DIR}/\$(IntDir)/DocumentWizard_p.c"
  "${CMAKE_CURRENT_BINARY_DIR}/\$(IntDir)/Project_i.c"
  "${CMAKE_CURRENT_BINARY_DIR}/\$(IntDir)/Project_p.c"
  "${CMAKE_CURRENT_BINARY_DIR}/\$(IntDir)/ProjectWizard_i.c"
  "${CMAKE_CURRENT_BINARY_DIR}/\$(IntDir)/ProjectWizard_p.c"
  "${CMAKE_CURRENT_BINARY_DIR}/\$(IntDir)/TeXnicCenterObjects_i.c"
)

set (PROJECT_CPP
  ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/CMakeTmp/dummy.cpp
)

set (PROJECT_H TeXnicCenterObjects.h)

add_library (${PROJECT_NAME} ${PROJECT_CPP} ${PROJECT_IDL} ${PROJECT_H}
  ${PROJECT_IDL_GENERATED})

set_source_files_properties (${PROJECT_IDL_GENERATED} PROPERTIES GENERATED TRUE)
