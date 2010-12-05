#define APP_NAME          "TeXnicCenter"
#define APP_VERSION       "2.0 Alpha 3"
#define APP_ID            "TeXnicCenter Alpha"
#define REGNAME           "TeXnicCenterNT"
#define APP_COPYRIGHT     "© 1999-2010 www.TeXnicCenter.org"

#define APP_PLATFORM      "Win32"
;#define APP_PLATFORM      "x64"

[_ISTool]
EnableISX=true

[Setup]
;Output
OutputDir=..\Output\Setup
OutputBaseFilename=TXCSetup

;About TeXnicCenter
AppName={#APP_NAME}
AppVersion={#APP_VERSION}
AppVerName={#APP_NAME} Version {#APP_VERSION}
AppID={#APP_ID}
AppPublisher=The TeXnicCenter Team
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
PrivilegesRequired=none
MinVersion=4.0.950,5.0.2195

;Setup
AllowNoIcons=true
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
AlwaysShowComponentsList=true
SolidCompression=true
InternalCompressLevel=ultra
Compression=lzma/ultra
ShowLanguageDialog=yes

UninstallDisplayIcon={app}\TeXnicCenter.exe
WizardImageFile=compiler:wizmodernimage-IS.bmp
WizardSmallImageFile=compiler:wizmodernsmallimage-IS.bmp


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

;MFC files. Copied as private assemblies. Windows will us globally installed versions, if they are installed.
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

;Remove the GUI workspace, i.e., the toolbars and menus and short cuts etc.
; Use this only in Alpha versions!
Root: HKCU; Subkey: Software\ToolsCenter\{#REGNAME}\Workspace; Flags: deletekey


[Tasks]
Name: DesktopLink; Description: Create a desktop icon; GroupDescription: Link
Name: SendToLink; Description: Add TeXnicCenter to the 'Send To' menu; GroupDescription: Link; Flags: unchecked

[Dirs]
Name: {app}\Templates\Documents; Flags: uninsalwaysuninstall
Name: {app}\Templates\Projects; Flags: uninsalwaysuninstall
Name: {app}\Help; Flags: uninsalwaysuninstall; Components: Help_Files

[Types]
Name: typical; Description: Typical (Recommended)
Name: compact; Description: Compact (Requires less space)
Name: custom; Description: Custom (Lets you choose, which components to install); Flags: iscustom
