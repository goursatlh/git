#!/usr/bin/env python3

# Import the 'dbus' module from dbus-python package.
# The package can be installed with `pip install dbus-python`.
# Documenation: https://dbus.freedesktop.org/doc/dbus-python/
import dbus

# We need the NetworkManager's D-Bus API documentation to
# know what objects and methods we are interested in:
# https://networkmanager.dev/docs/api/latest/spec.html

# We'll connect to system bus
bus = dbus.SystemBus()

# We'll send our messages to NetworkManager
NM_WELL_KNOWN_NAME = "org.freedesktop.NetworkManager"

# Call to the following method:
#  - object: /org/freedesktop/NetworkManager
#  - interface: org.freedesktop.NetworkManager
#  - method: GetDeviceByIpIface
#  - input argument: "eth0" (type: STRING)
#  - return value: the device's path (type: OBJECT_PATH)
#
# Get the path to the object that represents the device with
# the interface name "eth0".
nm_dbus_obj = bus.get_object(
    NM_WELL_KNOWN_NAME, "/org/freedesktop/NetworkManager"
)
nm_dbus_iface = dbus.Interface(
    nm_dbus_obj, "org.freedesktop.NetworkManager"
)
try:
    device_dbus_path = nm_dbus_iface.GetDeviceByIpIface("enp0s3")
except dbus.exceptions.DBusException as e:
    print("D-Bus error: " + str(e))
    quit()

print("D-Bus path to eth0 device: " + str(device_dbus_path))

# Call to the following method:
#  - object: the device that we obtained in the previous step
#  - interface: org.freedesktop.NetworkManager.Device
#  - method: Disconnect
#
# Request to the NM daemon to disconnect the device
# Note: NM will return an error if it was already disconnected
device_dbus_obj = bus.get_object(
    NM_WELL_KNOWN_NAME, device_dbus_path
)
device_dbus_iface = dbus.Interface(
    device_dbus_obj, "org.freedesktop.NetworkManager.Device"
)
try:
    device_dbus_iface.Disconnect()
except dbus.exceptions.DBusException as e:
    print("D-Bus error: " + str(e))
    quit()

print("Device disconnected")
