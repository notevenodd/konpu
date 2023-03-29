CC = gcc
CWARN = -Wall -Wextra -pedantic -fanalyzer -Werror $(EXTRA_CWARN)
CFLAGS = -std=c99 -fdiagnostics-color -O2 $(CWARN) $(EXTRA_CFLAGS)
ARCHFLAGS = $(EXTRA_ARCHFLAGS)

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

.PHONY: all clean cleanall

all: $(OBJ)

-include $(DEP)

.c.o:
	$(CC) $(CFLAGS) $(ARCHFLAGS) -MMD -c $<


clean:
	rm -f $(OBJ) $(DEP)

cleanall:
	rm -f *.o *.d *.gch $(EXE)
