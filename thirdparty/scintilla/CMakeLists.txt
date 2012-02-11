project (Scintilla)
cmake_minimum_required (VERSION 2.8.6)

if (NOT WIN32)
  find_package (GTK2 REQUIRED gtk)
endif (NOT WIN32)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/lexlib)
include_directories (${GTK2_INCLUDE_DIRS})

file (GLOB PROJECT_PUBLIC_H
  include/ILexer.h
  include/Platform.h
  include/SciLexer.h
  include/Scintilla.h
  include/ScintillaWidget.h
)

file (GLOB PROJECT_CXX
  lexlib/Accessor.cxx
  lexlib/CharacterSet.cxx
  lexlib/LexerBase.cxx
  lexlib/LexerModule.cxx
  lexlib/LexerNoExceptions.cxx
  lexlib/LexerSimple.cxx
  lexlib/PropSetSimple.cxx
  lexlib/StyleContext.cxx
  lexlib/WordList.cxx
  lexers/LexA68k.cxx
  lexers/LexAbaqus.cxx
  lexers/LexAda.cxx
  lexers/LexAPDL.cxx
  lexers/LexAsm.cxx
  lexers/LexAsn1.cxx
  lexers/LexASY.cxx
  lexers/LexAU3.cxx
  lexers/LexAVE.cxx
  lexers/LexBaan.cxx
  lexers/LexBash.cxx
  lexers/LexBasic.cxx
  lexers/LexBibTeX.cxx
  lexers/LexBullant.cxx
  lexers/LexCaml.cxx
  lexers/LexCLW.cxx
  lexers/LexCmake.cxx
  lexers/LexCOBOL.cxx
  lexers/LexCoffeeScript.cxx
  lexers/LexConf.cxx
  lexers/LexCPP.cxx
  lexers/LexCrontab.cxx
  lexers/LexCsound.cxx
  lexers/LexCSS.cxx
  lexers/LexD.cxx
  lexers/LexEiffel.cxx
  lexers/LexErlang.cxx
  lexers/LexEScript.cxx
  lexers/LexFlagship.cxx
  lexers/LexForth.cxx
  lexers/LexFortran.cxx
  lexers/LexGAP.cxx
  lexers/LexGui4Cli.cxx
  lexers/LexHaskell.cxx
  lexers/LexHTML.cxx
  lexers/LexInno.cxx
  lexers/LexKix.cxx
  lexers/LexLisp.cxx
  lexers/LexLout.cxx
  lexers/LexLua.cxx
  lexers/LexMagik.cxx
  lexers/LexMarkdown.cxx
  lexers/LexMatlab.cxx
  lexers/LexMetapost.cxx
  lexers/LexMMIXAL.cxx
  lexers/LexModula.cxx
  lexers/LexMPT.cxx
  lexers/LexMSSQL.cxx
  lexers/LexMySQL.cxx
  lexers/LexNimrod.cxx
  lexers/LexNsis.cxx
  lexers/LexOpal.cxx
  lexers/LexOthers.cxx
  lexers/LexPascal.cxx
  lexers/LexPB.cxx
  lexers/LexPerl.cxx
  lexers/LexPLM.cxx
  lexers/LexPOV.cxx
  lexers/LexPowerPro.cxx
  lexers/LexPowerShell.cxx
  lexers/LexProgress.cxx
  lexers/LexPS.cxx
  lexers/LexPython.cxx
  lexers/LexR.cxx
  lexers/LexRebol.cxx
  lexers/LexRuby.cxx
  lexers/LexScriptol.cxx
  lexers/LexSmalltalk.cxx
  lexers/LexSML.cxx
  lexers/LexSorcus.cxx
  lexers/LexSpecman.cxx
  lexers/LexSpice.cxx
  lexers/LexSQL.cxx
  lexers/LexTACL.cxx
  lexers/LexTADS3.cxx
  lexers/LexTAL.cxx
  lexers/LexTCL.cxx
  lexers/LexTeX.cxx
  lexers/LexTxt2tags.cxx
  lexers/LexVB.cxx
  lexers/LexVerilog.cxx
  lexers/LexVHDL.cxx
  lexers/LexYAML.cxx
  src/AutoComplete.cxx
  src/CallTip.cxx
  src/Catalogue.cxx
  src/CellBuffer.cxx
  src/CharClassify.cxx
  src/ContractionState.cxx
  src/Decoration.cxx
  src/Document.cxx
  src/Editor.cxx
  src/ExternalLexer.cxx
  src/Indicator.cxx
  src/KeyMap.cxx
  src/LineMarker.cxx
  src/PerLine.cxx
  src/PositionCache.cxx
  src/RESearch.cxx
  src/RunStyles.cxx
  src/ScintillaBase.cxx
  src/Selection.cxx
  src/Style.cxx
  src/UniConversion.cxx
  src/ViewStyle.cxx
  src/XPM.cxx
)

add_definitions (-DSCI_LEXER)

if (MSVC)
  add_definitions (-D_CRT_SECURE_NO_WARNINGS)
endif (MSVC)

if (WIN32 AND BUILD_SHARED_LIBS)
  file (GLOB PROJECT_RES win32/Scintilla.def win32/ScintRes.rc)
elseif (NOT WIN32)
  add_definitions (-DGTK)
endif (WIN32 AND BUILD_SHARED_LIBS)

if (WIN32)
  file (GLOB PROJECT_PLATFORM_CXX win32/ScintillaWin.cxx win32/PlatWin.cxx)
else(WIN32)
  file (GLOB PROJECT_PLATFORM_CXX gtk/ScintillaGTK.cxx win32/PlatGTK.cxx)
endif (WIN32)

add_library (scintilla ${PROJECT_CXX} ${PROJECT_PLATFORM_CXX} ${PROJECT_RES})

if (NOT WIN32)
  target_link_libraries (scintilla ${GTK2_LIBRARIES})
endif (NOT WIN32)

set_target_properties (scintilla PROPERTIES PUBLIC_HEADER "${PROJECT_PUBLIC_H}")

if (NOT BUILD_SHARED_LIBS)
  set_target_properties (scintilla PROPERTIES COMPILE_DEFINITIONS STATIC_BUILD)
endif (NOT BUILD_SHARED_LIBS)

file (READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt _VERSION)

string (REGEX REPLACE "([0-9]+)" "\\1" _VERSION "${_VERSION}")

math (EXPR VERSION_MAJOR "${_VERSION} / 100")
math (EXPR VERSION_MINOR "${_VERSION} % 100")

configure_file (scintilla-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/scintilla-config.cmake COPYONLY)
configure_file (scintilla-config-build.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/scintilla-config-build.cmake @ONLY)
configure_file (scintilla-config-version.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/scintilla-config-version.cmake @ONLY)

install (TARGETS scintilla EXPORT scintilla-targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include)

export (TARGETS scintilla FILE
  ${CMAKE_CURRENT_BINARY_DIR}/scintilla-targets.cmake)
export (PACKAGE scintilla)

install (EXPORT scintilla-targets DESTINATION . COMPONENT Development)
install (FILES
    ${CMAKE_CURRENT_BINARY_DIR}/scintilla-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/scintilla-config-version.cmake
    DESTINATION . COMPONENT Development)
