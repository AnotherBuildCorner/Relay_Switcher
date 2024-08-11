#include <esp_now.h>
#include <WiFi.h>
#define runpin 13

#define pincount 6
const int pins_1[pincount] = {14,27,26,25,33,32};
bool pinstate_1[pincount] = {0,0,0,0,0,0};

//const int pins_2[pincount] = {18,19,3,1,23,12};
const int pins_2[pincount] = {18,19,18,19,23,12};
bool pinstate_2[pincount] = {0,0,0,0,0,0};
// Disable pins 1 and 3 to enable serial coms during runtime
const int pins_3[pincount] = {15,2,4,16,5,17};
bool pinstate_3[pincount] = {0,0,0,0,0,0};

bool printouts = true;

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
#define CHANNEL 1

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
  WiFi.mode(WIFI_STA);
  delay(2000);
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  for(int i = 0; i <pincount; i++){
  pinMode(pins_1[i],OUTPUT);
  Serial.print("Activated Pin: ");
  Serial.print(pins_1[i]);
  Serial.println(" as output");
}
  for(int i = 0; i <pincount; i++){
  pinMode(pins_2[i],OUTPUT);
  Serial.print("Activated Pin: ");
  Serial.print(pins_2[i]);
  Serial.println(" as output");
}
  for(int i = 0; i <pincount; i++){
  pinMode(pins_3[i],OUTPUT);
  Serial.print("Activated Pin: ");
  Serial.print(pins_3[i]);
  Serial.println(" as output");
}

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;}
  esp_now_register_recv_cb(OnDataRecv);

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


    for(int j=0; j<pincount; j++){
  digitalWrite(pins_1[j],pinstate_1[j]);
}

for(int j=0; j<pincount; j++){
  digitalWrite(pins_2[j],pinstate_2[j]);
}

for(int j=0; j<pincount; j++){
  digitalWrite(pins_3[j],pinstate_3[j]);
}
}}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&Data, incomingData, sizeof(Data));
  pinstate_1[0] = Data.loop1;
  pinstate_1[1] = Data.loop2;
  pinstate_1[2] = Data.loop3;
  pinstate_1[3] = Data.loop4;
  pinstate_1[4] = Data.loop5;
  pinstate_1[5] = Data.loop6;

  pinstate_2[0] = Data.loop7;
  pinstate_2[1] = Data.loop8;
  pinstate_2[2] = Data.loop9;
  pinstate_2[3] = Data.loop10;
  pinstate_2[4] = Data.loop11;
  pinstate_2[5] = Data.loop12;

  pinstate_3[0] = Data.loop13;
  pinstate_3[1] = Data.loop14;
  pinstate_3[2] = Data.loop15;
  pinstate_3[3] = Data.loop16;
  pinstate_3[4] = Data.loop17;
  pinstate_3[5] = Data.loop18;
/*
  pinstate_2[0] = Data.loop1;
  pinstate_2[1] = Data.loop2;
  pinstate_2[2] = Data.loop3;
  pinstate_2[3] = Data.loop4;
  pinstate_2[4] = Data.loop5;
  pinstate_2[5] = Data.loop6;

  pinstate_3[0] = Data.loop1;
  pinstate_3[1] = Data.loop2;
  pinstate_3[2] = Data.loop3;
  pinstate_3[3] = Data.loop4;
  pinstate_3[4] = Data.loop5;
  pinstate_3[5] = Data.loop6;*/


for(int j=0; j<pincount; j++){
  digitalWrite(pins_1[j],pinstate_1[j]);
  if(printouts == true){
  Serial.print("Jack 1 Pin: ");
  Serial.print(pins_1[j]);
  Serial.print("--[");
  Serial.print(pinstate_1[j]);
  Serial.print("]  | ");}
  if(j == pincount){Serial.println();}
}

for(int j=0; j<pincount; j++){
  digitalWrite(pins_2[j],pinstate_2[j]);
  if(printouts == true){
  Serial.print("Jack 2 Pin: ");
  Serial.print(pins_2[j]);
  Serial.print("--[");
  Serial.print(pinstate_2[j]);
  Serial.print("]  | ");}
  if(j == pincount){Serial.println();}
}

for(int j=0; j<pincount; j++){
  digitalWrite(pins_3[j],pinstate_3[j]);
  if(printouts == true){
  Serial.print("Jack 1 Pin: ");
  Serial.print(pins_3[j]);
  Serial.print("--[");
  Serial.print(pinstate_3[j]);
  Serial.print("]  | ");}
  if(j == pincount){Serial.println();}
}



}