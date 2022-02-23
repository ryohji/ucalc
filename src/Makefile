C_CHECK=-Wall -Wextra
CFLAGS+=$(C_CHECK)
CXXFLAGS+=$(C_CHECK) -std=c++14

ucalc: parser.o ucalc.o
	$(CC) -o $@ $(LDFLAGS) -lc++ $^

parser.o: parser.cc ucalc.h
ucalc.o: ucalc.c ucalc.h

.PHONY: all clean

all: ucalc

clean:
	$(RM) ucalc *.o *~
