
# very basic makefile for jgrep.exe
#
# Borland C++ tools
#
# BCROOT defines the root directory of your bc5 install
#
# Optionally define SGIROOT to point to either the SGI
# or HP STL implimentation
#
!ifndef BCROOT
BCROOT=$(MAKEDIR)\..
!endif

BCC32   = $(BCROOT)\bin\Bcc32.exe 
TLINK32 = $(BCROOT)\bin\TLink32.exe

IDE_LinkFLAGS32 =  -L$(BCROOT)\LIB
LINKOPTS= -ap -Tpe -x
!ifdef SGIROOT
COMPOPTS= -O2 -DSTRICT; -I$(SGIROOT);$(BCROOT)\include;..\..\include;
!else
COMPOPTS= -O2 -DSTRICT; -I$(BCROOT)\include;..\..\include;
!endif


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














