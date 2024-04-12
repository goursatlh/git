CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = `pkg-config --cflags --libs dbus-1`

all: server client

server: dbus_ser_gpt.c
	$(CC) $(CFLAGS) -o server dbus_ser_gpt.c $(LIBS)

client: dbus_cli_gpt.c
	$(CC) $(CFLAGS) -o client dbus_cli_gpt.c $(LIBS)

clean:
	rm -f server client

