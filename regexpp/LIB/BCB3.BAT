@echo off
rem start with a cleanup:

echo building with %1 > out.txt
del *.obj
del b3*.lib
del b3*.dll
del *.il?
del *.csm
%1 -fbcb3.mak LIBNAME=b3re200 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
%1 -fbcb3.mak LIBNAME=b3re200m MULTITHREAD=1 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
%1 -fbcb3.mak LIBNAME=b3re200lm MULTITHREAD=1 DLL=1 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
del *.il?
%1 -fbcb3.mak LIBNAME=b3re200l DLL=1 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
del *.il?
%1 -fbcb3.mak LIBNAME=b3re200lv DLL=1 VCL=1 MULTITHREAD=1 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
del *.il?
%1 -fbcb3.mak LIBNAME=b3re200v VCL=1 MULTITHREAD=1 > t.t
if errorlevel goto handle_error
del *.obj
del *.il?
copy/b out.txt,+t.t out.txt
type t.t

goto exit

:handle_error
echo failed to build.
echo a copy of all compiler messages has been placed in out.txt
echo
goto done

:exit

echo success!
echo a copy of all compiler messages has been placed in out.txt
echo

:done
del t.t







