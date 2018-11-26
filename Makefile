CC=gcc
INC=include
CFLAGS=-Wall -I$(INCLUDE) -v

OBJ=obj
BIN=bin
LIBS=-lm

setup:
	test -d $(OBJ) || mkdir $(OBJ)
	test -d $(BIN) || mkdir $(BIN)

othello.o: src/othello.cpp $(INC)/game.h
	$(CC) -c -o $(OBJ)/othello.o $< $(CFLAGS)
othello: othello.o
	$(CC) -o $(BIN)/othello $(OBJ)/othello.o $(CFLAGS) $(LIBS)
all: setup othello

.PHONY: clean
clean:
	rm -rf $(OBJDIR)/*.o
	rm -rf $(BINDIR)/*