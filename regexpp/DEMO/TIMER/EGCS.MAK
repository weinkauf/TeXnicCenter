
# very basic makefile for jgrep.exe
#
# egcs compiler GCC
#

timer.exe : timer.cpp 
	gcc -otimer.exe -O -DSTRICT -I../../include -I./ timer.cpp -L../../lib -lregex++ -lstdc++ -luser32













