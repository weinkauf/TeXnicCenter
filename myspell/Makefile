
CC=g++

CXXFLAGS=-O2 -I.
LDFLAGS=-L. -lmyspell

LIBS=libmyspell.a

AR=ar rc
RANLIB=ranlib

OBJS = affix.o affixmgr.o hashmgr.o suggestmgr.o myspell.o

all: example

libmyspell.a: $(OBJS)
	$(AR) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) >/dev/null 2>&1

example: example.o $(LIBS)
	$(CC) $(CXXFLAGS) -o $@ example.o $(LDFLAGS)

%.o: %.cxx 
	$(CC) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *~ example libmyspell.a libmyspell.so*

distclean:	clean

depend:
	makedepend -- $(CXXFLAGS) -- *.[ch]xx

# DO NOT DELETE THIS LINE -- make depend depends on it.

affi.x.o: affix.hxx baseaffix.hxx atypes.hxx
affixmgr.o: affix.hxx baseaffix.hxx atypes.hxx affixmgr.hxx
hashmgr.o: hashmgr.hxx htypes.hxx htypes.hxx affixmgr.hxx
suggest.o: suggestmgr.hxx affixmgr.hxx
myspell.o: myspell.hxx suggestmgr.hxx hashmgr.hxx affixmgr.hxx
example.o: myspell.hxx suggestmgr.hxx hashmgr.hxx affixmgr.hxx
