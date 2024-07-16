#include <WiFi.h>
#include <esp_now.h>

// Define the structure to send
struct LoopData {
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
};

LoopData loops;

const int ledPins[3] = {16, 17, 18};
const int buttonPins[3] = {34, 35, 32};
bool ledStates[3] = {false, false, false};
unsigned long buttonPressTime[3] = {0, 0, 0};
bool buttonHeld[3] = {false, false, false};
const unsigned long holdTime = 1000; // Time in milliseconds to consider as a hold
bool change = false;

// Peer MAC address (replace with the MAC address of the receiver)
uint8_t peerMACAddress[] = {0xC8, 0x2E, 0x18, 0xEF, 0xE1, 0xE0};

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLDOWN);
  }

  // Initialize the loops struct to false
  memset(&loops, 0, sizeof(loops));

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, peerMACAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    bool buttonState = digitalRead(buttonPins[i]) == HIGH;

    if (buttonState) {
      change = true;
      if (buttonPressTime[i] == 0) {
        buttonPressTime[i] = millis(); // Start timing
      } else if (!buttonHeld[i] && (millis() - buttonPressTime[i] > holdTime)) {
        buttonHeld[i] = true;
        toggleLoop(i, true); // Button held
      }
    } else {
      if (buttonPressTime[i] != 0) {
        if (!buttonHeld[i]) {
          toggleLoop(i, false); // Button press
        }
        buttonPressTime[i] = 0;
        buttonHeld[i] = false;
      }
    }

    // Update LED state
    digitalWrite(ledPins[i], ledStates[i]);
  }
  if(change == true){
  sendLoopData();}
  delay(50); // Small delay to debounce buttons
  change = false;
}

void toggleLoop(int buttonIndex, bool held) {
  switch (buttonIndex) {
    case 0:
      if (held) {
        loops.loop2 = !loops.loop2;
      } else {
        loops.loop1 = !loops.loop1;
      }
      ledStates[buttonIndex] = max(loops.loop1,loops.loop2);
      break;
    case 1:
      if (held) {
        loops.loop4 = !loops.loop4;
      } else {
        loops.loop3 = !loops.loop3;
      }
      ledStates[buttonIndex] = max(loops.loop3,loops.loop4);
      break;
    case 2:
      if (held) {
        loops.loop6 = !loops.loop6;
      } else {
        loops.loop5 = !loops.loop5;
      }
        ledStates[buttonIndex] = max(loops.loop5,loops.loop6);
      break;
    // Add more cases if you have more buttons and loops
  }
}

void sendLoopData() {
  esp_err_t result = esp_now_send(peerMACAddress, (uint8_t *) &loops, sizeof(loops));

  if (result != ESP_OK) {
    Serial.println("Loop data sent successfully");

  }
}

