#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include "Setup1_st7789_130x320.h"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint16_t color = tft.color565(255, 220, 58); 
uint16_t colorfond = tft.color565(19, 19, 19);
uint16_t colorgrey = tft.color565(194, 194, 194);

typedef struct {
    char msg[32];
} Message;

Message responseMsg;

String macToString(const uint8_t* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

void OnDataRecv(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    Message *incoming = (Message*) data;

    String senderMac = macToString(macAddr);
    Serial.println("Received from: " + senderMac);
    tft.setTextColor(colorgrey);
    tft.setTextDatum(TL_DATUM);
    tft.setTextSize(1);
    tft.drawString("From: " + senderMac, 30, 120);

    if (strcmp(incoming->msg, "Who is Device X?") == 0) {
        strcpy(responseMsg.msg, "I am Device X");
        tft.setTextColor(colorgrey);
        tft.setTextDatum(TL_DATUM);
        tft.setTextSize(1);
        tft.drawString("Replying: I am Device X", 30, 100);

        // Envoi en broadcast
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&responseMsg, sizeof(responseMsg));
        
        if (result != ESP_OK) {
            Serial.println("Error sending the response. Error code: " + String(result));
            tft.setTextColor(colorgrey);
            tft.setTextDatum(TL_DATUM);
            tft.setTextSize(1);
            tft.drawString("Error sending the response", 30, 25);
        }
    }
}
void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    pinMode(TFT_PWR_PIN, OUTPUT);
    pinMode(TFT_BL_PIN, OUTPUT);
    digitalWrite(TFT_PWR_PIN, HIGH); 
    digitalWrite(TFT_BL_PIN, HIGH);  

    tft.begin();
    tft.setRotation(3);
    tft.setSwapBytes(true); 
    tft.fillScreen(TFT_BLACK);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        tft.setTextColor(colorgrey);
        tft.setTextDatum(TL_DATUM);
        tft.setTextSize(1);
        tft.drawString("Error initializing ESP-NOW", 30, 40);
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);

    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    //peerInfo.ifidx = ESP_IF_WIFI_STA; 
    peerInfo.ifidx = WIFI_IF_STA;


    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        tft.setTextColor(colorgrey);
        tft.setTextDatum(TL_DATUM);
        tft.setTextSize(1);
        tft.drawString("Failed to add peer", 30, 55);
        return;
    }

    Serial.println("Receiver set up");
    tft.setTextColor(colorgrey);
    tft.setTextDatum(TL_DATUM);
    tft.setTextSize(1);
    tft.drawString("Receiver set up", 30, 70);
}

void loop() {
    delay(1000);
}
