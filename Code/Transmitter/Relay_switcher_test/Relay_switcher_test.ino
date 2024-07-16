#define pincount 6
const int pins[pincount] = {14,27,26,25,32,33}; //loop order 1 to 6
//const int pins[pincount] = {18,19,3,1,23,12}; //loop order 1 to 6
int pinstate[pincount] = {0,0,0,0,0,0};
// const int pins[pincount] = {15,2,4,16,5,17}; //loop order 1 to 6
void setup() {
Serial.begin(115200);
Serial.setTimeout(50);

for(int i = 0; i <pincount; i++){
  pinMode(pins[i],OUTPUT);
  Serial.print("Activated Pin: ");
  Serial.print(pins[i]);
  Serial.println(" as output");
}

}

void loop() {
//loopover();
if (Serial.available() >= 1) {
ser_rec();}

}

void loopover(){
for(int j=0; j<pincount; j++){
  digitalWrite(pins[j],1);
  Serial.print("Pin: [");
  Serial.print(pins[j]);
  Serial.println("] High");
  delay(1000);
  digitalWrite(pins[j],0);
  Serial.print("Pin: [");
  Serial.print(pins[j]);
  Serial.println("] Low");
  delay(500);

}
}

void ser_rec(){

pinstate[0] = Serial.parseInt();
pinstate[1] = Serial.parseInt();
pinstate[2] = Serial.parseInt();
pinstate[3] = Serial.parseInt();
pinstate[4] = Serial.parseInt();
pinstate[5] = Serial.parseInt();
int over = Serial.parseInt();

for(int j=0; j<pincount; j++){
  pinstate[j] = min(pinstate[j],1);
  digitalWrite(pins[j],pinstate[j]);
  Serial.print("Pin: ");
  Serial.print(pins[j]);
  Serial.print("--[");
  Serial.print(pinstate[j]);
  Serial.print("]  | ");
}
Serial.println();
delay(500);}