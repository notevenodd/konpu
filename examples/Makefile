CC = gcc
CDEFS = # -DKONPU_PLATFORM_SDL2 -DKONPU_PLATFORM_POSIX $(EXTRA_CDEFS)
CWARN = -Wall -Wextra -pedantic -fanalyzer $(EXTRA_CWARN)
CFLAGS = -std=c99 -I../src $(shell sdl2-config --cflags) -O2 -fdiagnostics-color $(CDEFS) $(CWARN) $(EXTRA_CFLAGS)
LDFLAGS = $(shell sdl2-config --libs) $(EXTRA_LDFLAGS)
ARCHFLAGS = $(EXTRA_ARCHFLAGS)

SRC = $(wildcard *.c)
EXE = $(SRC:.c=)

.PHONY: all clean cleanall

all: $(EXE)

$(EXE): %: %.c
	$(CC) $(CFLAGS) $(ARCHFLAGS) -o $@ $< $(LDFLAGS)

clean:
	@echo "use 'make cleanall' to remove the binaries"

cleanall:
	rm -f $(EXE)
