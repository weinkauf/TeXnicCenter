@echo off
REM $Id$


call MakeClean.bat


echo                                        -
echo                                        -
echo ----------------------------------------
echo                                        -
echo Running doxygen

D:\Programme\doxygen\bin\doxygen.exe txc.doxy
C:\Programme\doxygen\bin\doxygen.exe txc.doxy
"D:\Program Files\doxygen\bin\doxygen.exe" txc.doxy
"C:\Program Files\doxygen\bin\doxygen.exe" txc.doxy

echo                                        -
echo                                        -
echo If this failed totally, then edit the
echo Batch-File and ADD your path to doxygen to it.
echo Do NOT remove other paths!
echo                                        -
echo ----------------------------------------




echo                                        -
echo                                        -
echo ----------------------------------------
echo                                        -
echo Finishing the HTML-Creation

copy head.png html\head.png

echo                                        -
echo ----------------------------------------



echo                                        -
echo                                        -
echo ----------------------------------------
echo                                        -
echo Running HTML Help Workshop

"D:\Programme\HTML Help Workshop\hhc.exe" html\index.hhp
"C:\Programme\HTML Help Workshop\hhc.exe" html\index.hhp
"D:\Program Files\HTML Help Workshop\hhc.exe" html\index.hhp
"C:\Program Files\HTML Help Workshop\hhc.exe" html\index.hhp

echo                                        -
echo                                        -
echo If this failed totally, then edit the
echo Batch-File and ADD your path to hhc to it.
echo Do NOT remove other paths!
echo                                        -
echo ----------------------------------------



pause

