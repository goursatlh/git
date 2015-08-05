CFLAGS=-lpthread
CTARGETS = cli
all: cli

cli: chat_cli.o chat_io.o
	cc -o cli $(CFLAGS) chat_cli.o chat_io.o 

clean:
	rm -f *.o $(CTARGETS)
