if (NOT TARGET manual)
  return ()
endif (NOT TARGET manual)

find_package (InnoSetup)

if (NOT INNOSETUP_FOUND)
  return ()
endif (NOT INNOSETUP_FOUND)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set (_TARGET x64)
else (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set (_TARGET Win32)
endif (CMAKE_SIZEOF_VOID_P EQUAL 8)

add_custom_target (setup
  COMMAND ${INNOSETUP_COMPILER_EXECUTABLE}
  ${CMAKE_CURRENT_SOURCE_DIR}/TeXnicCenter.iss
  /DROOT_DIR=${CMAKE_SOURCE_DIR}
  /DSOURCE_DIR=${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}
  /DOUTPUT_DIR=${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}
  /DTARGET_${_TARGET}
  SOURCES TeXnicCenter.iss
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating setup"
)

add_dependencies (setup app manual lang_en_US lang_de_DE shared)
