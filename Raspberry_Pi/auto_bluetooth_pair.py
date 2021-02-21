"""Not all libraries/class variables/methods are used.
However, if you encounter any problems or want to develop your code
further you might need those. Thus, they will be left as they are."""

import dbus
import dbus.service
import dbus.mainloop.glib
import sys
import time
import pexpect
import subprocess
import uuid
import socket

from datetime import datetime
from bluezutils import *
from pprint import *
from bluetooth import *
from gi.repository import GObject, GLib
from dbus.mainloop.glib import DBusGMainLoop

DBusGMainLoop(set_as_default=True)

path = "/test/agent"
AGENT_INTERFACE = "org.bluez.Agent1"
BUS_NAME = "org.bluez"
bus = dbus.SystemBus()

device_obj = None
dev_path = None

def set_trusted(path2):
    props = dbus.Interface(bus.get_object("org.bluez", path2), "org.freedesktop.DBus.Properties")
    props.Set("org.bluez.Device1", "Trusted", True)

class Rejected(dbus.DBusException):
    _dbus_error_name = "org.bluez.Error.Rejected"

class Agent(dbus.service.Object):
    exit_on_release = True

    def set_exit_on_release(self, exit_on_release):
        self.exit_on_release = exit_on_release

    `+ at + `dbus.service.method(AGENT_INTERFACE,
        in_signature="", out_signature="")
    def Release(self):
        print("Release")
        if self.exit_on_release:
            mainloop.quit()

    `+ at + `dbus.service.method(AGENT_INTERFACE,
        in_signature="os", out_signature="")
    def AuthorizeService(self, device, uuid):
        print("AuthorizeService (%s, %s)" % (device, uuid))
        return

    `+ at + `dbus.service.method(AGENT_INTERFACE,
        in_signature="o", out_signature="s")
    def RequestPinCode(self, device):
        set_trusted(device)
        return "0000"

    `+ at + `dbus.service.method(AGENT_INTERFACE,
        in_signature="o", out_signature="u")
    def RequestPasskey(self, device):
        set_trusted(device)
        return dbus.UInt32("0000")

    `+ at + `dbus.service.method(AGENT_INTERFACE,
        in_signature="ou", out_signature="")
    def RequestConfirmation(self, device, passkey):
        set_trusted(device)
        return

    `+ at + `dbus.service.method(AGENT_INTERFACE,
        in_signature="o", out_signature="")
    def RequestAuthorization(self, device):
        return

    `+ at + `dbus.service.method(AGENT_INTERFACE,
        in_signature="", out_signature="")
    def Cancel(self):
        print("Cancel")

def pair_reply():
    print("Device paired and trusted")
    set_trusted(dev_path)

def pair_error(error):
    err_name = error.get_dbus_name()
    if err_name == "org.freedesktop.DBus.Error.NoReply" and device_obj:
        print("Timed out. Cancelling pairing")
        device_obj.CancelPairing()
    else:
        print("Creating device failed: %s" % (error))
        mainloop.quit()

def register_agent():
    agent = Agent(bus, path)
    capability = "NoInputNoOutput"
    obj = bus.get_object(BUS_NAME, "/org/bluez");
    manager = dbus.Interface(obj, "org.bluez.AgentManager1")
    manager.RegisterAgent(path, capability)

def start_discovery():
    global pi_adapter
    pi_adapter = find_adapter()
    scan_filter = dict({"DuplicateData": False})
    pi_adapter.SetDiscoveryFilter(scan_filter)
    pi_adapter.StartDiscovery()

def stop_discovery():
    pi_adapter.StopDiscovery()

def get_device(dev_str):
    # use [Service] and [Object path]:
    device_proxy_object = bus.get_object("org.bluez","/org/bluez/hci0/dev_"+dev_str)
    # use [Interface]:
    device1 = dbus.Interface(device_proxy_object,"org.bluez.Device1")
    return device1

def char_changer(text):
    text = text.replace(':', r'_')
    return text

def slave_finder(device_name):

    global sublist_normal
    sublist_normal = []
    sublist= []
    address = []
    edited_address = None
    sub = subprocess.run(["hcitool scan"], text = True, shell = True, capture_output=True)
    print(sub.stdout) #string type
    sublist = sub.stdout.split()
    for i in range(len(sublist)):
        if sublist[i] == device_name:
            print(sublist[i-1])
            sublist_normal.append(sublist[i-1])
            edited_address = char_changer(sublist[i-1])
            address.append(edited_address)
    return address

def connect_paired(baddr):
    global s
    channel = 1
    s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
    s.connect((baddr,channel))
    print ("Connected to: "+baddr)

def remove_all_paired():
    for i in range(len(sublist_normal)):
        sub = subprocess.run(["bluetoothctl remove " + sublist_normal[i]],
            text = True, shell = True, capture_output=True)
        time.sleep(1)

if __name__ == '__main__':

    pair_status = None
    address_list = slave_finder('Slave') #!R
    time.sleep(2)
    remove_all_paired()
    print(sublist_normal)
    if address_list:
        register_agent()
        start_discovery()
        time.sleep(10)
        for i in range(len(address_list)):
            new_dbus_device = get_device(address_list[i])
            dev_path = new_dbus_device.object_path
            device_properties = dbus.Interface(new_dbus_device, "org.freedesktop.DBus.Properties")
            pair_status = device_properties.Get("org.bluez.Device1", "Paired")
            if not pair_status:
                new_dbus_device.Pair(reply_handler=pair_reply, error_handler=pair_error, timeout=60000)

    mainloop = GLib.MainLoop()
    mainloop.run()