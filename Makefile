CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g -O3
IFLAGS=-Wall -Wextra -std=c99 -O3
INSTALLDIR=~/.local/bin
BINARY=minfetch
SRC=./minfetch.c

all: $(BINARY)

install: $(SRC)
	$(CC) $(IFLAGS) $(SRC) -o $(BINARY)
	strip $(BINARY)
	mkdir -p $(INSTALLDIR)
	cp $(BINARY) $(INSTALLDIR)

$(BINARY): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BINARY)

clean: $(BINARY)
	rm $(BINARY)

uninstall: clean
	rm $(INSTALLDIR)/$(BINARY)
