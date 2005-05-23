@echo off
rem Build the regexpp library (simply calls the batch file)
rem $Id$
rem

cd ..\regexpp\lib
call vc6.bat
copy mre200dl.dll ..\..\output\Debug
copy mre200dl.ilk ..\..\output\Debug
copy mre200dl.pdb ..\..\output\Debug
copy mre200dl.dll ..\..\output\Release
cd ..\..\texniccenter