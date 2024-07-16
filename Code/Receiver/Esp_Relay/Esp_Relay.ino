#include <esp_now.h>
#include <WiFi.h>
#define runpin 13

#define pincount 6
const int pins[pincount] = {14,27,26,25,32,33};
bool pinstate[pincount] = {0,0,0,0,0,0};

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
}DataStruct;
DataStruct Data;
#define CHANNEL 1

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  for(int i = 0; i <pincount; i++){
  pinMode(pins[i],OUTPUT);

  Serial.print("Activated Pin: ");
  Serial.print(pins[i]);
  Serial.println(" as output");
}

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;}
  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {

}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&Data, incomingData, sizeof(Data));
  pinstate[0] = Data.loop1;
  pinstate[1] = Data.loop2;
  pinstate[2] = Data.loop3;
  pinstate[3] = Data.loop4;
  pinstate[4] = Data.loop5;
  pinstate[5] = Data.loop6;

for(int j=0; j<pincount; j++){
  digitalWrite(pins[j],pinstate[j]);
  Serial.print("Pin: ");
  Serial.print(pins[j]);
  Serial.print("--[");
  Serial.print(pinstate[j]);
  Serial.print("]  | ");
}
Serial.println();

}