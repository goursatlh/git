#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>

int main() 
{
    DBusConnection* dbus_conn = NULL;
    DBusError dbus_error;

    // Initialize D-Bus error
    dbus_error_init(&dbus_error);

    // Connect to D-Bus system bus
    dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error);
    if (dbus_error_is_set(&dbus_error)) 
    {
        fprintf(stderr, "D-Bus connection error: %s\n", dbus_error.message);
        dbus_error_free(&dbus_error);
        return 1;
    }

    printf("Connected to D-Bus as \"%s\".\n", dbus_bus_get_unique_name(dbus_conn));

    // Now you're on the bus and can access various services!
    // For example, you can query battery status, control system sleep, etc.

    // Clean up
    dbus_connection_unref(dbus_conn);
    return 0;
}
