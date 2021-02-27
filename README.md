# THE_ECCG
This is the repository of The Edge to Cloud Code Generator (ECCG).

## The ECCG is a web application that generates ready to use code/guide for designing IoT applications mainly designed for data scientists. 

### The ECCG is tested with the following hardware:

- [Raspberry Pi 4 Model B][link1]
- [Arduino Expansion Shield for Raspberry Pi model B][link2]
- [Grove - Temperature&Humidity Sensor (DHT11)][link3]
- [Grove - Air Quality Sensor v1.3][link4]
- [Grove - UART WiFi V2][link5]
- [Grove - Bee Socket][link6]
- [Digi XBee 3 Zigbee 3 RF Module][link7]
- [Grove - Serial Bluetooth v3.0][link8]

### Notes regarding Arduino codes:
- The Arduino codes are actually given as JS variables that are used to build the web application.
- They need to be slightly edited to be a MWE. 
- The codes include debugging part that prints data flow to Serial Monitor.
- The some of AT (Hayes) commands are loaded into flash memory, hence requiring to be run only once. However running them each time do not make any significant differences.
- The ECCG assume you are running modules for the first time.

### Notes regarding bluetooth auto pairing Python3 script:
- [BlueZ][link9] is the official Linux Bluetooth protocol stack.
- BlueZ is based on D-Bus.
- The auto pairing Python3 script is written based on D-Bus.
- The script does the following: (I) looks for a bluetooth module named 'Slave', (II) removes if paired, (III) then repairs.
- The process is asynchronous.
- The D-Bus might be difficult to be understood by beginners, but you should be able to realize what kind of changes you need to make by looking at the script, without knowing how D-Bus work.

### Notes regarding ZigBee Python3 script:
- Python script is preferred due the Digi-Key providing an official [API][link10]. 
- The script allows you to send data to designated coordinator.

### Notes regarding given examples:
- The examples folder contains ready-to-use Arduino codes.


[link1]: <https://www.raspberrypi.org/products/raspberry-pi-4-model-b/>
[link2]: <https://www.dfrobot.com/product-1148.html/>
[link3]: <https://wiki.seeedstudio.com/Grove-TemperatureAndHumidity_Sensor/>
[link4]: <https://wiki.seeedstudio.com/Grove-Air_Quality_Sensor_v1.3/>
[link5]: <https://wiki.seeedstudio.com/Grove-UART_Wifi_V2/>
[link6]: <https://wiki.seeedstudio.com/Grove-Bee_Socket/>
[link7]: <https://www.digi.com/products/embedded-systems/digi-xbee/rf-modules/2-4-ghz-rf-modules/xbee3-zigbee-3>
[link8]: <https://wiki.seeedstudio.com/Grove-Serial_Bluetooth_v3.0/>
[link9]: <http://www.bluez.org/release-of-bluez-5-54-and-5-53/>
[link10]: <https://xbplib.readthedocs.io/en/latest/#indexapi>
