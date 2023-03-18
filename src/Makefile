CC = gcc
CFLAGS = -std=c99 $(EXTRA_CFLAGS) -fdiagnostics-color -O2 -Wall -Wextra -Werror
LDFLAGS =
ARCHFLAGS =

EXE = konpu
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(ARCHFLAGS) -o $@ $(OBJ)

-include $(DEP)

.c.o:
	$(CC) $(CFLAGS) $(ARCHFLAGS) -MMD -c $<


clean:
	rm -f $(OBJ) $(DEP)

cleanall:
	rm -f *.o *.d *.gch $(EXE)
