@echo off
REM $Id$


echo ----------------------------------------
echo                                        -
echo Removing all previous doxygen output
echo                                        -
echo ----------------------------------------

del html\* /s /q
del TXCSourceCodeDoc.chm /q
del TXCSourceCodeDoc.chi /q
del warnings.log /q

echo                                        -
echo                                        -
echo Removed all previous doxygen output
echo                                        -
echo ----------------------------------------

