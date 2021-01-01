CC = gcc
CFLAGS = -std=c89 -pedantic -Wall -Werror -g `pkg-config libxml-2.0 --cflags`
LDFLAGS = `pkg-config libxml-2.0 --libs-only-L`
LDLIBS = `pkg-config libxml-2.0 --libs-only-l`
EXEC = ddgm.out
RM = rm -fv

.PHONY: all clean

all: $(EXEC)

$(EXEC): ddgm.c ddg.o player.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

ddg.o: ddg.c
	$(CC) $(CFLAGS) -c -o $@ $<

player.o: player.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) *.o