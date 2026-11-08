ESP32 Penetration Testing

Environment
    Hardware/Software: ESP32, ArduinoIDE, Wifi-Adapter, Kali Linux 
    Target: ESP32
    Attacker: Kali Linux (VM)
    Connection: ESP32 running in WiFi Access Point (AP) mode with Kali joining as WiFi client
    Scope: All performed in hardware I own.


Kali Setup
    1. screen /dev/ttyUSB0 115200
    2. screen -ls
    3. scren -X -S {session_id} quit


Connecting Kali to ESP32's AP (on managed mode)
    1. sudo iwlist wlan0 scan | grep -i "ESP32-Target"
    2. iwconfig 


NMCLI
    1. nmcli device wifi list
    2. nmcli device wifi connect "ESP32-Target" password "password123"

Check AP
    1. curl http://192.168.4.1/ 
        Hello from ESP32
    2. curl http://192.168.4.1/scan.txt
        SSID, RSSI, Security
            TestNetwork, -45, WPA2
    3. nmap -sV -p 80 192.168.4.1

DoS Attack
    for i in {1..50} do curl -s http://192.168.4.1/ & done

Debug
Stuck In Monitor Mode
    1. sudo airmon-ng stop wlan0
    2. sudo iwconfig wlan0 mode managed
    3. sudo systemctl restart NetworkManager
    
Scanning Wrong IP
    192.168.1 is your ESP32's IP. It should be listed when you restart the program (click the left button on ESP32) and check the screen. You accidentally scanned your Kali's


