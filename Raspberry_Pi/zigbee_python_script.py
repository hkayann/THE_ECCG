'''All informative readings/notifications are commented out. Uncomment when needed.'''
import logging
import time

from digi.xbee.devices import XBeeDevice
from digi.xbee.exception import OperationNotSupportedException, XBeeException, RecoveryException
from digi.xbee.util import utils
from digi.xbee.models.status import NetworkDiscoveryStatus

PORT = "/dev/serial0"
BAUD_RATE = 9600

def open_device():

    #print("Opening the XBee device by forcing its baudrate to %d" % BAUD_RATE)
    device = XBeeDevice(PORT, BAUD_RATE)

    try:
        device.open(force_settings=True)
        #print("Device opened and set to operate at %d bauds" % BAUD_RATE)
        return device
    except XBeeException as e:
        #print("ERROR: %s" % str(e))
        return

def set_parameters(xbee_device):

    PARAM_NODE_ID = "NI"
    PARAM_PAN_ID = "ID"
    PARAM_DEST_ADDRESS_H = "DH"
    PARAM_DEST_ADDRESS_L = "DL"
    MAKE_COORDINATOR = "CE"
    SECURITY = "EE"
    JOIN = "NJ"

    protocol = xbee_device.get_protocol()
    role = xbee_device.get_role()
    PARAM_VALUE_NODE_ID = "Jedi"
    PARAM_VALUE_PAN_ID = utils.hex_string_to_bytes("ABCD")
    PARAM_VALUE_DEST_ADDRESS_H = utils.hex_string_to_bytes("00")
    PARAM_VALUE_DEST_ADDRESS_L = utils.hex_string_to_bytes("FFFF")
    CO = utils.hex_string_to_bytes("1")
    SE = utils.hex_string_to_bytes("0")
    JO = utils.hex_string_to_bytes("FF")

    xbee_device.set_parameter(PARAM_NODE_ID, bytearray(PARAM_VALUE_NODE_ID, 'utf8'))
    xbee_device.set_parameter(PARAM_PAN_ID, PARAM_VALUE_PAN_ID)
    xbee_device.set_parameter(PARAM_DEST_ADDRESS_H, PARAM_VALUE_DEST_ADDRESS_H)
    xbee_device.set_parameter(PARAM_DEST_ADDRESS_L, PARAM_VALUE_DEST_ADDRESS_L)
    xbee_device.set_parameter(MAKE_COORDINATOR,CO)
    xbee_device.set_parameter(SECURITY,SE)
    xbee_device.set_parameter(JOIN,JO)

    apply_changes_enabled = xbee_device.is_apply_changes_enabled()
    xbee_device.write_changes()

    # Get parameters. Run this during setup if you want.
    #print("Node ID:                     %s" % xbee_device.get_parameter(PARAM_NODE_ID).decode())
    #print("PAN ID:                      %s" % utils.hex_to_string(xbee_device.get_parameter(PARAM_PAN_ID)))
    #print("Destination address high:    %s" % utils.hex_to_string(xbee_device.get_parameter(PARAM_DEST_ADDRESS_H)))
    #print("Destination address low:     %s" % utils.hex_to_string(xbee_device.get_parameter(PARAM_DEST_ADDRESS_L)))
    #print("Coordinator MAC Address      %s" % utils.hex_to_string(xbee_device.get_64bit_addr()))
    #print("16-bit network address:      %s" % xbee_device.get_16bit_addr()) #it is same?
    #print("Power level:                 %s" % xbee_device.get_power_level().description) #highest - +8db then.
    #print("Protocol:                    %s" % protocol)
    #print("Role:                        %s" % role)
    #print("Association Status:          %s" % utils.hex_to_string(xbee_device.get_parameter("AI"))) #if the network successfully formed
    #print("Zigbee Defined Address:      %s" % utils.hex_to_string(xbee_device.get_parameter("MY")))
    #print("Scanned Channels:            %s" % utils.hex_to_string(xbee_device.get_parameter("SC")))
    #print("Stack profile:               %s" % utils.hex_to_string(xbee_device.get_parameter("ZS")))
    #print("OP:                          %s" % utils.hex_to_string(xbee_device.get_parameter("OP")))
    #print("OI:                          %s" % utils.hex_to_string(xbee_device.get_parameter("OI")))
    #print("Operating Mode:              %s" % operating_mode)

    #print("")
    #print("All parameters were set correctly!")

def discover_network(device):

    xbee_network = device.get_network()
    xbee_network.set_discovery_timeout(15)  # 15 seconds.
    xbee_network.clear()
    # Callback for discovered devices.
    def callback_device_discovered(remote):
        print("Device discovered: %s" % remote)
    # Callback for discovery finished.
    def callback_discovery_finished(status):
        if status == NetworkDiscoveryStatus.SUCCESS:
            print("Discovery process finished successfully.")
        else:
            print("There was an error discovering devices: %s" % status.description)

        xbee_network.add_device_discovered_callback(callback_device_discovered)
        xbee_network.add_discovery_process_finished_callback(callback_discovery_finished)
        xbee_network.start_discovery_process()
        #print("Discovering remote XBee devices...")
        while xbee_network.is_discovery_running():
            time.sleep(0.1)

def read_data(xbee_device):

    xbee_device.flush_queues()
    #print("Waiting for data...\n")
    while True:
        xbee_message = xbee_device.read_data()
        if xbee_message is not None:
            print("From %s >> %s" % (xbee_message.remote_device.get_64bit_addr(),
                                     xbee_message.data.decode()))

if __name__ == '__main__':

    zigbee = open_device()
    set_parameters(zigbee)
    zigbee.execute_command("AC")
    time.sleep(1)
    zigbee.execute_command("WR")
    #print("Network Established")
    discover_network(zigbee)
    read_data(zigbee)