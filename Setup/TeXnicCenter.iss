[_ISTool]
EnableISX=true

[Setup]
OutputBaseFilename=TXCSetup
MinVersion=4.0.950,4.0.1381
AppCopyright=© 1999-2001 www.ToolsCenter.org
AppName=TeXnicCenter
AppVerName=TeXnicCenter Version 1 Beta 5.05
LicenseFile=GPL.rtf
AdminPrivilegesRequired=false
DefaultGroupName=TeXnicCenter
DefaultDirName={pf}\TeXnicCenter
UsePreviousAppDir=true
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
AppPublisher=ToolsCenter
AppPublisherURL=www.ToolsCenter.org
AppSupportURL=www.ToolsCenter.org/products/texniccenter/support.htm
AppUpdatesURL=www.ToolsCenter.org
AppVersion=Version 1 Beta 5.05
AppID=TeXnicCenter
AlwaysShowComponentsList=false
UninstallDisplayIcon={app}\TEXCNTR.EXE

[Components]
Name: Application_Files; Description: Application Files (TeXnicCenter and its components); Flags: fixed; Types: compact typical custom
Name: System_Files; Description: System Files (Updates your system files - STRONGLY RECOMMENDED); Types: compact typical custom
Name: Help_Files; Description: Help Files (Online help for TeXnicCenter and LaTeX reference); Types: typical custom
Name: Skins; Description: Skins (Allows you to change the look & feel of TeXnicCenter); Types: typical custom

[Files]
Source: ..\output\Release\cryseditEx.dll; DestDir: {app}; CopyMode: alwaysoverwrite; Components: Application_Files
Source: ..\output\Release\GPL_english.htm; DestDir: {app}; CopyMode: alwaysoverwrite; Components: Application_Files
Source: ..\output\Release\GPL_german.htm; DestDir: {app}; CopyMode: alwaysoverwrite; Components: Application_Files
Source: ..\output\Release\MFCExt.dll; DestDir: {app}; CopyMode: normal; Flags: comparetimestampalso; Components: Application_Files
Source: ..\output\Release\TeXCntr.chm; DestDir: {app}; CopyMode: normal; Flags: comparetimestampalso; Components: Help_Files
Source: ..\output\Release\TEXCNTR.EXE; DestDir: {app}; CopyMode: alwaysoverwrite; Components: Application_Files
Source: ..\TeXnicCenter\Tips_english.txt; DestDir: {app}; CopyMode: normal; Flags: comparetimestampalso; Components: Application_Files
Source: ..\TeXnicCenter\Tips_german.txt; DestDir: {app}; CopyMode: normal; Flags: comparetimestampalso; Components: Application_Files
Source: ..\output\Release\UserImages.bmp; DestDir: {app}; CopyMode: normal; Flags: comparetimestampalso; Components: Application_Files
Source: ..\output\Release\mre200l.dll; DestDir: {sys}; CopyMode: normal; Flags: comparetimestampalso sharedfile; Components: Application_Files
Source: ..\output\Release\BCGCBResDEU.dll; DestDir: {sys}; CopyMode: normal; Flags: comparetimestampalso sharedfile; Components: Application_Files
Source: ..\output\Release\BCGSkinDownloader.dll; DestDir: {sys}; CopyMode: normal; Flags: comparetimestampalso sharedfile; Components: Application_Files
Source: ..\output\Release\BCGCB56.dll; DestDir: {sys}; CopyMode: normal; Flags: comparetimestampalso sharedfile; Components: Application_Files

Source: ..\..\..\WINDOWS\SYSTEM\MFC42.DLL; DestDir: {sys}; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall regserver; Components: System_Files
Source: ..\..\..\WINDOWS\SYSTEM\MSVCIRT.DLL; DestDir: {sys}; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall; Components: System_Files
Source: ..\..\..\WINDOWS\SYSTEM\MSVCRT.DLL; DestDir: {sys}; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall; Components: System_Files
Source: ..\..\..\WINDOWS\SYSTEM\MSVCP60.DLL; DestDir: {sys}; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall; Components: System_Files
Source: ..\TeXnicCenterObjects\Lib\TeXnicCenterObjects.tlb; DestDir: {app}; CopyMode: normal; Flags: comparetimestampalso regtypelib; Components: Application_Files
Source: ..\output\Release\Skins\*.dll; DestDir: {app}\Skins; CopyMode: normal; Flags: comparetimestampalso; Components: Skins
Source: ..\..\..\WINDOWS\SYSTEM\OLEAUT32.DLL; DestDir: {sys}; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall regserver; Components: System_Files

[Icons]
Name: {group}\TeXnicCenter; Filename: {app}\TEXCNTR.EXE; WorkingDir: {userdocs}; IconIndex: 0
Name: {group}\TeXnicCenter Help; Filename: {app}\TeXCntr.chm; WorkingDir: {app}; IconFilename: {win}\HH.EXE; IconIndex: 0
Name: {userdesktop}\TeXnicCenter; Filename: {app}\TEXCNTR.EXE; WorkingDir: {userdocs}; Comment: TeXnicCenter; IconIndex: 0; Tasks: DesktopLink
Name: {sendto}\TeXnicCenter; Filename: {app}\TEXCNTR.EXE; Comment: TeXnicCenter; IconIndex: 0; Tasks: SendToLink; WorkingDir: {userdocs}

[Registry]
Root: HKLM; Subkey: Software\ToolsCenter\TeXnicCenter; ValueType: string; ValueName: CurrentVersion; ValueData: 1 Beta 5.05
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
Name: {app}\Skins; Flags: uninsalwaysuninstall

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
