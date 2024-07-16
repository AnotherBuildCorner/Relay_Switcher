#include <esp_now.h>
#include <WiFi.h>

#define SlaveCnt 2

// Structure to hold peer information
typedef struct {
  uint8_t peer_addr[6];
} SlaveInfo;

SlaveInfo slaves[SlaveCnt];

uint8_t broadcastAddresses[][6] = {
  {0xC8, 0x2E, 0x18, 0xEF, 0xE1, 0xE0}, // Light Test ESP
  {0x40, 0x22, 0xD8, 0x77, 0x15, 0xA4}, // Train Test ESP

  
};

// Define the structure that will hold the data to be sent over ESP-NOW
typedef struct DataStruct {
bool loop1;
bool loop2;
bool loop3;
bool loop4;
bool loop5;
bool loop6;
bool loop7;
bool loop8;
bool loop9;
bool loop10;
bool loop11;
bool loop12;
bool loop13;
bool loop14;
bool loop15;
bool loop16;
bool loop17;
bool loop18;
}DataStruct;
DataStruct Data;
int BufferClear;
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  /*char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");*/
}

void setup() {
 Serial.begin(115200);
 Serial.setTimeout(10);
 pinMode(4, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

    for (int i = 0; i < SlaveCnt; i++) {
    memcpy(peerInfo.peer_addr, broadcastAddresses[i], 6);
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
    }
  }
  
}




void loop() {
if (Serial.available() >= 2 ){
    // Read the integers from Serial
    Data.loop1 = Serial.parseInt();
    Data.loop2 = Serial.parseInt();
    Data.loop3 = Serial.parseInt();
    Data.loop4 = Serial.parseInt();
    Data.loop5 = Serial.parseInt();
    Data.loop6 = Serial.parseInt();
    Data.loop7 = Serial.parseInt();
    Data.loop8 = Serial.parseInt();
    Data.loop9 = Serial.parseInt();
    Data.loop10 = Serial.parseInt();
    Data.loop11 = Serial.parseInt();
    Data.loop12 = Serial.parseInt();
    Data.loop13 = Serial.parseInt();
    Data.loop14 = Serial.parseInt();
    Data.loop15 = Serial.parseInt();
    Data.loop16 = Serial.parseInt();
    Data.loop17 = Serial.parseInt();
    Data.loop18 = Serial.parseInt();

    int BufferClear = Serial.parseInt(); 
    esp_err_t result = esp_now_send(0, (uint8_t *) &Data, sizeof(DataStruct));  
  }

}

