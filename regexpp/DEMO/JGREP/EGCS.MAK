
# very basic makefile for jgrep.exe
#
# egcs compiler GCC
#

jgrep.exe : jgrep.cpp main.cpp
	gcc -ojgrep.exe -O -DSTRICT -I../../include -I./ jgrep.cpp main.cpp -L../../lib -lregex++ -lstdc++ -luser32














