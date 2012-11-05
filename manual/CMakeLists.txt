project (manual)

find_package (HTMLHelp)

if (NOT HTML_HELP_COMPILER)
  return ()
endif (NOT HTML_HELP_COMPILER)

set (_NAME TeXnicCenter)
set (_OUTPUT_FILE ${_NAME}.hhp)
set (_DIRECTORY ${CMAKE_BINARY_DIR})
set (BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (_COPY_OUTPUT_DIR ${_DIRECTORY}/${CMAKE_CFG_INTDIR}/Help)
set (_OUTPUT_DOC_FILE ${BUILD_DIR}/${_NAME}.chm)
set (_TARGET manual)

set (_BUILD_SCRIPT
  ${CMAKE_CURRENT_BINARY_DIR}/sphinx-htmlhelp-build.cmake)
file (WRITE ${_BUILD_SCRIPT}
"
EXECUTE_PROCESS (COMMAND \${HTML_HELP_COMPILER} \${OUTPUT_NAME}
  WORKING_DIRECTORY \${BUILD_DIR} RESULT_VARIABLE _RESULT)
")

add_custom_target (${_TARGET}
  COMMAND ${CMAKE_COMMAND}
          -DHTML_HELP_COMPILER=${HTML_HELP_COMPILER}
          -DBUILD_DIR=${BUILD_DIR}
          -DOUTPUT_NAME=${_OUTPUT_FILE}
          -P ${_BUILD_SCRIPT}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${_OUTPUT_DOC_FILE} ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/Amslatex.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/Bibtex.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/CfgGuide.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/ClsGuide.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/FAQ.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/FntGuide.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/Latex.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/Latex2e.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/Latex2eForAuthors.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/LatexAdv.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/LatexHelpBook.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/LatexMathsPix.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/MakeIndex.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/ModGuide.chm
    ${_COPY_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/LaTeXHelp/TexCatalogue.chm
    ${_COPY_OUTPUT_DIR}
  COMMENT "Generating ${_NAME} manual"
)
