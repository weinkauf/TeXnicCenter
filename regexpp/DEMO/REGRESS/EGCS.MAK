
# very basic makefile for regress.exe
#
# egcs compiler GCC on Win32
#

total : r1.exe r2.exe r4.exe r5.exe
	r1 tests.txt test1252.txt
	r2 tests.txt
	r4 tests.txt test1252.txt
	r5 tests.txt

r1.exe : parse.cpp regress.cpp tests.cpp r1.cpp
	gcc -O -or1.exe -DSTRICT -I../../include -I./ r1.cpp -lstdc++ -luser32

r2.exe : r2.cpp
	gcc -O -or2.exe -DSTRICT -I../../include -I./ r2.cpp -lstdc++ -luser32

r4.exe : parse.cpp regress.cpp tests.cpp r4.cpp
	gcc -O -or4.exe -DSTRICT -I../../include -I./ r4.cpp -lstdc++ -luser32

r5.exe : parse.cpp regress.cpp tests.cpp r5.cpp
	gcc -O -or5.exe -DSTRICT -I../../include -I./ r5.cpp -lstdc++ -luser32















