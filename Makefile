CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -Wpedantic -g -O3
IFLAGS=-Wall -Wextra -std=c99 -Wpedantic -O3
INSTALLDIR=~/.local/bin
BINARY=minfetch
SRC=./minfetch.c

all: $(BINARY)

install: $(SRC)
	$(CC) $(IFLAGS) $(SRC) -o $(BINARY)
	mkdir -p $(INSTALLDIR)
	cp $(BINARY) $(INSTALLDIR)

$(BINARY): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BINARY)

clean: $(BINARY)
	rm $(BINARY)

uninstall: clean
	rm $(INSTALLDIR)/$(BINARY)
