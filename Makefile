# vim: set noet ts=8 sw=8 sts=8:
#

VPATH =
CC = gcc
LD = gcc

CFLAGS=-Wall -Wextra -pedantic -std=c99 -Wshadow -Wpointer-arith \
	-Wcast-qual -Wcast-align -Wstrict-prototypes -Wmissing-prototypes \
	-Wswitch-default -Wswitch-enum -Wuninitialized -Wconversion \
	-Wredundant-decls -Wnested-externs -Wunreachable-code \
	-g -O3 -Wno-unused-parameter


OBJS = main.o
PROG = main.exe


all: $(PROG)


%.o: %.c
	$(CC) -c $(CFLAGS) `pkg-config --cflags gtk+-3.0` -o $@ $^

$(PROG): $(OBJS)
	$(LD) -o $(PROG) $(OBJS) `pkg-config --libs gtk+-3.0` -lopengl32


.PHONY: clean
clean:
	rm -f *.o
	rm -f $(PROG)
