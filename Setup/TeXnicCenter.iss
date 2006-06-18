[_ISTool]
EnableISX=true

Use7zip=false
[Setup]
OutputBaseFilename=TXCSetup
MinVersion=4.0.950,4.0.1381
AppCopyright=© 1999-2006 www.TeXnicCenter.org
AppName=TeXnicCenter
AppVerName=TeXnicCenter Version 1 Beta 7.01 (Greengrass)
LicenseFile=GPL.rtf
AdminPrivilegesRequired=false
DefaultGroupName=TeXnicCenter
DefaultDirName={pf}\TeXnicCenter
UsePreviousAppDir=true
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
AppPublisher=TeXnicCenter.org
AppPublisherURL=www.TeXnicCenter.org
AppSupportURL=http://www.TeXnicCenter.org/support
AppUpdatesURL=http://www.TeXnicCenter.org/downloads
AppVersion=Version 1 Beta 7.01
AppID=TeXnicCenter
AlwaysShowComponentsList=false
UninstallDisplayIcon={app}\TEXCNTR.EXE

SolidCompression=true
ShowLanguageDialog=yes
WizardImageFile=compiler:wizmodernimage-IS.bmp
WizardSmallImageFile=compiler:wizmodernsmallimage-IS.bmp
[Components]
Name: Application_Files; Description: Application Files (TeXnicCenter and its components); Flags: fixed; Types: compact typical custom
Name: Help_Files; Description: Help Files (Online help for TeXnicCenter and LaTeX reference); Types: typical custom

[Files]
Source: ..\output\Release\cryseditEx.dll; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\output\Release\GPL_english.htm; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\output\Release\GPL_german.htm; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\output\Release\MFCExt.dll; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\TEXCNTR.EXE; DestDir: {app}; Components: Application_Files; Flags: ignoreversion
Source: ..\TeXnicCenter\Tips_english.txt; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\MsXmlCpp.dll; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\TeXnicCenter\Tips_german.txt; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\UserImages.bmp; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\mre200l.dll; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\BCGSkinDownloader.dll; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\output\Release\BCGCB63.dll; DestDir: {app}; Flags: promptifolder comparetimestamp; Components: Application_Files
Source: ..\TeXnicCenterObjects\Lib\TeXnicCenterObjects.tlb; DestDir: {app}; Flags: regtypelib promptifolder comparetimestamp; Components: Application_Files

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
[Icons]
Name: {group}\TeXnicCenter; Filename: {app}\TEXCNTR.EXE; WorkingDir: {userdocs}; IconIndex: 0
Name: {group}\TeXnicCenter Help; Filename: {app}\Help\TeXCntr.chm; WorkingDir: {app}\Help; IconFilename: {win}\HH.EXE; IconIndex: 0; Components: Help_Files
Name: {userdesktop}\TeXnicCenter; Filename: {app}\TEXCNTR.EXE; WorkingDir: {userdocs}; Comment: TeXnicCenter; IconIndex: 0; Tasks: DesktopLink
Name: {sendto}\TeXnicCenter; Filename: {app}\TEXCNTR.EXE; Comment: TeXnicCenter; IconIndex: 0; Tasks: SendToLink; WorkingDir: {userdocs}

[Registry]
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter; ValueType: string; ValueName: CurrentVersion; ValueData: 1 Beta 6.20
Root: HKLM; Subkey: Software\ToolsCenter; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenter; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenter\Settings\Options\ProjectTemplatePaths; ValueType: dword; ValueName: Size; ValueData: 1; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenter\Settings\Options\ProjectTemplatePaths; ValueType: string; ValueName: String0; ValueData: {app}\Templates\Projects; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter; ValueType: string; ValueName: AppPath; ValueData: {app}
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter\Templates; ValueType: string; ValueName: DefaultProjectTemplatePath; ValueData: {app}\Templates\Projects
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenter\Settings\Options\DocumentTemplatePaths; ValueType: dword; ValueName: Size; ValueData: 1; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKCU; Subkey: Software\ToolsCenter\TeXnicCenter\Settings\Options\DocumentTemplatePaths; ValueType: string; ValueName: String0; ValueData: {app}\Templates\Documents; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter\Templates; ValueType: string; ValueName: DefaultDocumentTemplatePath; ValueData: {app}\Templates\Documents

[Tasks]
Name: DesktopLink; Description: Create a link on the desktop; GroupDescription: Link; Flags: unchecked
Name: SendToLink; Description: Add TeXnicCenter to the 'Send To' menu; GroupDescription: Link

[Dirs]
Name: {app}\Templates\Documents; Flags: uninsalwaysuninstall
Name: {app}\Templates\Projects; Flags: uninsalwaysuninstall
Name: {app}\Help; Flags: uninsalwaysuninstall; Components: Help_Files

[Types]
Name: typical; Description: Typical (Will be useful for the most users)
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
