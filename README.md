# Wireless-Hardware-Monitor
**Wireless Hardware Monitor with ESP8266 NodeMCU**

[![IMAGE ALT TEXT](http://img.youtube.com/vi/j76s0uAeMlU/0.jpg)](http://www.youtube.com/watch?v=j76s0uAeMlU "Video Title")

Wireless Hardware Monitor

* This project has no safety precaution whatsoever. It is only intended to be used in safe environment such as home wifi.
* All required SSID, password for WiFi and IP address should be local.

**Functions:**
Real-time Display of CPU, GPU temporature, CPU, GPU, RAM, VRAM usage, date and time

**Hardware List:**

ESP8266 board (ESP8266 NodeMCU Lua - [Link](https://www.amazon.com/HiLetgo-Internet-Development-Wireless-Micropython/dp/B010O1G1ES/ref=sr_1_2?crid=383KYRNYJWSZJ&dib=eyJ2IjoiMSJ9.gJShu3rQeKD8EK_mYUdf6S1HtsQDVwyVFPsSdBjZMvXhxQPq_tUNRGe6eyvYXcgsWt_epcPOUVjU-eyWYcFP0DMiK1nKaJ7n47ZFtqveEdzHsMMkutRtIMgKBbmQ29YNFh8p8A6R4uk3gUO9oYz8Zwnvc8QyEGUukKMn_mr4jeFpfq4Fi-k5GDxnFvKquh-Qt4w3lzGqe8Fng962uwdmQT6AqnbRzCEyjcKbw5CYLEc.7oMdj6360w_RsCnwOQbUM9UagDiJ9WVOS36m56-rfX8&dib_tag=se&keywords=ESP8266+NodeMCU&qid=1708609851&sprefix=esp8266+nodemcu%2Caps%2C508&sr=8-2))

LCD Display (TFT LCD 1.3inch 240x240 IPS ST7789 IC Driver - [Link](https://www.amazon.com/MakerFocus-Display-1-3inch-Interface-Routines/dp/B07P9X3L7M))

**Pin Connections**

ST7789 TFT Display ---  ESP8266 NodeMCU
               GND --- GND
               VCC --- 3V3
               SCL --- D5
               SDA --- D7
               RES --- D2
               DC  --- D1
               BLK --- 3V3

**Software List:**

Openhardwaremonitor (only v0.9.3 works for me - [Download Link](https://openhardwaremonitor.org) )

Arduino IDE (Any version should be fine)

**Additional Library for Arduino IDE:**
NTPClient v3.2.0 by Fabrice Weinberg
Adafruit BusIO v1.8.3 by Adafruit 
Adafruit Circuit Playground v1.8.6 by Adafruit 
Adafruit GFX Library v1.10.10 by Adafruit 
Adafruit seesaw Library v1.4.5 by Adafruit 
Adafruit ST7735 and ST7789 Library v1.7.3 by Adafruit 
Adafruit STMPE610 v1.1.3 by Adafruit 
ArduinoJson v6.18.3 by Benoit Blanchon
DisplayMenu v0.9.2 by guttih
ESP8266 Weather Station v2.0.1 by ThingPulse
Json Streaming Parser v1.0.5 by Daniel Eichhorn
Mini Grafx v1.2.5 by Daniel Eichhorn
TFT_eSPI v2.3.70 by Bodmer
Time v1.6.1 by Paul Stoffregen
Jpg_Decoder v0.0.3 by Bodmer

**Settings for uploading code to board via arduino IDE:**
Board: NodeMCU 1.0 (ESP-12E Module)
Upload Speed: 115200
CPU Frequency: 80MHz
Flash Size: 4MB (FS:2MB OTA:~1019KB)

**How to start monitoring**
1. Run HWmonitor.exe from the openhardwaremonitor directory you downloaded
2. Run server_ESP.py, I'm using a anaconda command prompt client on windows
3. The output of server_ESP.py should contain the local IP address it's running on
4. Fill your WiFi SSID, password, and IP address from step 3 in the 8266client.ino file
5. Upload 8266client.ino to your board via arduino IDE

* Currently the only way to stop the server is to kill the process via task manager, ctrl+c probably won't work.

Thanks to the socket [tutorial](https://www.youtube.com/watch?v=3QiPPX-KeSc) by TechWithTim on Youtube.

**My Youtube video of this project (Cringe Warning)**


[![IMAGE ALT TEXT](http://img.youtube.com/vi/j76s0uAeMlU/0.jpg)](http://www.youtube.com/watch?v=j76s0uAeMlU "Video Title")
