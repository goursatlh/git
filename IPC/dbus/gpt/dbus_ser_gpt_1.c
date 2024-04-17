#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>

#define BUS_NAME "com.example.HelloWorld"
#define OBJECT_PATH "/com/example/HelloWorld"
#define INTERFACE_NAME "com.example.HelloWorld"

void SayHello(DBusConnection *conn) 
{
    printf("dbus: hello, world\n");
	return;
}

DBusObjectPathVTable vtable = {
    .message_function = SayHello,
};

int main() 
{
    DBusError err;
    DBusConnection *conn;
    int ret;

    // Initialize D-Bus error
    dbus_error_init(&err);

    // Connect to the D-Bus session bus
    //conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
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
    ret = dbus_bus_request_name(conn, BUS_NAME, 0, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Name Error (%s)\n", err.message);
        dbus_error_free(&err);
        exit(1);
    }
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf(stderr, "Not Primary Owner\n");
        exit(1);
    }
	//dbus_connection_register_object_path()/dbus_connection_register_fallback()
    if (!dbus_connection_register_object_path(conn, OBJECT_PATH, &vtable, NULL))
    {
        fprintf(stderr, "dbus: Could not set up message handler\n");
        exit(1);
    }

    // Main loop
    printf("dbus: begin to start the loop to process the dbus message.\n");
    while (1) {
        // Process D-Bus events
        dbus_connection_read_write_dispatch(conn, -1);
    }

    // Close the connection
    dbus_connection_close(conn);

#if 0
	dbus_connection_register_object_path(conn, OBJECT_PATH, &vtable, NULL);
	dbus_connection_setup_with_g_main(conn, NULL);
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
#endif
    return 0;
}

