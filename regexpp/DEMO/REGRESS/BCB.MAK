
# very basic makefile for regress
#
# Borland C++ tools
#
# BCROOT defines the root directory of your bcb install
#
!ifndef BCROOT
BCROOT=$(MAKEDIR)\..
!endif

BCC32   = $(BCROOT)\bin\Bcc32.exe 
TLINK32 = $(BCROOT)\bin\ILink32.exe

IDE_LinkFLAGS32 =  -L$(BCROOT)\LIB
LINKOPTS= -ap -Tpe -x
COMPOPTS= -O2 -tWM- -DSTRICT; -w-inl -w-aus -w-csu -w-eff -w-rch -I$(BCROOT)\include;..\..\include;

all :: r1.exe r2.exe r3.exe r4.exe r5.exe r6.exe
	r1 tests.txt test1252.txt
	r2 tests.txt
	r3 tests.txt
	r4 tests.txt test1252.txt
	r5 tests.txt
	r6 tests.txt


r1.exe : r1.obj
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LINKOPTS) +
$(BCROOT)\lib\c0x32.obj+
r1.obj+
$(BCROOT)\lib\import32.lib+
$(BCROOT)\lib\cw32.lib
$<,$*
|

r2.exe : r2.obj
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LINKOPTS) +
$(BCROOT)\lib\c0x32.obj+
r2.obj+
$(BCROOT)\lib\import32.lib+
$(BCROOT)\lib\cw32.lib
$<,$*
|
r3.exe : r3.obj
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LINKOPTS) +
$(BCROOT)\lib\c0x32.obj+
r3.obj+
$(BCROOT)\lib\import32.lib+
$(BCROOT)\lib\cw32.lib
$<,$*
|
r4.exe : r4.obj
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LINKOPTS) +
$(BCROOT)\lib\c0x32.obj+
r4.obj+
$(BCROOT)\lib\import32.lib+
$(BCROOT)\lib\cw32.lib
$<,$*
|
r5.exe : r5.obj
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LINKOPTS) +
$(BCROOT)\lib\c0x32.obj+
r5.obj+
$(BCROOT)\lib\import32.lib+
$(BCROOT)\lib\cw32.lib
$<,$*
|
r6.exe : r6.obj
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LINKOPTS) +
$(BCROOT)\lib\c0x32.obj+
r6.obj+
$(BCROOT)\lib\import32.lib+
$(BCROOT)\lib\cw32.lib
$<,$*
|

.cpp.obj:
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ $*.cpp
|


