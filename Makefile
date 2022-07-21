CC=cc
CFLAGS=-Wall -Wextra -std=c99 -Wpedantic -g -O2
IFLAGS=-Wall -Wextra -std=c99 -Wpedantic -O2
BINARY=minfetch
SRC=$(wildcard ./src/*.c)
OBJ=$(SRC:.c=.o)

all: $(BINARY)
	./$(BINARY)

install: $(OBJ)
	$(CC) $(IFLAGS) $(SRC) -o $(BINARY)

$(BINARY): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BINARY)

clean: $(BINARY) $(OBJ)
	rm $(BINARY)
	rm $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
