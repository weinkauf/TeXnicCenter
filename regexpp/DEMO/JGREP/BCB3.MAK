
# very basic makefile for jgrep.exe
#
# Borland C++ tools
#
# BCROOT defines the root directory of your bc builder install
#
# for some reason C++ builder will not link this with Tlink32
# works fine with ILink32 though.
#
!ifndef BCROOT
BCROOT=$(MAKEDIR)\..
!endif

BCC32   = $(BCROOT)\bin\Bcc32.exe 
TLINK32 = $(BCROOT)\bin\ILink32.exe

IDE_LinkFLAGS32 =  -L$(BCROOT)\LIB
LINKOPTS= -ap -Tpe -x
COMPOPTS= -O2 -tWM- -DSTRICT; -I$(BCROOT)\include;..\..\include;



jgrep.exe : jgrep.obj main.obj
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LINKOPTS) +
$(BCROOT)\lib\c0x32.obj+
main.obj+
jgrep.obj+
$(BCROOT)\lib\import32.lib+
$(BCROOT)\lib\cw32.lib
$<,$*

|
jgrep.obj :  jgrep.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ jgrep.cpp
|

main.obj :  main.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ main.cpp
|












