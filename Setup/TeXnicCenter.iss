[_ISTool]
EnableISX=true

[Setup]
OutputBaseFilename=TeXnicCenter Setup
MinVersion=4.0.950,4.0.1381
AppCopyright=© 1999-2007 www.TeXnicCenter.org
AppName=TeXnicCenter
AppVerName=TeXnicCenter Version 1 Beta 8
LicenseFile=GPL.rtf
PrivilegesRequired=none
DefaultGroupName=TeXnicCenter 8
AllowNoIcons=true
DefaultDirName={pf}\TeXnicCenter 8
UsePreviousAppDir=true
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
AppPublisher=TeXnicCenter.org
AppPublisherURL=http://www.TeXnicCenter.org/
AppSupportURL=http://www.TeXnicCenter.org/support/
AppUpdatesURL=http://www.TeXnicCenter.org/downloads/
AppVersion=1 Beta 8
AppID=TeXnicCenter
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

[Files]
Source: ..\output\Release\CrystalEdit.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\output\Release\GPL_english.htm; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\output\Release\GPL_german.htm; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\output\Release\MFCExt.dll; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\TeXnicCenter.exe; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\TeXnicCenter\Tips_english.txt; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\MsXmlCpp.dll; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\TeXnicCenter\Tips_german.txt; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\UserImages.bmp; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\TeXnicCenterObjects.tlb; DestDir: {app}; Flags: regtypelib promptifolder comparetimestamp; Components: Application_Files

Source: ..\output\Release\language\*.dll; DestDir: {app}\Language; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\language\*.aff; DestDir: {app}\Language; Components: Application_Files; Flags: promptifolder comparetimestamp
Source: ..\output\Release\language\*.dic; DestDir: {app}\Language; Components: Application_Files; Flags: promptifolder comparetimestamp
Source: ..\output\Release\Help\*.chm; DestDir: {app}\Help; Flags: promptifolder comparetimestamp; Components: Help_Files
Source: ..\output\Release\Help\*.chw; DestDir: {app}\Help; Flags: promptifolder comparetimestamp; Components: Help_Files

Source: ..\output\Release\*.xsd; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\packages\*.xml; DestDir: {app}\packages; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\packages\*.bmp; DestDir: {app}\packages; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: msxml4.dll; DestDir: {sys}; Flags: regserver sharedfile
Source: msxml4r.dll; DestDir: {sys}; Flags: sharedfile
Source: msxml4a.dll; DestDir: {sys}; Flags: sharedfile
DestDir: {app}\Templates\Documents\Deutsch; Source: ..\output\Release\Templates\Documents\Deutsch\*.tex; Components: " Templates"; Flags: comparetimestamp
DestDir: {app}\Templates\Documents\Deutsch erweitert; Source: ..\output\Release\Templates\Documents\Deutsch erweitert\*.tex; Components: " Templates"; Flags: comparetimestamp
DestDir: {app}\Templates\Documents\English; Source: ..\output\Release\Templates\Documents\English\*.tex; Components: " Templates"; Flags: comparetimestamp
DestDir: {app}\Templates\Documents\English advanced; Source: ..\output\Release\Templates\Documents\English advanced\*.tex; Components: " Templates"; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\English advanced; Source: ..\output\Release\Templates\Projects\English advanced\*.tex; Components: " Templates"; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\English; Source: ..\output\Release\Templates\Projects\English\*.tex; Components: " Templates"; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\Deutsch erweitert; Source: ..\output\Release\Templates\Projects\Deutsch erweitert\*.tex; Components: " Templates"; Flags: comparetimestamp
DestDir: {app}\Templates\Projects\Deutsch; Source: ..\output\Release\Templates\Projects\Deutsch\*.tex; Components: " Templates"; Flags: comparetimestamp
[Icons]
Name: {group}\TeXnicCenter; Filename: {app}\TeXnicCenter.exe; WorkingDir: {userdocs}; IconIndex: 0
Name: {group}\TeXnicCenter Help; Filename: {app}\Help\TeXCntr.chm; WorkingDir: {app}\Help; IconFilename: {win}\HH.EXE; IconIndex: 0; Components: Help_Files
Name: {userdesktop}\TeXnicCenter; Filename: {app}\TeXnicCenter.exe; WorkingDir: {userdocs}; Comment: TeXnicCenter; IconIndex: 0; Tasks: DesktopLink
Name: {sendto}\TeXnicCenter; Filename: {app}\TeXnicCenter.exe; Comment: TeXnicCenter; IconIndex: 0; Tasks: SendToLink; WorkingDir: {userdocs}

[Registry]
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter 8; ValueType: string; ValueName: CurrentVersion; ValueData: 1 Beta 7.01
Root: HKLM; Subkey: Software\ToolsCenter 8; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter 8; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenter 8; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenter 8\Settings\Options\ProjectTemplatePaths; ValueType: dword; ValueName: Size; ValueData: 1; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenter 8\Settings\Options\ProjectTemplatePaths; ValueType: string; ValueName: String0; ValueData: {app}\Templates\Projects; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter 8; ValueType: string; ValueName: AppPath; ValueData: {app}
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter 8\Templates; ValueType: string; ValueName: DefaultProjectTemplatePath; ValueData: {app}\Templates\Projects
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenter 8\Settings\Options\DocumentTemplatePaths; ValueType: dword; ValueName: Size; ValueData: 1; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenter 8\Settings\Options\DocumentTemplatePaths; ValueType: string; ValueName: String0; ValueData: {app}\Templates\Documents; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter 8\Templates; ValueType: string; ValueName: DefaultDocumentTemplatePath; ValueData: {app}\Templates\Documents

[Tasks]
Name: DesktopLink; Description: Create a link on the desktop; GroupDescription: Link; Flags: unchecked
Name: SendToLink; Description: Add TeXnicCenter to the 'Send To' menu; GroupDescription: Link

[Dirs]
Name: {app}\Templates\Documents; Flags: uninsalwaysuninstall
Name: {app}\Templates\Projects; Flags: uninsalwaysuninstall
Name: {app}\Help; Flags: uninsalwaysuninstall; Components: Help_Files

[Types]
Name: typical; Description: Typical (Recommended)
Name: compact; Description: Compact (Requires less space)
Name: custom; Description: Custom (Lets you choose, which components to install); Flags: iscustom
[Code]
{
program Setup;

function InitializeSetup(): Boolean;
var
	oldversion: string;
begin

	Result:= true;
	if RegQueryStringValue(HKLM, 'Software\Wiegand\TeXnicCenter', 'CurrentVersion', oldversion) then begin
		Result:= MsgBox('Setup has found an older version of TeXnicCenter on your system.' #13#13 'It is recommended to uninstall older versions, before installing TeXnicCenter Version 1 Beta 5.00.' #13#13 'Would you like to cancel setup now to uninstall the older version of TeXnicCenter?', mbConfirmation, MB_YESNO) = IDNO;
	end;
end;

begin
end.
}
