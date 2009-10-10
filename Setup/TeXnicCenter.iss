[_ISTool]
EnableISX=true

[Setup]
OutputDir=..\Output\Setup
OutputBaseFilename=TXCSetup
MinVersion=4.0.950,5.0.2195
AppCopyright=© 1999-2009 www.TeXnicCenter.org
AppName=TeXnicCenter2
AppVerName=TeXnicCenter Version 2.0 Alpha 1
LicenseFile=GPL.rtf
PrivilegesRequired=none
DefaultGroupName=TeXnicCenter2
AllowNoIcons=true
DefaultDirName={pf}\TeXnicCenter 2 Alpha 1
UsePreviousAppDir=true
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
AppPublisher=The TeXnicCenter Team
AppPublisherURL=http://www.texniccenter.org/
AppSupportURL=http://www.texniccenter.org/support/find-help
AppUpdatesURL=http://www.texniccenter.org/resources/downloads
AppVersion=Version 2.0 Alpha 1
AppID=TeXnicCenter2
AlwaysShowComponentsList=true
UninstallDisplayIcon={app}\TeXnicCenter.exe

SolidCompression=true
InternalCompressLevel=ultra
Compression=lzma/ultra

ShowLanguageDialog=yes
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
Source: ..\Output\Product\Unicode Release\GPL_english.htm; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\Unicode Release\GPL_german.htm; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\Unicode Release\MFCExt.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\Unicode Release\TeXnicCenter.exe; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\Unicode Release\MsXmlCpp.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\Unicode Release\Tips_english.txt; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\Unicode Release\Tips_german.txt; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\Unicode Release\TeXnicCenterObjects.tlb; DestDir: {app}; Components: Application_Files; Flags: regtypelib promptifolder comparetimestamp

Source: ..\Output\Product\Unicode Release\UserImages.bmp; DestDir: {app}; Components: Application_Files; Flags: promptifolder comparetimestamp

Source: ..\Output\Product\Unicode Release\Language\*.dll; DestDir: {app}\Language; Components: Application_Files; Flags: ignoreversion
Source: ..\Output\Product\Unicode Release\Help\*.chm; DestDir: {app}\Help; Components: Help_Files; Flags: ignoreversion
Source: ..\Output\Product\Unicode Release\Help\*.chw; DestDir: {app}\Help; Components: Help_Files; Flags: ignoreversion

Source: ..\Output\Product\Unicode Release\*.xsd; DestDir: {app}; Components: Application_Files; Flags: promptifolder comparetimestamp
Source: ..\Output\Product\Unicode Release\Packages\*.xml; DestDir: {app}\Packages; Components: Application_Files; Flags: promptifolder comparetimestamp
Source: ..\Output\Product\Unicode Release\Packages\*.bmp; DestDir: {app}\Packages; Components: Application_Files; Flags: promptifolder comparetimestamp
Source: msxml4.dll; DestDir: {sys}; Flags: regserver sharedfile
Source: msxml4r.dll; DestDir: {sys}; Flags: sharedfile
Source: msxml4a.dll; DestDir: {sys}; Flags: sharedfile

DestDir: {app}\Templates\Documents\English; Source: ..\Output\Product\Unicode Release\Templates\Documents\English\*.tex; Components: Templates\English; Flags: comparetimestamp
DestDir: {app}\Templates\Documents\English advanced; Source: ..\Output\Product\Unicode Release\Templates\Documents\English advanced\*.tex; Components: Templates\English; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\English advanced; Source: ..\Output\Product\Unicode Release\Templates\Projects\English advanced\*.tex; Components: Templates\English; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\English; Source: ..\Output\Product\Unicode Release\Templates\Projects\English\*.tex; Components: Templates\English; Flags: comparetimestamp

DestDir: {app}\Templates\Documents\Deutsch; Source: ..\Output\Product\Unicode Release\Templates\Documents\Deutsch\*.tex; Components: Templates\Deutsch; Flags: comparetimestamp
DestDir: {app}\Templates\Documents\Deutsch erweitert; Source: ..\Output\Product\Unicode Release\Templates\Documents\Deutsch erweitert\*.tex; Components: Templates\Deutsch; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\Deutsch erweitert; Source: ..\Output\Product\Unicode Release\Templates\Projects\Deutsch erweitert\*.tex; Components: Templates\Deutsch; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\Deutsch; Source: ..\Output\Product\Unicode Release\Templates\Projects\Deutsch\*.tex; Components: Templates\Deutsch; Flags: comparetimestamp

DestDir: {app}\Dictionaries; Source: ..\Output\Product\Unicode Release\Dictionaries\de_DE.*; Flags: comparetimestamp promptifolder; Components: Dictionaries\Deutsch
DestDir: {app}\Dictionaries; Source: ..\Output\Product\Unicode Release\Dictionaries\en_GB.*; Flags: comparetimestamp promptifolder; Components: Dictionaries\EnglishGB
DestDir: {app}\Dictionaries; Source: ..\Output\Product\Unicode Release\Dictionaries\en_US.*; Flags: comparetimestamp promptifolder; Components: Dictionaries\EnglishUS
DestDir: {app}\Dictionaries; Source: ..\Output\Product\Unicode Release\Dictionaries\fr_FR.*; Flags: comparetimestamp promptifolder; Components: Dictionaries\Francais
DestDir: {app}\Dictionaries; Source: ..\Output\Product\Unicode Release\Dictionaries\fr_FR-1990.*; Flags: comparetimestamp promptifolder; Components: " Dictionaries\Francais1990"

[Icons]
Name: {group}\TeXnicCenter; Filename: {app}\TeXnicCenter.exe; WorkingDir: {userdocs}; IconIndex: 0
Name: {group}\TeXnicCenter Help; Filename: {app}\Help\TeXnicCenter.chm; WorkingDir: {app}\Help; IconFilename: {win}\HH.EXE; IconIndex: 0; Components: Help_Files
Name: {userdesktop}\TeXnicCenter; Filename: {app}\TeXnicCenter.exe; WorkingDir: {userdocs}; Comment: TeXnicCenter; IconIndex: 0; Tasks: DesktopLink
Name: {sendto}\TeXnicCenter; Filename: {app}\TeXnicCenter.exe; Comment: TeXnicCenter; IconIndex: 0; Tasks: SendToLink; WorkingDir: {userdocs}

[Registry]
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenterNT; ValueType: string; ValueName: CurrentVersion; ValueData: 2.0 Alpha 1
Root: HKLM; Subkey: Software\ToolsCenterNT; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenterNT; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenterNT; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenterNT; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenterNT\Settings\Options\ProjectTemplatePaths; ValueType: dword; ValueName: Size; ValueData: 1; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenterNT\Settings\Options\ProjectTemplatePaths; ValueType: string; ValueName: String0; ValueData: {app}\Templates\Projects; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenterNT; ValueType: string; ValueName: AppPath; ValueData: {app}
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenterNT\Templates; ValueType: string; ValueName: DefaultProjectTemplatePath; ValueData: {app}\Templates\Projects
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenterNT\Settings\Options\DocumentTemplatePaths; ValueType: dword; ValueName: Size; ValueData: 1; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenterNT\Settings\Options\DocumentTemplatePaths; ValueType: string; ValueName: String0; ValueData: {app}\Templates\Documents; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenterNT\Templates; ValueType: string; ValueName: DefaultDocumentTemplatePath; ValueData: {app}\Templates\Documents

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
