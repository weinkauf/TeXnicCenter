@echo off
rem start with a cleanup:

echo building with %1 > out.txt
del *.obj
del b2*.lib
del b2*.dll
del *.il?
del *.csm
%1 -fbc5.mak LIBNAME=b2re200 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
%1 -fbc5.mak LIBNAME=b2re200m MULTITHREAD=1 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
%1 -fbc5.mak LIBNAME=b2re200lm MULTITHREAD=1 DLL=1 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
del *.il?
%1 -fbc5.mak LIBNAME=b2re200l DLL=1 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t
del *.obj
del *.il?

goto exit

:handle_error
echo failed to build.
goto done


:exit

echo success!
echo a copy of all compiler messages has been placed in out.txt
echo

:done

del t.t











