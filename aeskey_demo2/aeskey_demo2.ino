#include <AESLib.h>
#include <Preferences.h>

Preferences preferences;
AESLib aesLib;

void storeAESKey(uint8_t* key, size_t len) {
  preferences.begin("aes-settings", false);
  preferences.putBytes("aesKey", key, len);
  preferences.end();
}

bool readStoredAESKey(uint8_t* key, size_t len) {
  preferences.begin("aes-settings", true);
  size_t keyLen = preferences.getBytes("aesKey", key, len);
  preferences.end();
  return keyLen == len;
}

bool isAESKeyStored() {
  uint8_t tempKey[16];
  return readStoredAESKey(tempKey, sizeof(tempKey));
}

String encodeMessage(String message) {
  uint8_t key[16];
  readStoredAESKey(key, sizeof(key));

  byte iv[N_BLOCK];
  for (int i = 0; i < N_BLOCK; i++) {
    iv[i] = random(0, 256);
  }

  byte encryptedMessage[message.length() + N_BLOCK];
  uint16_t encryptedSize = aesLib.encrypt((byte*)message.c_str(), message.length(), encryptedMessage, key, sizeof(key), iv);

  String encrypted = "";
  for (int i = 0; i < N_BLOCK; i++) {
    encrypted += String(iv[i], HEX);
  }
  for (uint16_t i = 0; i < encryptedSize; i++) {
    encrypted += String(encryptedMessage[i], HEX);
  }

  return encrypted;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  randomSeed(analogRead(0));

  Serial.println("Looking for stored AES key...");
  if (!isAESKeyStored()) {
    Serial.println("No key stored");
     uint8_t newKey[16];
      for (int i = 0; i < 16; i++) {
        newKey[i] = random(0, 256);
      }
    storeAESKey(newKey, sizeof(newKey));
    Serial.println("New AES key generated and stored.");
  } else {
    Serial.println("Key Found");
    uint8_t storedKey[16];
    readStoredAESKey(storedKey, sizeof(storedKey));
    for (int i = 0; i < sizeof(storedKey); i++) {
      Serial.print(storedKey[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

  String plaintext = "Bonjour, ceci est un message";
  Serial.println("Plaintext: " + plaintext);
  String encrypted = encodeMessage(plaintext);
  Serial.println("Encrypted: " + encrypted);

  // Display IV separately
  Serial.print("IV: ");
  for (int i = 0; i < N_BLOCK; i++) {
      Serial.print(String(encrypted[i], HEX));
  }
  Serial.println();

}

void loop() {
  // Your regular loop code
}
