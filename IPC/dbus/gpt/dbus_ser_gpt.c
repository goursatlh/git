#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>

// Define the method callback function
DBusHandlerResult method_handler(DBusConnection *connection, DBusMessage *message, void *user_data) {
    DBusMessage *reply;
    const char *method_name;

	printf("%s/%d in\n", __FUNCTION__, __LINE__);
    // Get the method name
    method_name = dbus_message_get_member(message);
	printf("%s/%d method_name %s\n", __FUNCTION__, __LINE__, method_name);

    // Create a reply message
    reply = dbus_message_new_method_return(message);

    // Append a string to the reply message
    dbus_message_append_args(reply, DBUS_TYPE_STRING, &method_name, DBUS_TYPE_INVALID);

    // Send the reply message
    dbus_connection_send(connection, reply, NULL);

    // Free the reply message
    dbus_message_unref(reply);

    return DBUS_HANDLER_RESULT_HANDLED;
}

int main() 
{
    DBusError err;
    DBusConnection *conn;
    int ret;

    // Initialize D-Bus error
    dbus_error_init(&err);

    // Connect to the D-Bus session bus
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    //conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Connection Error (%s)\n", err.message);
        dbus_error_free(&err);
        exit(1);
    }
    if (conn == NULL) {
        fprintf(stderr, "Connection Null\n");
        exit(1);
    }

    // Request a bus name
    ret = dbus_bus_request_name(conn, "org.example.test", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Name Error (%s)\n", err.message);
        dbus_error_free(&err);
        exit(1);
    }
    
    printf("start the app : org.example.test, ret %d\n", ret);
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf(stderr, "Not Primary Owner\n");
        exit(1);
    }

    // Register a method handler
    dbus_bus_add_match(conn, "type='method_call',interface='org.example.test'", &err);
    dbus_connection_add_filter(conn, method_handler, NULL, NULL);

    // Main loop
    while (1) {
        // Process D-Bus events
        dbus_connection_read_write_dispatch(conn, -1);
    }

    // Close the connection
    dbus_connection_close(conn);

    return 0;
}

