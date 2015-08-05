CFLAGS=-lpthread
CTARGETS = sev cli
all: cli sev

cli: chat_cli.o chat_io.o
	cc -o cli $(CFLAGS) chat_cli.o chat_io.o 


sev: chat_sev.o chat_io.o
	cc -o sev $(CFLAGS) chat_sev.o chat_io.o 

clean:
	rm -f *.o $(CTARGETS)
