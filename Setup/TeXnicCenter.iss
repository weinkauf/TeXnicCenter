;TeXnicCenter Inno Setup Script

;-------------------------------------------
; Some defines
;-------------------------------------------

;Uncomment this for x64
;#define TARGET_x64

;Uncomment this for an Alpha Build
#define ALPHA_BUILD

;For debugging the Inno preprocessor
;#define Debug

;-------------------------------------------
; TODO
;-------------------------------------------

;~ Setup for current user  versus  Setup for all users:
;~ We need elevated priveleges to write to HKLM and such.
;~ For the "all users" case: more or less like now. Except for HKCR -> needs to be some other like HKLM/Classes
;~ For "current user" case: copy msxml to local dir (maybe do this always?). HKCR -> HKCU/Classes. Not in "Program Files".

;~ msxml is weird. Maybe deliver an appropriate installer? Would that require admin privileges?

;~ The defaults for the doc and project templates should really be handled from within TXC.
;~ If the key or dir doesn't exist, then use the defaults.

;~ Artwork: Setup Icon, Setup wizard image




;-------------------------------------------
; Start of the actual script
;-------------------------------------------

#ifdef TARGET_x64
  #define APP_PLATFORM      "x64"
#else
  #define APP_PLATFORM      "Win32"
#endif

#define APP_NAME          GetStringFileInfo("..\Output\Product\" + APP_PLATFORM + "\Release\TeXnicCenter.exe", PRODUCT_NAME)
#define APP_VERSION       GetStringFileInfo("..\Output\Product\" + APP_PLATFORM + "\Release\TeXnicCenter.exe", PRODUCT_VERSION)
#define APP_PUBLISHER     GetStringFileInfo("..\Output\Product\" + APP_PLATFORM + "\Release\TeXnicCenter.exe", COMPANY_NAME)
#define APP_COPYRIGHT     GetStringFileInfo("..\Output\Product\" + APP_PLATFORM + "\Release\TeXnicCenter.exe", LEGAL_COPYRIGHT)
#define REGNAME           "TeXnicCenterNT"

;Alpha Builds can be used side-by-side with stable builds.
;Hence, they go by default into other directories.
#ifdef ALPHA_BUILD
  #define APP_ID            APP_NAME + " Alpha"
#else
  #define APP_ID            APP_NAME
#endif


[Setup]
;Output
OutputDir=..\Output\Setup
OutputBaseFilename=TXCSetup
;About TeXnicCenter
AppName={#APP_NAME}
AppVersion={#APP_VERSION}
AppVerName={#APP_NAME} Version {#APP_VERSION}
AppID={#APP_ID}
AppPublisher={#APP_PUBLISHER}
AppPublisherURL=http://www.texniccenter.org/
AppSupportURL=http://www.texniccenter.org/support/find-help
AppUpdatesURL=http://www.texniccenter.org/resources/downloads
AppCopyright={#APP_COPYRIGHT}
LicenseFile=GPL.rtf
DefaultDirName={pf}\{#APP_ID}
DefaultGroupName={#APP_ID}
;Previous
UsePreviousAppDir=true
;Requirements
;PrivilegesRequired=none
MinVersion=4.0.950,5.0.2195
;Setup
AllowNoIcons=true
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
AlwaysShowComponentsList=true
ShowLanguageDialog=yes
UninstallDisplayIcon={app}\TeXnicCenter.exe
WizardImageFile=compiler:wizmodernimage-IS.bmp
WizardSmallImageFile=compiler:wizmodernsmallimage-IS.bmp
ChangesAssociations=true
;Compression
SolidCompression=true
InternalCompressLevel=Ultra64
;Compression=none
Compression=lzma2/Ultra64
;Architecture (don't use these for Win32, since 32bit code can run on x64 as well. See Inno Help.)
#ifdef TARGET_x64
  ArchitecturesAllowed=x64
  ArchitecturesInstallIn64BitMode=x64
#endif


[Components]
Name: Application_Files; Description: Application Files (TeXnicCenter core components); Flags: fixed; Types: compact typical custom
Name: Help_Files; Description: Help Files (Help for TeXnicCenter and LaTeX reference); Types: typical custom
Name: Templates; Description: LaTeX Templates (Templates for creating new projects and documents); Types: custom typical
Name: Templates\English; Description: English LaTeX Templates; Types: custom typical
Name: Templates\Deutsch; Description: Deutsche LaTeX Vorlagen; Types: custom typical
Name: Dictionaries; Description: Dictionaries; Types: custom typical
Name: Dictionaries\EnglishGB; Description: English (GB); Types: custom
Name: Dictionaries\EnglishUS; Description: English (US); Types: custom typical
Name: Dictionaries\Deutsch; Description: Deutsch; Types: custom typical
Name: Dictionaries\Francais; Description: Français; Types: custom
Name: Dictionaries\Francais1990; Description: Français (appelé Réforme 1990); Types: custom

[Files]
;Application Files
Source: ..\Output\Product\{#APP_PLATFORM}\Release\TeXnicCenter.exe; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\{#APP_PLATFORM}\Release\MFCExt.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\{#APP_PLATFORM}\Release\MsXmlCpp.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\{#APP_PLATFORM}\Release\SciLexer.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\{#APP_PLATFORM}\Release\Language\*.dll; DestDir: {app}\Language; Components: Application_Files; Flags: ignoreversion

;The type lib for creating wizards. We do not issue an error, if registering doesn't work.
Source: ..\Output\Product\{#APP_PLATFORM}\Release\TeXnicCenterObjects.tlb; DestDir: {app}; Components: Application_Files; Flags: regtypelib promptifolder comparetimestamp noregerror

;Some other files
Source: ..\Output\Product\{#APP_PLATFORM}\Release\GPL_english.htm; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\{#APP_PLATFORM}\Release\GPL_german.htm; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\{#APP_PLATFORM}\Release\Tips_english.txt; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\{#APP_PLATFORM}\Release\Tips_german.txt; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\{#APP_PLATFORM}\Release\UserImages.bmp; DestDir: {app}; Components: Application_Files; Flags: promptifolder comparetimestamp

;Packages and other XML files
Source: ..\Output\Product\{#APP_PLATFORM}\Release\*.xsd; DestDir: {app}; Components: Application_Files; Flags: promptifolder comparetimestamp
Source: ..\Output\Product\{#APP_PLATFORM}\Release\Packages\*.xml; DestDir: {app}\Packages; Components: Application_Files; Flags: promptifolder comparetimestamp
Source: ..\Output\Product\{#APP_PLATFORM}\Release\Packages\*.bmp; DestDir: {app}\Packages; Components: Application_Files; Flags: promptifolder comparetimestamp

;Help
Source: ..\Output\Product\{#APP_PLATFORM}\Release\Help\*.chm; DestDir: {app}\Help; Components: Help_Files; Flags: ignoreversion
Source: ..\Output\Product\{#APP_PLATFORM}\Release\Help\*.chw; DestDir: {app}\Help; Components: Help_Files; Flags: ignoreversion

;XML System files - do we really need this? For an admin account on XP, we don't need these files at all.
;For a regular user, we need these files in the systems dir. A simple copy to the app dir doesn't work. What am I missing?
Source: msxml4.dll; DestDir: {sys}; Flags: regserver sharedfile
Source: msxml4r.dll; DestDir: {sys}; Flags: sharedfile
Source: msxml4a.dll; DestDir: {sys}; Flags: sharedfile
;Source: msxml4.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
;Source: msxml4r.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
;Source: msxml4a.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion

;MFC files. Copied as private assemblies. Windows will use globally installed versions, if they are installed.
Source: msvcr100.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: msvcp100.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: mfc100u.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion

;Templates
DestDir: {app}\Templates\Documents\English; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Templates\Documents\English\*.tex; Components: Templates\English; Flags: comparetimestamp
DestDir: {app}\Templates\Documents\English advanced; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Templates\Documents\English advanced\*.tex; Components: Templates\English; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\English advanced; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Templates\Projects\English advanced\*.tex; Components: Templates\English; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\English; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Templates\Projects\English\*.tex; Components: Templates\English; Flags: comparetimestamp

DestDir: {app}\Templates\Documents\Deutsch; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Templates\Documents\Deutsch\*.tex; Components: Templates\Deutsch; Flags: comparetimestamp
DestDir: {app}\Templates\Documents\Deutsch erweitert; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Templates\Documents\Deutsch erweitert\*.tex; Components: Templates\Deutsch; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\Deutsch erweitert; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Templates\Projects\Deutsch erweitert\*.tex; Components: Templates\Deutsch; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\Deutsch; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Templates\Projects\Deutsch\*.tex; Components: Templates\Deutsch; Flags: comparetimestamp

;Dictionaries
DestDir: {app}\Dictionaries; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Dictionaries\de_DE.*; Flags: comparetimestamp promptifolder; Components: Dictionaries\Deutsch
DestDir: {app}\Dictionaries; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Dictionaries\en_GB.*; Flags: comparetimestamp promptifolder; Components: Dictionaries\EnglishGB
DestDir: {app}\Dictionaries; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Dictionaries\en_US.*; Flags: comparetimestamp promptifolder; Components: Dictionaries\EnglishUS
DestDir: {app}\Dictionaries; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Dictionaries\fr_FR.*; Flags: comparetimestamp promptifolder; Components: Dictionaries\Francais
DestDir: {app}\Dictionaries; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Dictionaries\fr_FR-1990.*; Flags: comparetimestamp promptifolder; Components: " Dictionaries\Francais1990"

[Icons]
Name: {group}\{#APP_ID}; Filename: {app}\TeXnicCenter.exe; WorkingDir: {userdocs}; IconIndex: 0
Name: {group}\TeXnicCenter Help; Filename: {app}\Help\TeXnicCenter.chm; WorkingDir: {app}\Help; IconFilename: {win}\HH.EXE; IconIndex: 0; Components: Help_Files
Name: {userdesktop}\{#APP_ID}; Filename: {app}\TeXnicCenter.exe; WorkingDir: {userdocs}; Comment: TeXnicCenter; IconIndex: 0; Tasks: DesktopLink
Name: {sendto}\{#APP_ID}; Filename: {app}\TeXnicCenter.exe; Comment: TeXnicCenter; IconIndex: 0; Tasks: SendToLink; WorkingDir: {userdocs}

[Registry]
;Write infos about the installation such that all other programs can access them
Root: HKLM; Subkey: Software\ToolsCenter; Flags: uninsdeletekeyifempty noerror
Root: HKLM; Subkey: Software\ToolsCenter\{#REGNAME}; Flags: uninsdeletekey noerror
Root: HKLM; Subkey: Software\ToolsCenter\{#REGNAME}; ValueType: string; ValueName: AppPath; ValueData: {app}; Flags: noerror
Root: HKLM; Subkey: Software\ToolsCenter\{#REGNAME}; ValueType: string; ValueName: CurrentVersion; ValueData: {#APP_VERSION}; Flags: noerror

;Add the templates for the current user
Root: HKCU; Subkey: Software\ToolsCenter; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: Software\ToolsCenter\{#REGNAME}; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\{#REGNAME}\Settings\Options\ProjectTemplatePaths; ValueType: dword; ValueName: Size; ValueData: 1; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\{#REGNAME}\Settings\Options\ProjectTemplatePaths; ValueType: string; ValueName: String0; ValueData: {app}\Templates\Projects; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\{#REGNAME}\Settings\Options\DocumentTemplatePaths; ValueType: dword; ValueName: Size; ValueData: 1; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\{#REGNAME}\Settings\Options\DocumentTemplatePaths; ValueType: string; ValueName: String0; ValueData: {app}\Templates\Documents; Flags: createvalueifdoesntexist uninsdeletekey

;Reset the GUI workspace, i.e., the toolbars and menus and short cuts etc., if desired by the user
Root: HKCU; Subkey: Software\ToolsCenter\{#REGNAME}\Workspace; Flags: deletekey; Tasks: ResetWorkspace;

;Register file type: tex ; if the user wants it
Root: HKCR; SubKey: .tex; ValueType: string; ValueData: TeXnicCenter.tex; Flags: UninsDeleteValue; Tasks: FileAssociations; 
Root: HKCR; SubKey: TeXnicCenter.tex; ValueType: string; ValueData: "LaTeX Document"; Flags: UninsDeleteKey; Tasks: FileAssociations; 
Root: HKCR; SubKey: TeXnicCenter.tex\DefaultIcon; ValueType: string; ValueData: {app}\TeXnicCenter.exe,1; Tasks: FileAssociations;
Root: HKCR; SubKey: TeXnicCenter.tex\shell\open\command; ValueType: string; ValueData: """{app}\TeXnicCenter.exe"" ""%1"""; Tasks: FileAssociations;

;Register file type: tcp ; always (for now) 
Root: HKCR; SubKey: .tcp; ValueType: string; ValueData: TeXnicCenter.tcp; Flags: UninsDeleteValue; 
Root: HKCR; SubKey: TeXnicCenter.tcp; ValueType: string; ValueData: "TeXnicCenter Project"; Flags: UninsDeleteKey; 
Root: HKCR; SubKey: TeXnicCenter.tcp\DefaultIcon; ValueType: string; ValueData: {app}\TeXnicCenter.exe,3; 
Root: HKCR; SubKey: TeXnicCenter.tcp\shell\open\command; ValueType: string; ValueData: """{app}\TeXnicCenter.exe"" ""%1""";


[Tasks]
Name: DesktopLink; Description: Create a desktop icon; GroupDescription: Links and Associations
Name: SendToLink; Description: Add TeXnicCenter to the 'Send To' menu; GroupDescription: Links and Associations; Flags: unchecked
Name: FileAssociations; Description: "Make TeXnicCenter the default LaTeX Editor"; GroupDescription: Links and Associations; 
Name: ResetWorkspace; Description: "Reset customizations (menus, toolbars, shortcuts) [recommended]"; GroupDescription: "Customizations of previously installed versions of TeXnicCenter 2"; 

[Dirs]
Name: {app}\Templates\Documents; Flags: uninsalwaysuninstall
Name: {app}\Templates\Projects; Flags: uninsalwaysuninstall
Name: {app}\Help; Flags: uninsalwaysuninstall; Components: Help_Files

[Types]
Name: typical; Description: Typical (Recommended)
Name: compact; Description: Compact (Requires less space)
Name: custom; Description: Custom (Lets you choose, which components to install); Flags: iscustom

[Run]
Filename: {app}\TeXnicCenter.exe; Description: "Launch TeXnicCenter"; Flags: postinstall nowait skipifsilent

[InnoIDE_Settings]
LogFileOverwrite=false



#ifdef Debug
  #expr SaveToFile(AddBackslash(SourcePath) + "Preprocessed.iss")
#endif
