# offgridkom
Off grif communication business card

<img src="https://github.com/ccadic/offgricom/blob/main/20230813_010139.jpg">

The project is to develop an electronic board (communicator) capable of sending and receiving messages via WiFi, by Lora or Lorawan radio protocol, and by the ESP-NOW protocol. The mode of communication will vary in several ways. Option 1: Private communication between 2 boards. Option 2: Private communication between 2 boards, but using relays if they are too far apart (Lorawan network with gateways or another communicator serving as a repeater). Option 3: Broadcast communication where all users can hear and participate. A small centralized server will also need to be developed to manage traffic and handle routing. User addresses will be based on ERC-20 addresses or preferably TRC-20 to prepare for future interactions with the blockchain or to allow simple transactions (USDT on TRC-20 for instance)

<img src="https://github.com/ccadic/offgricom/blob/main/demo4.jpg">

**Specifications
**

Specifications of the "OffGridKom BC" board
The OffGridKom board has the following technical specifications:
Micro-controller ESP32S3 Wroom 1
ST7789 320x172 Color Display
RAKWireless RAK3172 Radio Micro-controller – 868 MHZ*
RainSun 868 MHZ ceramic antenna
T9Word alphanumeric keypad for composing messages
USB-C connection (compatible with external keyboard)
Two micro-switches for ESP32 reset and boot mode
An additional micro-switch for custom function
Board format: Exactly the size of a credit card
Weight: Approximately 20 grams



The hardware project is proprietary at the moment until we find a way to get a return on all the money invested. 
The software is GPL3

