
# very basic makefile for timer32.exe
#
# Borland C++ tools
#
# BCROOT defines the root directory of your bc5 install
#
# optionally define SGIROOT to point to a copy of either the SGI
# or HP STL implimentation.
#



!ifndef BCROOT
BCROOT=$(MAKEDIR)\..
!endif

BCC32   = $(BCROOT)\bin\Bcc32.exe 
TLINK32 = $(BCROOT)\bin\TLink32.exe

IDE_LinkFLAGS32 =  -L$(BCROOT)\LIB
LINKOPTS= -Tpe -ap -c -x
!ifdef SGIROOT
COMPOPTS= -O2 -DSTL_NO_WSTRING; -I$(SGIROOT)$(BCROOT)\include;..\..\include;
!else
COMPOPTS= -DSTL_NO_WSTRING; -I$(BCROOT)\include;..\..\include;
!endif

timer32.exe : timer.obj
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LINKOPTS) +
$(BCROOT)\lib\c0x32.obj+
timer.obj+
$(BCROOT)\lib\import32.lib+
$(BCROOT)\lib\cw32.lib
$<,$*

|
timer.obj :  timer.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ timer.cpp
|













