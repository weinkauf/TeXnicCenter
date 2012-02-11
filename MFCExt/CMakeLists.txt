project (mfcext)

include (UseMFC)

set (PROJECT_CPP
  CollectionException.cpp
  ComboColorPicker.cpp
  ExtAfx.cpp
  FileDialogEx.cpp
  FileVersionInfo.cpp
  FolderSelect.cpp
  FormatEdit.cpp
  IniFile.cpp
  MFCExt.cpp
  MRUCombo.cpp
  MRUStringList.cpp
  PathTool.cpp
  PerformanceTimer.cpp
  Registry.cpp
  SingleInstanceWinApp.cpp
  StdAfx.cpp
  SystemInfo.cpp
  UniqueStringList.cpp
  WildCard.cpp
  WorkerThread.cpp
)

set (PROJECT_H
  AutoBuildCount.h
  CollectionException.h
  ComboColorPicker.h
  ComHelpers.h
  DoubleBuffer.h
  ExtAfx.h
  ExtApp.h
  ExtCollection.h
  ExtCOM.h
  ExtControls.h
  ExtDebug.h
  ExtDialog.h
  ExtException.h
  ExtTools.h
  FileDialogEx.h
  FileVersionInfo.h
  FolderSelect.h
  FormatEdit.h
  IniFile.h
  MRUCombo.h
  MRUList.h
  MRUStringList.h
  PathTool.h
  PerformanceTimer.h
  Registry.h
  resource.h
  SingleInstanceWinApp.h
  SortArray.h
  SortList.h
  StdAfx.h
  SystemInfo.h
  UniqueStringList.h
  WildCard.h
  WorkerThread.h
)

set (PROJECT_RES MFCExt.rc res/MFCExt.rc2)

source_group ("Resource Files" FILES ${PROJECT_RES})

add_library (${PROJECT_NAME} ${PROJECT_CPP} ${PROJECT_H} ${PROJECT_RES})

# Setup precompiled headers
if (MSVC)
  set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -YuStdAfx.h)
  set_source_files_properties (StdAfx.cpp PROPERTIES COMPILE_FLAGS -Yc)
endif (MSVC)

target_link_libraries (${PROJECT_NAME} version)

if (NOT BUILD_SHARED_LIBS)
  set_source_files_properties (${PROJECT_RES} PROPERTIES HEADER_FILE_ONLY TRUE)
endif (NOT BUILD_SHARED_LIBS)
