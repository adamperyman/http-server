IDIR 									= ./include
CC 										= gcc
CFLAGS 								= -I$(IDIR) -W -Wall

LDIR 									= ./lib

LIBS									= -lm

_DEPS 								= file-handling.h macros.h output.h sockets.h
DEPS 									= $(patsubst %,$(IDIR)/%,$(_DEPS))

MAIN 									= ./src/http-server.o

_OBJ 									= file-handling.o output.o sockets.o
OBJ 									= $(patsubst %,$(LDIR)/%,$(_OBJ))

$(LDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

HTTP-Server: $(OBJ) $(MAIN)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f ./*.o $(LDIR)/*.o $(MAIN) *~ core $(IDIR)/*~ ./HTTP-Server
