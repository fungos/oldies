CPP  = g++
CC   = gcc
RES  = 
OBJ  = main.o $(RES)
LINKOBJ  = main.o $(RES)
BIN  = animal
RM = rm -f

all: animal

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o "animal" $(LIBS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)
