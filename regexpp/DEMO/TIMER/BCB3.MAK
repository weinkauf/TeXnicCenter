
# very basic makefile for timer.exe
#
# Borland C++ tools
#
# BCROOT defines the root directory of your bc builder install
#
# for some reason TLink32 always crashes and burns trying to link
# this - use ILink32 instead.



!ifndef BCROOT
BCROOT=$(MAKEDIR)\..
!endif

BCC32   = $(BCROOT)\bin\Bcc32.exe 
TLINK32 = $(BCROOT)\bin\ILink32.exe

IDE_LinkFLAGS32 =  -L$(BCROOT)\LIB
LINKOPTS= -Tpe -ap -c -x
COMPOPTS= -O2 -tWC -tWM- -D_NO_VCL; -I$(BCROOT)\include;..\..\include;


timer.exe : timer.obj
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












