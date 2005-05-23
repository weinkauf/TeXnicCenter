@echo off
rem Build all help files and moves the created files resp. additional help files
rem to the appropriate output directories.
rem $Id$
rem

rem hhc returns 1, even if it was successful
@hhc TeXnicCenter.hhp
@if %errorlevel% GEQ 2 goto error

@cd ..\output
@echo "Creating help directories..."
if not exist .\Release\help (
md .\Release\help
)
if not exist .\Debug\help (
md .\Debug\help
) 
@echo "Copying created help files..."
@for %%i in (*.chm;*.chw) do copy %%i .\Debug\help
@for %%i in (*.chm;*.chw) do copy %%i .\Release\help
@cd ..\texniccenter
@echo "Copying additional help files..."
@for %%i in (GPL_english.htm GPL_german.htm Tips_english.txt Tips_german.txt UserImages.bmp) do copy %%i ..\output\Debug
@for %%i in (GPL_english.htm GPL_german.htm Tips_english.txt Tips_german.txt UserImages.bmp) do copy %%i ..\output\Release

cd ..\TCHelp
goto finish
:error
@echo "** Build failed! **"
:finish