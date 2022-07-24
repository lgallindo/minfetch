CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -Wpedantic -g
IFLAGS=-Wall -Wextra -std=c99 -Wpedantic
INSTALLDIR=~/.local/bin
BINARY=minfetch
SRC=$(wildcard ./src/*.c)
OBJ=$(SRC:.c=.o)

all: $(BINARY)

install: $(OBJ)
	@$(CC) $(IFLAGS) $(SRC) -o $(BINARY)
	@mkdir -p $(INSTALLDIR)
	@cp $(BINARY) $(INSTALLDIR)

$(BINARY): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(BINARY)

clean: $(BINARY) $(OBJ)
	@rm $(BINARY)
	@rm $(OBJ)

uninstall: clean
	@rm $(INSTALLDIR)/$(BINARY)

%.o: %.c
	@$(CC) $(CFLAGS) -c $^ -o $@
