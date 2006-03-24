@echo off
rem Copy all package files to the appropriate output directories
rem $Id$
rem
@echo Create directories...
if not exist ..\output\release\packages (md ..\output\release\packages)
if not exist ..\output\debug\packages (md ..\output\debug\packages)

@echo Copying package files...
cd packages
copy *.xml ..\..\output\debug\packages
copy *.xml ..\..\output\release\packages
copy *.bmp ..\..\output\debug\packages
copy *.bmp ..\..\output\release\packages
cd ..
