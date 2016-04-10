IDIR 									= ./include
CC 										= gcc
CFLAGS 								= -I$(IDIR) -W -Wall -Werror

LDIR 									= ./lib

LIBS									= -lm

_DEPS 								= functions.h macros.h
DEPS 									= $(patsubst %,$(IDIR)/%,$(_DEPS))

MAIN 									= ./src/http-server.o

_OBJ 									= functions.o
OBJ 									= $(patsubst %,$(LDIR)/%,$(_OBJ))

$(LDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

HTTP-Server: $(OBJ) $(MAIN)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f ./*.o $(ODIR)/*.o *~ core $(IDIR)/*~ ./HTTP-Server