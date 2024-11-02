#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>
#include <WiFi.h>
#include <FastLED.h>

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define screen_switch_time 0
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const bool loops_m1[6] = {0,0,0,0,0,0};
const bool loops_m2[6] = {0,0,0,0,0,0};
const bool loops_m3[6] = {0,0,0,0,0,0};

bool loops[19] = {0,loops_m1,loops_m2,loops_m3};
unsigned long timers[4]={0,0,0,0};

const int push_array[2][8] = {{1,3,5,8,9,10,11,12},{0,0,0,0,0,0,0,0}};  //put loop number to be switched by a given switch, first array is main, second is expansion.
const int hold_array[2][8] = {{2,4,6,7,0,0,0,0},{0,0,0,0,0,0,0,0}}; //loop number on the press/hold function on a given Loop, 0 should mean no hold function
const bool toggle_mode[2][8] = {{true,true,true,true,true,false,false,false},{false,false,false,false,false,false,false,false}};  //true means when hold active, the push controls the hold pedal,  false means push controls 1, hold controls the other.
bool mode_state[2][8] = {{false,false,false,false,false,false,false,false},{false,false,false,false,false,false,false,false}}; //false for hold inactive, true for hold active.
#define LED_DATA_PIN_1 15
#define LED_DATA_PIN_2 12
#define LED_DATA_PIN_3 13
#define NUM_LEDS 9
#define NUM_STATES 6
CRGB leds_1[NUM_LEDS];
CRGB leds_2[NUM_LEDS];
CRGB leds_3[1];
const int pwr = 30;
const int red = 75;
int Switch_Status[2][NUM_LEDS] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};// 0 for off, 1 for push on, 2 for hold active, 3 for hold on, 4 for both active, 5 for toggle off
int colors[NUM_STATES][3] = {{0,0,0},{red,0,0},{pwr,0,pwr},{0,0,pwr},{red,pwr,0},{0,pwr,0}};

const char* labels_m1[] = {"Face","Bender","Broadcast","Bad Bob","BlackBox","Revival"};
const char* labels_m2[] = {"Doppler","Flange","Volante","Megabyte","Tensor","Mercury"};
const char* labels_m3[] = {"E","E","E","E","E","E"};
const int label_count = 12;

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
  int Vol;
} DataStruct;
DataStruct Data;


#define SlaveCnt 3

// Structure to hold peer information
typedef struct {
  uint8_t peer_addr[6];
} SlaveInfo;

esp_now_peer_info_t peerInfo;

SlaveInfo slaves[SlaveCnt];

uint8_t broadcastAddresses[][6] = {
  {0xC8, 0x2E, 0x18, 0xEF, 0xE1, 0xE0}, // Light Test ESP
  {0x40, 0x22, 0xD8, 0x77, 0x15, 0xA4},
  {0xF0, 0xF5, 0xBD, 0x2D, 0x0D, 0x5C}, // Train Test ESP
};


// Rotary encoder setup
const int encoderPinA = 32;
const int encoderPinB = 33;
const int buttonPin = 36;
const int enc_speed = 3;
int count = 0;
// MUX control pins
const int error_thresh = 1;
const int hold_thresh = 10;
const int muxcount = 1;    
const int mux[2][6] = {
   {4,16,17,14,8,1},  
  {18,19,23,5,8,1}}; //disabled mux 3, and stole pin 14 for mux 1                 
  //{25,26,27,14,8,1}}; //Pin 1,2,3,return,switch count,delay(ms)  

const int volPin = 27;
int Vol = 100;

volatile bool state = 0;
volatile int adc = 1;
int mux_deb[2][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};

// Debouncing variables
const unsigned long DEBOUNCE_DELAY = 10; // Debounce time in milliseconds
unsigned long lastDebounceTime = 0; // Last time the encoder state was checked
int lastEncoderState = LOW; // The previous state of the encoder

volatile int lastEncoderValue = 0;
volatile int encoderPosition[3] = {enc_speed, enc_speed, enc_speed};
volatile int screenPosition[3] = {1, 1, 1};

bool buttonState = false;
bool lastButtonState = false;
bool VolumeScreen = 0;
unsigned long screen_time = 0;

const int encodermax = label_count*enc_speed;

void IRAM_ATTR readEncoder() {
  unsigned long currentTime = millis();

  // Read encoder pins
  int MSB = digitalRead(encoderPinA); // MSB = most significant bit
  int LSB = digitalRead(encoderPinB); // LSB = least significant bit

  int encoded = (MSB << 1) | LSB; // converting the 2 pin value to a single number
  int sum = (lastEncoderValue << 2) | encoded; // adding it to the previous encoded value

  // Only update position if debounce time has passed
  if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderPosition[1]++;
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderPosition[1]--;

    lastEncoderValue = encoded; // store this value for next time
    lastDebounceTime = currentTime; // Update the last debounce time
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
}

void setup() {
  Serial.begin(115200);
  delay(500);
  pinMode(buttonPin, INPUT);
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(volPin, INPUT);
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN_1>(leds_1, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN_2>(leds_2, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN_3>(leds_3, 1);
  leds_1[0] = CRGB(255,0,0);
  leds_2[0] = CRGB(0,255,0);
  leds_3[0] = CRGB(0,0,255);

  // Initialize MUX control pins  
  for(int k=0;k<muxcount;k++){
    for (int j=0;j<3;j++){
      pinMode(mux[k][j], OUTPUT);
    }
  //Mux signal return pins
  pinMode(mux[k][3], INPUT_PULLUP);
  }
  //pinMode(5,INPUT);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), readEncoder, CHANGE);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.print("OLED & Encoder Working");
  display.display();

  // Initialize ESP-NOW
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
  initLoopState();
  delay(500);
  PrintSerialString1();

  for(int z = 0; z < 6; z++){
  Serial.print(loops_m1[z]);
  Serial.print(loops_m2[z]);
  Serial.print(loops_m3[z]);
  Serial.print(" | ");}
    Serial.println();

  for(int x = 0; x < 19; x++){
  Serial.print(loops[x]);
  Serial.print(" | ");}
  Serial.println();
  esp_now_send(0, (uint8_t *)&Data, sizeof(DataStruct)); //send initial states
}


//---------------------------------------------------------------------------------------------------------
void loop() {
  unsigned long ct = millis();
  bool holdtrigger = false;
  int loop;

  //VolumeHandler();

  // Check MUX buttons
  if(VolumeScreen == true){VolumeHandler();}
  else{
  screenPosition[1] = encoderPosition[1] / enc_speed;}
  for (int j = 0; j < muxcount; j++) {
    for (int i = 0; i < mux[j][4]; i++) {
      if (readMuxButton(i, j)) {
        mux_deb[j][i] += 1;
      }

      //============Hold Engaged
      if (mux_deb[j][i] >= hold_thresh && hold_array[j][i] != 0) {
        
        mux_deb[j][i] = 0;
        holdtrigger = true;
        timers[1] = ct + 500;

        if (toggle_mode[j][i] == false) { // check for toggle, or push-hold function, this is push-hold.
          switch_status_control(j, i + 1, holdtrigger, false);
          loop = hold_array[j][i];
          screenPosition[2] = loop;
          encoderPosition[2] = screenPosition[2] * enc_speed;
          toggleLoop(loop);}


         else { // toggle function here
          mode_state[j][i] = !mode_state[j][i];
          if (mode_state[j][i]) { // hold mode engaged, toggle hold loop
            Switch_Status[j][i+1] = 5;
            loop = hold_array[j][i];
            loops[loop] = 1;
            screenPosition[2] = loop;
            encoderPosition[2] = screenPosition[2] * enc_speed;
            loop = push_array[j][i];
            loops[loop] = 0;
            write_all_loops();


          } else { // hold mode disengaged
            loop = hold_array[j][i];
            loops[loop] = 0;
            loop = push_array[j][i];
            loops[loop] = 0;
            screenPosition[2] = loop;
            encoderPosition[2] = screenPosition[2] * enc_speed;
            Switch_Status[j][i+1] = 0;
            write_all_loops();


          }
          
        }
        screen_time = ct;
      } else if (mux_deb[j][i] >= error_thresh && readMuxButton(i, j) == false && ct > timers[1]) { // check for button push not hold
        timers[1] = ct + 100;
        
        if (mode_state[j][i] == true) { // hold engaged
          loop = hold_array[j][i];
          switch_status_control(j, i + 1, holdtrigger, true);
          screenPosition[2] = loop;
          encoderPosition[2] = screenPosition[2] * enc_speed;
          toggleLoop(loop);
          screen_time = ct;

        } else { // standard function
          loop = push_array[j][i];
          switch_status_control(j, i + 1, holdtrigger, false);
          screenPosition[2] = loop;
          encoderPosition[2] = screenPosition[2] * enc_speed;
          toggleLoop(loop);
          screen_time = ct;

        }
        PrintSerialString1();
      }

      if (readMuxButton(i, j) == false) {
        mux_deb[j][i] = 0;
      }
    }
  }
if(VolumeScreen == false){
  if (screenPosition[1] > label_count) {
    screenPosition[1] = 1;
    encoderPosition[1] = enc_speed;
  }

  if (screenPosition[1] < 1) {
    screenPosition[1] = label_count;
    encoderPosition[1] = enc_speed * label_count;
  }}

  // Read the button state
  
  if (millis() - screen_switch_time >= screen_time && screen_switch_time != 0) {
    encoderPosition[0] = encoderPosition[1];
    screenPosition[0] = screenPosition[1];
  } else {
    encoderPosition[0] = encoderPosition[2];
    screenPosition[0] = screenPosition[2];
 }

  buttonState = digitalRead(buttonPin); // Read button state directly

  // If the button state has changed
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) { // Button pressed (since it's a pulldown resistor)
      // Toggle loop state
      VolumeScreen = !VolumeScreen;
    }
    lastButtonState = buttonState;
  }

  if (ct + 50 > timers[0]) {
    timers[0] = ct;
    display_color_led_1(Switch_Status[0], leds_1);
    display_color_led_2(Switch_Status[1], leds_2);
    FastLED.show();
    if(VolumeScreen == true){
      updateVolDisplay();}
    else{
      updateDisplay();}
  }
}


//----------------------------------------------------------------------------------------------------------------------------------------------

bool readMuxButton(int button,int muxn) {
  digitalWrite(mux[muxn][0], bitRead(button, 0));
  digitalWrite(mux[muxn][1], bitRead(button, 1));
  digitalWrite(mux[muxn][2], bitRead(button, 2));


  delay(mux[muxn][5]); // Small delay to let the MUX settle
  bool s = !digitalRead(mux[muxn][3]);
  return s;
}

void switch_status_control(int mux, int led,bool trig, bool flip){
  int x = Switch_Status[mux][led];
  if(flip == false){
  if(trig == true){
      switch(x){ // states for hold
    case 0: Switch_Status[mux][led] = 3; break;
    case 1: Switch_Status[mux][led] = 2; break;
    case 2: Switch_Status[mux][led] = 1; break;
    case 3: Switch_Status[mux][led] = 0; break;
  }}
  else{
  switch(x){
    case 0: Switch_Status[mux][led] = 1; break;
    case 1: Switch_Status[mux][led] = 0; break;
    case 2: Switch_Status[mux][led] = 3; break;
    case 3: Switch_Status[mux][led] = 2; break;
  }}
  }
  else{
  if(trig == true){ // states for non-hold
      switch(x){
    case 0: Switch_Status[mux][led] = 5; break;
    case 1: Switch_Status[mux][led] = 4; break;
    case 4: Switch_Status[mux][led] = 1; break;
    case 5: Switch_Status[mux][led] = 0; break;

  }}
  else{
  switch(x){
    case 0: Switch_Status[mux][led] = 1; break;
    case 1: Switch_Status[mux][led] = 0; break;
    case 4: Switch_Status[mux][led] = 5; break;
    case 5: Switch_Status[mux][led] = 4; break;

  }}
}
}



void display_color_led_1(int status_array[], CRGB led_array[]){
for(int i = 1; i < NUM_LEDS; i++){
  int x = status_array[i];
  if(i < 5){
  leds_1[5-i] = CRGB(colors[x][0],colors[x][1],colors[x][2]);}
  else{
    leds_1[13-i] = CRGB(colors[x][0],colors[x][1],colors[x][2]);}
  }
  

}

void display_color_led_2(int status_array[], CRGB led_array[]){
for(int i = 1; i < NUM_LEDS; i++){
  int x = status_array[i];
  if(i < 5){
  leds_2[5-i] = CRGB(colors[x][0],colors[x][1],colors[x][2]);}
  else{
    leds_2[13-i] = CRGB(colors[x][0],colors[x][1],colors[x][2]);}
  }
  

}

void PrintSerialString1(){
  
  Serial.println("Loop status: 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 ");
    Serial.print("            ");
  Serial.print(Data.loop1);
    Serial.print(" | ");
  Serial.print(Data.loop2);
    Serial.print(" | ");
  Serial.print(Data.loop3);
    Serial.print(" | ");
  Serial.print(Data.loop4);
    Serial.print(" | ");
  Serial.print(Data.loop5);
    Serial.print(" | ");
  Serial.print(Data.loop6);
    Serial.print(" | ");
  Serial.print(Data.loop7);
    Serial.print(" | ");
  Serial.println(Data.loop8);
}

void print_mux(){
for(int x = 0; x < 3; x ++){
  for (int y = 0; y <8;y++){
    Serial.print(mux_deb[x][y]);
    Serial.print(" | ");
  }
}
Serial.println();}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.print("Selected Loop:");
  
  display.setCursor(100, 10);
  display.print(screenPosition[0]);
  display.setTextSize(2);
  display.setCursor(0, 20);
  if(screenPosition[0] > 12){
      display.print(labels_m3[screenPosition[0]-1-12]);
  }
  else if(screenPosition[0] > 6){
      display.print(labels_m2[screenPosition[0]-1-6]);
  }
  else{
    display.print(labels_m1[screenPosition[0]-1]);
  }
  
  display.setCursor(0, 40);
  display.setTextSize(1);
  display.print("Status:");
  display.setTextSize(2);
  display.setCursor(60, 50);
  display.print(getLoopState(screenPosition[0]) ? "ON" : "OFF");
  display.display();
}

void initLoopState(){
  for(int x = 0; x <6; x++){
    loops[x+1] = loops_m1[x];
    loops[x+7] = loops_m2[x];
    loops[x+13] = loops_m3[x];
  }
    Data.loop1 = loops_m1[0];
    Data.loop2= loops_m1[1];
    Data.loop3= loops_m1[2];
    Data.loop4= loops_m1[3];
    Data.loop5= loops_m1[4];
    Data.loop6= loops_m1[5];
    Data.loop7= loops_m2[0];
    Data.loop8= loops_m2[1];
    Data.loop9= loops_m2[2];
    Data.loop10= loops_m2[3];
    Data.loop11= loops_m2[4];
    Data.loop12= loops_m2[5];
    Data.loop13= loops_m3[0];
    Data.loop14= loops_m3[1];
    Data.loop15= loops_m3[2];
    Data.loop16= loops_m3[3];
    Data.loop17= loops_m3[4];
    Data.loop18= loops_m3[5];


}

bool getLoopState(int loop) {
  switch (loop) {
    case 1: return Data.loop1;
    case 2: return Data.loop2;
    case 3: return Data.loop3;
    case 4: return Data.loop4;
    case 5: return Data.loop5;
    case 6: return Data.loop6;
    case 7: return Data.loop7;
    case 8: return Data.loop8;
    case 9: return Data.loop9;
    case 10: return Data.loop10;
    case 11: return Data.loop11;
    case 12: return Data.loop12;
    case 13: return Data.loop13;
    case 14: return Data.loop14;
    case 15: return Data.loop15;
    case 16: return Data.loop16;
    case 17: return Data.loop17;
    case 18: return Data.loop18;
    case 19: return Data.Vol;
    default: return false;
  }
}

void toggleLoop(int loop) {
  int l = loop;
  loops[l] = !loops[l];
  switch (loop) {
    case 1: Data.loop1 = loops[l]; break;
    case 2: Data.loop2 = loops[l]; break;
    case 3: Data.loop3 = loops[l]; break;
    case 4: Data.loop4 = loops[l]; break;
    case 5: Data.loop5 = loops[l]; break;
    case 6: Data.loop6 = loops[l]; break;
    case 7: Data.loop7 = loops[l]; break;
    case 8: Data.loop8 = loops[l]; break;
    case 9: Data.loop9 = loops[l]; break;
    case 10: Data.loop10 = loops[l]; break;
    case 11: Data.loop11 = loops[l]; break;
    case 12: Data.loop12 = loops[l]; break;
    case 13: Data.loop13 = loops[l]; break;
    case 14: Data.loop14 = loops[l]; break;
    case 15: Data.loop15 = loops[l]; break;
    case 16: Data.loop16 = loops[l]; break;
    case 17: Data.loop17 = loops[l]; break;
    case 18: Data.loop18 = loops[l]; break;
  }
  // Send data over ESP-NOW
  esp_err_t result = esp_now_send(0, (uint8_t *)&Data, sizeof(DataStruct));
}

void write_all_loops(){
  int n = 1;
Data.loop1 = loops[n+0];
Data.loop2 = loops[n+1];
Data.loop3 = loops[n+2];
Data.loop4 = loops[n+3];
Data.loop5 = loops[n+4];
Data.loop6 = loops[n+5];
Data.loop7 = loops[n+6];
Data.loop8 = loops[n+7];
Data.loop9 = loops[n+8];
Data.loop10 = loops[n+9];
Data.loop11 = loops[n+10];
Data.loop12 = loops[n+11];
Data.loop13 = loops[n+12];
Data.loop14 = loops[n+13];
Data.loop15 = loops[n+14];
Data.loop16 = loops[n+15];
Data.loop17 = loops[n+16];
Data.loop18 = loops[n+17];

  esp_err_t result = esp_now_send(0, (uint8_t *)&Data, sizeof(DataStruct));
}

void VolumeHandler(){
  static int lastPosition = 0;
  const int stepsize = 5;
  if(lastPosition < encoderPosition[1] && Vol <= 100-stepsize){
    Vol += 5;
    lastPosition = encoderPosition[1];
    Data.Vol = Vol;
    esp_err_t result = esp_now_send(0, (uint8_t *)&Data, sizeof(DataStruct));
  }
  if(lastPosition > encoderPosition[1] && Vol >= 0+stepsize){
  Vol -= 5;
  lastPosition = encoderPosition[1];
  Data.Vol = Vol;
    esp_err_t result = esp_now_send(0, (uint8_t *)&Data, sizeof(DataStruct));
  }
  else{
    lastPosition = encoderPosition[1];
  }}
  

void updateVolDisplay() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.print("Volume:");

  display.setTextSize(2);
  display.setCursor(40, 30);
  display.print(Vol);

  display.display();
}