project (app)

include (UseMFC)

find_package (HtmlHelp REQUIRED)
find_package (scintilla REQUIRED NO_MODULE)

set (PROJECT_CPP
  aboutbox.cpp
  AbstractLaTeXCommand.cpp
  ActiveProfileDialog.cpp
  Advice.cpp
  AutoCompleteDlg.cpp
  AutoCompleteListbox.cpp
  BibItem.cpp
  BibTeXDocument.cpp
  BiBTeXEntry.cpp
  BibTeXEntryType.cpp
  BiBTeXFile.cpp
  BibTeXOutputFilter.cpp
  BibTeXView.cpp
  BibView.cpp
  BookmarkEventArgs.cpp
  BookmarkView.cpp
  BrowseButton.cpp
  buildview.cpp
  childfrm.cpp
  CodeBookmark.cpp
  CodeDocument.cpp
  CodeView.cpp
  configuration.cpp
  DdeCommand.cpp
  DeclareOption.cpp
  DocumentNewDialog.cpp
  DocumentTokenizer.cpp
  DragSourceBase.cpp
  EditMenuButton.cpp
  EditMenuButtonOpt.cpp
  EncodingConverter.cpp
  environmentview.cpp
  ErrorListPane.cpp
  EventArgs.cpp
  FileChangeWatcher.cpp
  FileClean.cpp
  FileCleanConfirmDialog.cpp
  filegrep.cpp
  FileTreeCtrl.cpp
  FileViewEx.cpp
  FileViewPane.cpp
  findinfilesdialog.cpp
  FindReplaceDlg.cpp
  FontOccManager.cpp
  global.cpp
  gotodialog.cpp
  grepview.cpp
  IDocumentWizard.cpp
  insertfloatobjectdialog.cpp
  insertgraphicdialog.cpp
  insertheaderdialog.cpp
  inserttabulardialog.cpp
  IProject.cpp
  IProjectWizard.cpp
  ItemIDList.cpp
  itempropertydialog.cpp
  LabelDragSource.cpp
  LaTeXCommand.cpp
  LaTeXDocument.cpp
  LaTeXDocumentBase.cpp
  LatexOutputFilter.cpp
  latexproject.cpp
  LaTeXTokenizer.cpp
  LaTeXView.cpp
  LaTeXViewBase.cpp
  mainfrm.cpp
  MenuButton.cpp
  MetaPostDocument.cpp
  MetaPostView.cpp
  mrutrackselcombobox.cpp
  MultiExtensionDocTemplate.cpp
  navigatorview.cpp
  NewCommand.cpp
  NewEnvironment.cpp
  NotifyingEditListBox.cpp
  optiondialog.cpp
  OptionPageEditor.cpp
  optionpagefile.cpp
  OptionPageFileClean.cpp
  optionpagegeneric.cpp
  OptionPageLanguage.cpp
  optionpagepath.cpp
  OptionPageTextFormat.cpp
  OutputBuilder.cpp
  OutputDoc.cpp
  OutputFilter.cpp
  OutputInfo.cpp
  outputview.cpp
  OutputWizard.cpp
  OutputWizardDistributionPath.cpp
  OutputWizardFinish.cpp
  OutputWizardMikTex.cpp
  OutputWizardPage.cpp
  OutputWizardViewer.cpp
  OutputWizardWelcome.cpp
  PackageScanProgress.cpp
  ParseOutputView.cpp
  PersistPosEdit.cpp
  PlaceHolder.cpp
  PlaceholderSetsOptDlg.cpp
  PlaceholderSingleOptDlg.cpp
  PostProcessor.cpp
  PProcessor.cpp
  PreProcessor.cpp
  process.cpp
  ProcessCommand.cpp
  Profile.cpp
  ProfileDialog.cpp
  ProfileExchangeDialog.cpp
  ProfileNameDialog.cpp
  ProfilePage.cpp
  ProfilePageLatex.cpp
  ProfilePagePostprocessor.cpp
  ProfilePagePreprocessor.cpp
  ProfilePagePprocessor.cpp
  ProfilePageViewer.cpp
  Project.cpp
  projectnewdialog.cpp
  projectpropertydialog.cpp
  ProjectSupportingWinApp.cpp
  ProjectTemplate.cpp
  ProjectView.cpp
  PropertyDialog.cpp
  RegistryStack.cpp
  ScintillaCtrl.cpp
  ScintillaDocView.cpp
  SearchToolBarEditButton.cpp
  SearchToolBarEditCtrl.cpp
  ShadowWindow.cpp
  ShellUtilXP.cpp
  ShellView.cpp
  SortListCtrl.cpp
  SpellCheckDlg.cpp
  Speller.cpp
  SpellerBackgroundThread.cpp
  SpellerSource.cpp
  splash.cpp
  stdafx.cpp
  StructureItem.cpp
  structureparser.cpp
  StructureTreeCtrl.cpp
  structureview.cpp
  StyleFile.cpp
  StyleFileContainer.cpp
  TabPersistPosEdit.cpp
  TemplateDialog.cpp
  TeXnicCenter.cpp
  textfilesavedialog.cpp
  TextModules.cpp
  TextModulesDlg.cpp
  TextOutsourceDlg.cpp
  textsourcefile.cpp
  ThemedCtrl.cpp
  tipdlg.cpp
  ToolBarsCustomizeDialog.cpp
  TransparencyDlg.cpp
  UserTool.cpp
  UserToolAdvDlg.cpp
  WorkspacePane.cpp
  WorkspacePaneBase.cpp
)

set (PROJECT_H
  aboutbox.h
  AbstractLaTeXCommand.h
  ActiveProfileDialog.h
  Advice.h
  AutoCompleteDlg.h
  AutoCompleteListbox.h
  BibItem.h
  BibTeXDocument.h
  BiBTeXEntry.h
  BibTeXEntryType.h
  BiBTeXFile.h
  BibTeXOutputFilter.h
  BibTeXView.h
  BibView.h
  BookmarkEventArgs.h
  BookmarkView.h
  BrowseButton.h
  buildview.h
  CharType.h
  childfrm.h
  CodeBookmark.h
  CodeDocument.h
  CodeView.h
  configuration.h
  DdeCommand.h
  DeclareOption.h
  DialogTemplate.h
  DocumentNewDialog.h
  DocumentTokenizer.h
  DragSourceBase.h
  EditMenuButton.h
  EditMenuButtonOpt.h
  EncodingConverter.h
  EndOfLineMode.h
  environmentview.h
  ErrorListPane.h
  EventArgs.h
  FileChangeWatcher.h
  FileClean.h
  FileCleanConfirmDialog.h
  filegrep.h
  FileTreeCtrl.h
  FileViewEx.h
  FileViewPane.h
  findinfilesdialog.h
  FindReplaceDlg.h
  FontOccManager.h
  global.h
  gotodialog.h
  grepview.h
  IDocumentWizard.h
  insertfloatobjectdialog.h
  InsertGraphicDialog.h
  insertheaderdialog.h
  inserttabulardialog.h
  IProject.h
  IProjectWizard.h
  ItemIDList.h
  itempropertydialog.h
  LabelDragSource.h
  LaTeXCommand.h
  LaTeXDocument.h
  LaTeXDocumentBase.h
  LatexOutputFilter.h
  LatexProject.h
  LaTeXTokenizer.h
  LaTeXView.h
  LaTeXViewBase.h
  MainFrm.h
  MenuButton.h
  MetaPostDocument.h
  MetaPostView.h
  mrutrackselcombobox.h
  MultiExtensionDocTemplate.h
  navigatorview.h
  NewCommand.h
  NewEnvironment.h
  NotifyingEditListBox.h
  Nullable.h
  OleDrop.h
  optiondialog.h
  OptionPageEditor.h
  OptionPageFile.h
  OptionPageFileClean.h
  optionpagegeneric.h
  OptionPageLanguage.h
  optionpagepath.h
  OptionPageTextFormat.h
  OutputBuilder.h
  OutputDoc.h
  OutputFilter.h
  OutputInfo.h
  outputview.h
  OutputViewBase.h
  OutputViewVisitor.h
  OutputWizard.h
  OutputWizardDistributionPath.h
  OutputWizardFinish.h
  OutputWizardMikTex.h
  OutputWizardPage.h
  OutputWizardViewer.h
  OutputWizardWelcome.h
  PackageScanProgress.h
  ParseOutputView.h
  PersistPosEdit.h
  PlaceHolder.h
  PlaceholderSetsOptDlg.h
  PlaceholderSingleOptDlg.h
  PostProcessor.h
  PreProcessor.h
  PProcessor.h
  PrivateToolBar.h
  process.h
  ProcessCommand.h
  Profile.h
  ProfileDialog.h
  ProfileExchangeDialog.h
  ProfileNameDialog.h
  ProfilePage.h
  ProfilePageLatex.h
  ProfilePagePostprocessor.h
  ProfilePagePreprocessor.h
  ProfilePagePprocessor.h
  ProfilePageViewer.h
  Project.hpp
  ProjectNewDialog.h
  ProjectPropertyDialog.h
  ProjectSupportingWinApp.h
  ProjectTemplate.h
  ProjectView.h
  PropertyDialog.h
  PropertyPage.h
  RegistryStack.h
  resource.h
  RunTimeHelper.h
  ScintillaCtrl.h
  ScintillaDocView.h
  SearchToolBarEditButton.h
  SearchToolBarEditCtrl.h
  ShellUtilXP.h
  ShellView.h
  SortListCtrl.h
  SpellCheckDlg.h
  Speller.h
  SpellerBackgroundThread.h
  SpellerSource.h
  splash.h
  stdafx.h
  StructureItem.h
  StructureItemInfo.h
  structureparser.h
  StructureTreeCtrl.h
  structureview.h
  StyleFile.h
  StyleFileContainer.h
  TabPersistPosEdit.h
  targetver.h
  TemplateDialog.h
  TeXnicCenter.h
  textfilesavedialog.h
  TextModules.h
  TextModulesDlg.h
  TextOutsourceDlg.h
  textsource.h
  textsourcefile.h
  ThemedCtrl.h
  tipdlg.h
  ToolBarsCustomizeDialog.h
  TransparencyDlg.h
  TreeCtrlEx.h
  UserTool.h
  UserToolAdvDlg.h
  Version.h
  WorkspacePane.h
  WorkspacePaneBase.h
)

set (PROJECT_RES
  TeXnicCenter.rc
  "res/additional.bmp"
  "res/bibtextype.ico"
  "res/bibtoolbar.bmp"
  "res/bookmark view toolbar.bmp"
  "res/buildanimation.bmp"
  "res/document.bmp"
  "res/documenttypes.bmp"
  "res/documenttypeslarge.bmp"
  "res/errorlist.bmp"
  "res/fileview.bmp"
  "res/find.bmp"
  "res/format.bmp"
  "res/formatrecentlyused.bmp"
  "res/idr_late.ico"
  "res/images.bmp"
  "res/Incremental search.cur"
  "res/insert.bmp"
  "res/itemtypes.bmp"
  "res/latex.bmp"
  "res/LatexDoc.ico"
  "res/LatexDocOld.ico"
  "res/Lightbulb.bmp"
  "res/logo.bmp"
  "res/mainframe.bmp"
  "res/math.bmp"
  "res/mathbar1.bmp"
  "res/mathbar10.bmp"
  "res/mathbar11.bmp"
  "res/mathbar12.bmp"
  "res/mathbar13.bmp"
  "res/mathbar14.bmp"
  "res/mathbar15.bmp"
  "res/mathbar16.bmp"
  "res/mathbar17.bmp"
  "res/mathbar2.bmp"
  "res/mathbar3.bmp"
  "res/mathbar4.bmp"
  "res/mathbar5.bmp"
  "res/mathbar6.bmp"
  "res/mathbar7.bmp"
  "res/mathbar8.bmp"
  "res/mathbar9.bmp"
  "res/navigationbar.bmp"
  "res/outputbar.bmp"
  "res/Reverse incremental search.cur"
  "res/sdragcopy.cur"
  "res/separator.bmp"
  "res/singledrag.cur"
  "res/splash.bmp"
  "res/structuretoolbar.bmp"
)

include_directories (${CMAKE_SOURCE_DIR}/interfacewrapper)
include_directories (${CMAKE_SOURCE_DIR}/mfcext)
include_directories (${CMAKE_SOURCE_DIR}/msxmlcpp)
include_directories (${CMAKE_SOURCE_DIR}/objects)
include_directories (${CMAKE_SOURCE_DIR}/thirdparty/hunspell/src/hunspell)
include_directories (${CMAKE_SOURCE_DIR}/thirdparty/libiconv/include)
include_directories (${scintilla_INCLUDE_DIRS})
include_directories (${objects_BINARY_DIR}/objects.dir/${CMAKE_CFG_INTDIR})
include_directories (${HTML_HELP_INCLUDE_DIRS})

add_definitions (-DHUNSPELL_STATIC)

source_group ("Resource Files" FILES ${PROJECT_RES})

add_executable (${PROJECT_NAME} WIN32 ${PROJECT_CPP} ${PROJECT_RES}
  ${PROJECT_H})

# Setup precompiled headers
if (MSVC)
  set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -Yustdafx.h)
  set_source_files_properties (stdafx.cpp PROPERTIES COMPILE_FLAGS -Yc)
  set_source_files_properties (ItemIDList.cpp PROPERTIES COMPILE_FLAGS -Y-)
endif (MSVC)

target_link_libraries (${PROJECT_NAME} uxtheme objects hunspell libiconv mfcext
  msxmlcpp ${scintilla_LIBRARIES} htmlhelp interfacewrapper)

set_target_properties (${PROJECT_NAME} PROPERTIES OUTPUT_NAME TeXnicCenter)
