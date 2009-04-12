@echo off
REM -- Zuerst Zuordnungsdatei der mit Microsoft Visual C++ erzeugten resource.h erstellen
REM -- First, create TeXnicCenter.hm from the Microsoft Visual C++ resource.h file
echo // Von MAKEHELP.BAT erzeugte Hilfe-Zuordnungsdatei. Wird verwendet von TEXNICCENTER.HPJ. >"hlp\TeXnicCenter.hm"
echo. >>"hlp\TeXnicCenter.hm"
echo // Befehle (ID_* und IDM_*) >>"hlp\TeXnicCenter.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\TeXnicCenter.hm"
echo. >>"hlp\TeXnicCenter.hm"
echo // Eingabeaufforderungen (IDP_*) >>"hlp\TeXnicCenter.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\TeXnicCenter.hm"
echo. >>"hlp\TeXnicCenter.hm"
echo // Ressourcen (IDR_*) >>"hlp\TeXnicCenter.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\TeXnicCenter.hm"
echo. >>"hlp\TeXnicCenter.hm"
echo // Dialogfelder (IDD_*) >>"hlp\TeXnicCenter.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\TeXnicCenter.hm"
echo. >>"hlp\TeXnicCenter.hm"
echo // Rahmen-Steuerelemente (IDW_*) >>"hlp\TeXnicCenter.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\TeXnicCenter.hm"
REM -- Hilfe erstellen für Projekt TEXNICCENTER
REM -- Create help files for TEXNICCENTER project

echo Erstelle Win32-Hilfedateien
echo Creating Win32 help files
start /wait hcw /C /E /M "hlp\TeXnicCenter.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\TeXnicCenter.hlp" goto :Error
if not exist "hlp\TeXnicCenter.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\TeXnicCenter.hlp" Debug
if exist Debug\nul copy "hlp\TeXnicCenter.cnt" Debug
if exist Release\nul copy "hlp\TeXnicCenter.hlp" Release
if exist Release\nul copy "hlp\TeXnicCenter.cnt" Release
if exist "Unicode Debug" copy "hlp\TeXnicCenter.hlp" "Unicode Debug"
if exist "Unicode Debug" copy "hlp\TeXnicCenter.cnt" "Unicode Debug"
if exist "Unicode Release" copy "hlp\TeXnicCenter.hlp" "Unicode Release"
if exist "Unicode Release" copy "hlp\TeXnicCenter.cnt" "Unicode Release"
echo.
goto :done

:Error
echo hlp\TeXnicCenter.hpj(1) : Fehler: Problem beim Erstellen der Hilfedatei festgestellt
echo hlp\TeXnicCenter.hpj(1) : Error: Problem creating help files
:done
echo.
