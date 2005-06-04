@echo off
rem Build the regexpp library (simply calls the batch file)
rem $Id$
rem

cd ..\regexpp\lib
call vc6.bat

if not exist ..\..\output\Release (
md ..\..\output\Release
)
if not exist ..\..\output\Debug (
md ..\..\output\Debug
) 

copy mre200dl.dll ..\..\output\Debug
copy mre200dl.ilk ..\..\output\Debug
copy mre200dl.pdb ..\..\output\Debug
copy mre200dl.dll ..\..\output\Release
cd ..\..\texniccenter