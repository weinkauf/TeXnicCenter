@echo off
rem start with a cleanup:

echo building with %1 > out.txt
del *.obj
del b2*.lib
del b2*.dll
del *.il?
del *.csm
%1 -fbcb1.mak LIBNAME=b2re200lv DLL=1 VCL=1 MULTITHREAD=1 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
del *.il?
%1 -fbcb1.mak LIBNAME=b2re200v VCL=1 MULTITHREAD=1 >t.t
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








