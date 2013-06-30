;TeXnicCenter Inno Setup Script

;-------------------------------------------
; Some defines
;-------------------------------------------

;Uncomment this for x64
#define TARGET_x64

;Uncomment this for an Alpha Build
;#define ALPHA_BUILD

;For debugging the Inno preprocessor
;#define Debug

;-------------------------------------------
; TODO
;-------------------------------------------

;~ Setup for current user  versus  Setup for all users:
;~ We need elevated priveleges to write to HKLM and such.
;~ For the "all users" case: more or less like now. Except for HKCR -> needs to be some other like HKLM/Classes
;~ For "current user" case: HKCR -> HKCU/Classes. Not in "Program Files".

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

;~ We need that for the file name of the setup itself
#define APP_VERSION_NOSPACE StringChange(APP_VERSION, " ", "")


[Setup]
;Output
OutputDir=..\Output\Setup
OutputBaseFilename=TXCSetup_{#APP_VERSION_NOSPACE}_{#APP_PLATFORM}
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
Name: Dictionaries\English; Description: English (US & GB); Types: custom typical
Name: Dictionaries\Deutsch; Description: Deutsch; Types: custom typical
Name: Dictionaries\Francais; Description: Français (Moderne, Classique, Réforme 1990, Toutes variantes); Types: custom

[Files]
;Application Files
Source: ..\Output\Product\{#APP_PLATFORM}\Release\TeXnicCenter.exe; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\{#APP_PLATFORM}\Release\Shared.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
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

;MFC files. Copied as private assemblies. Windows will use globally installed versions, if they are installed.
Source: {#APP_PLATFORM}\msvcr100.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: {#APP_PLATFORM}\msvcp100.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: {#APP_PLATFORM}\mfc100u.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion

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
DestDir: {app}\Dictionaries; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Dictionaries\de*; Flags: comparetimestamp promptifolder; Components: Dictionaries\Deutsch
DestDir: {app}\Dictionaries; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Dictionaries\en*; Flags: comparetimestamp promptifolder; Components: Dictionaries\English
DestDir: {app}\Dictionaries; Source: ..\Output\Product\{#APP_PLATFORM}\Release\Dictionaries\fr*; Flags: comparetimestamp promptifolder; Components: Dictionaries\Francais

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

;Register with default programs

; tex
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.tex; ValueType: expandsz; ValueName: FriendlyTypeName; ValueData: "@""{app}\Shared.dll"",-1001"; Flags: UninsDeleteValue;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.tex\CLSID; ValueType: string; ValueData: "{{573345AA-E76C-44E0-8CC3-7392506E442A}";
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.tex\DefaultIcon; ValueType: string; ValueData: {app}\TeXnicCenter.exe,1;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.tex\shell\open\command; ValueType: string; ValueData: """{app}\TeXnicCenter.exe"" ""%1""";

; dtx
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.dtx; ValueType: expandsz; ValueName: FriendlyTypeName; ValueData: "@""{app}\Shared.dll"",-1003"; Flags: UninsDeleteValue;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.dtx\CLSID; ValueType: string; ValueData: "{{04409A22-D986-4AEE-9F0D-B344159F8A35}";
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.dtx\DefaultIcon; ValueType: string; ValueData: {app}\TeXnicCenter.exe,1;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.dtx\shell\open\command; ValueType: string; ValueData: """{app}\TeXnicCenter.exe"" ""%1""";

; ltx
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.ltx; ValueType: expandsz; ValueName: FriendlyTypeName; ValueData: "@""{app}\Shared.dll"",-1002"; Flags: UninsDeleteValue;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.ltx\CLSID; ValueType: string; ValueData: "{{0B48F570-9642-4A6C-A8F5-8F4D92CBEFA3}";
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.ltx\DefaultIcon; ValueType: string; ValueData: {app}\TeXnicCenter.exe,1;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.ltx\shell\open\command; ValueType: string; ValueData: """{app}\TeXnicCenter.exe"" ""%1""";

; sty
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.sty; ValueType: expandsz; ValueName: FriendlyTypeName; ValueData: "@""{app}\Shared.dll"",-1004"; Flags: UninsDeleteValue;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.sty\CLSID; ValueType: string; ValueData: "{{497E9AD0-F881-4AC2-91C3-4488717EB1FD}";
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.sty\DefaultIcon; ValueData: {app}\TeXnicCenter.exe,1;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.sty\shell\open\command; ValueType: string; ValueData: """{app}\TeXnicCenter.exe"" ""%1""";

; ins
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.ins; ValueType: expandsz; ValueName: FriendlyTypeName; ValueData: "@""{app}\Shared.dll"",-1005"; Flags: UninsDeleteValue;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.ins\CLSID; ValueType: string; ValueData: "{{ABFB8011-FA0A-4DAD-9F75-75852358F387}";
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.ins\DefaultIcon; ValueData: {app}\TeXnicCenter.exe,1;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.ins\shell\open\command; ValueType: string; ValueData: """{app}\TeXnicCenter.exe"" ""%1""";

; bib
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.bib; ValueType: expandsz; ValueName: FriendlyTypeName; ValueData: "@""{app}\Shared.dll"",-1006"; Flags: UninsDeleteValue;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.bib\CLSID; ValueType: string; ValueData: "{{C10CF936-2EE8-4A9D-8A64-E1B2FD03E133}";
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.bib\DefaultIcon; ValueData: {app}\TeXnicCenter.exe,1;
Root: HKLM; SubKey: SOFTWARE\Classes\TeXnicCenter2.AssocFile.bib\shell\open\command; ValueType: string; ValueData: """{app}\TeXnicCenter.exe"" ""%1""";

Root: HKLM; SubKey: SOFTWARE\TeXnicCenter2\Capabilities; ValueType: string; ValueName: ApplicationDescription; ValueData: "@""{app}\Shared.dll"",-1000"; Flags: UninsDeleteValue;
Root: HKLM; SubKey: SOFTWARE\TeXnicCenter2\Capabilities\FileAssociations; ValueType: string; ValueName: .tex; ValueData: TeXnicCenter2.AssocFile.tex;
Root: HKLM; SubKey: SOFTWARE\TeXnicCenter2\Capabilities\FileAssociations; ValueType: string; ValueName: .dtx; ValueData: TeXnicCenter2.AssocFile.dtx;
Root: HKLM; SubKey: SOFTWARE\TeXnicCenter2\Capabilities\FileAssociations; ValueType: string; ValueName: .ltx; ValueData: TeXnicCenter2.AssocFile.ltx;
Root: HKLM; SubKey: SOFTWARE\TeXnicCenter2\Capabilities\FileAssociations; ValueType: string; ValueName: .sty; ValueData: TeXnicCenter2.AssocFile.sty;
Root: HKLM; SubKey: SOFTWARE\TeXnicCenter2\Capabilities\FileAssociations; ValueType: string; ValueName: .ins; ValueData: TeXnicCenter2.AssocFile.ins;
Root: HKLM; SubKey: SOFTWARE\TeXnicCenter2\Capabilities\FileAssociations; ValueType: string; ValueName: .bib; ValueData: TeXnicCenter2.AssocFile.bib;

Root: HKLM; SubKey: SOFTWARE\RegisteredApplications; ValueType: string; ValueName: TeXnicCenter; ValueData: SOFTWARE\TeXnicCenter2\Capabilities;

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
