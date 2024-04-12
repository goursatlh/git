#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>

int main() 
{
    DBusError err;
    DBusConnection *conn;
    DBusMessage *msg, *reply;
    int ret;

    // Initialize D-Bus error
    dbus_error_init(&err);

    // Connect to the D-Bus session bus
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Connection Error (%s)\n", err.message);
        dbus_error_free(&err);
        exit(1);
    }
    if (conn == NULL) {
        fprintf(stderr, "Connection Null\n");
        exit(1);
    }

    // Create a message
    msg = dbus_message_new_method_call("org.example.test", // target for the message (service name)
                                       "/org/example/test", // object to call on
                                       "org.example.test", // interface to call on
                                       "TestMethod"); // method name
    if (msg == NULL) {
        fprintf(stderr, "Message Null\n");
        exit(1);
    }

    // Send the message and get a reply
    reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Reply Error (%s)\n", err.message);
        dbus_error_free(&err);
        exit(1);
    }
    if (reply == NULL) {
        fprintf(stderr, "Reply Null\n");
        exit(1);
    }

    // Print the reply
    printf("Reply: %s\n", dbus_message_get_signature(reply));

    // Free the message
    dbus_message_unref(msg);
    dbus_message_unref(reply);

    // Close the connection
    dbus_connection_close(conn);

    return 0;
}

