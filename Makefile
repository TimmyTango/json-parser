CC=clang
CFLAGS=-std=c99 -Wall -Wextra -I include/
BINDIR=bin
SRCDIR=src

main: $(BINDIR)/main.o $(BINDIR)/json_tokenizer.o
	$(CC) -o main $(BINDIR)/main.o $(BINDIR)/json_tokenizer.o

$(BINDIR)/main.o: $(SRCDIR)/main.c
	$(CC) -c -o $(BINDIR)/main.o $(SRCDIR)/main.c $(CFLAGS)

$(BINDIR)/json_tokenizer.o: $(SRCDIR)/json_tokenizer.c
	$(CC) -c -o $(BINDIR)/json_tokenizer.o $(SRCDIR)/json_tokenizer.c $(CFLAGS)

clean:
	rm -f main bin/*.o