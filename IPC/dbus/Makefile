.PHONY:clean

CFLAG			 = `pkg-config --libs --cflags dbus-1`

SERVER_TARGET	 = server
CLIENT_TARGET	 = client

all:$(SERVER_TARGET) $(CLIENT_TARGET)

server:
	gcc dbus_ser.c -o $(SERVER_TARGET) $(CFLAG)

client:
	gcc dbus_cli.c -o $(CLIENT_TARGET) $(CFLAG)

clean:
	rm $(SERVER_TARGET) $(CLIENT_TARGET)

