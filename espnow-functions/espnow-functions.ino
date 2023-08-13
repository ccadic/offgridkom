#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include "Setup1_st7789_130x320.h"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// couleur de fond
uint16_t color = tft.color565(255, 220, 58);
uint16_t colorfond = tft.color565(19, 19, 19);
uint16_t colorgrey = tft.color565(194, 194, 194);

typedef struct {
    char msg[32];
} Message;

Message myData;
esp_now_peer_info_t peerInfo;

void OnDataRecv(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    Message *incoming = (Message*) data;
    Serial.print("Received: ");
    Serial.println(incoming->msg);
    tft.setTextColor(colorgrey);
    tft.setTextDatum(TL_DATUM);
    tft.setTextSize(1);
    tft.drawString(String("Received: ") + incoming->msg, 30, 120);
}

void setup() {
    // Init Serial Monitor
    Serial.begin(115200);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    pinMode(TFT_PWR_PIN, OUTPUT);
    pinMode(TFT_BL_PIN, OUTPUT);

    digitalWrite(TFT_PWR_PIN, HIGH); // Turn on the display power
    digitalWrite(TFT_BL_PIN, HIGH);  // Turn on the backlight

    tft.begin();
    tft.setRotation(3); // Set the screen rotation

    tft.setSwapBytes(true); // Swap the byte order for pushImage()
    tft.fillScreen(TFT_BLACK);

    // Get MAC Address of the device and print it
    String mac = WiFi.macAddress();
    tft.setTextColor(colorgrey);
    tft.setTextDatum(TL_DATUM);
    tft.setTextSize(1);
    tft.drawString("MAC: " + mac, 30, 25);
    
    Serial.println("MAC: " + mac);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        tft.setTextColor(colorgrey);
        tft.setTextDatum(TL_DATUM);
        tft.setTextSize(1);
        tft.drawString("Error initializing ESP-NOW", 30, 50);
        return;
    }

    // Register for receive callback
    esp_now_register_recv_cb(OnDataRecv);

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        tft.setTextColor(colorgrey);
        tft.setTextDatum(TL_DATUM);
        tft.setTextSize(1);
        tft.drawString("Failed to add peer", 30, 75);
        return;
    }

    Serial.println("Setup complete");
    tft.drawString("Requester Setup complete", 30, 100);
}

void loop() {
    strcpy(myData.msg, "Who is Device X?");
    esp_now_send(broadcastAddress, (uint8_t*)&myData, sizeof(myData));
    delay(2000);
}
