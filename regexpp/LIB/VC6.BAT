@echo off
rem start with a cleanup:

echo building with nmake > out.txt
del *.obj
del m*.lib
del m*.dll
del *.idb
del *.pdb
del *.pch
nmake -fvc6.mak CFG="Release" LIBNAME=mre200 > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
del *.pch
nmake -fvc6.mak CFG="Release" LIBNAME=mre200m MULTITHREAD="1" > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
del *.pch
nmake -fvc6.mak CFG="Debug" LIBNAME=mre200d > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
del *.pch
nmake -fvc6.mak CFG="Debug" LIBNAME=mre200dm MULTITHREAD="1" > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
del *.pch
nmake -fvc6dll.mak CFG="Debug" LIBNAME=mre200dl MULTITHREAD="1" > t.t
if errorlevel goto handle_error
copy/b out.txt,+t.t out.txt
type t.t

del *.obj
del *.pch
nmake -fvc6dll.mak CFG="Release" LIBNAME=mre200l > t.t
if errorlevel goto handle_error
del *.obj
del *.pch
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


