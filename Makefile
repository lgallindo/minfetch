CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -Wpedantic -g -O3 -Wno-unused-result -Wno-discarded-qualifiers
IFLAGS=-Wall -Wextra -std=c99 -Wpedantic -O3 -Wno-unused-result -Wno-discarded-qualifiers
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
