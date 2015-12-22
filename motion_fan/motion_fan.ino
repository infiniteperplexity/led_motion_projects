#include "LPD8806.h"
#include "definitions.h"
long timer=0;   //general purpuse timer
long timer_old;

// Number of RGB LEDs in strand:
int nLEDs = 8;
int nStrips = 5;
// Chose 2 pins for output; can be any valid output pins:
int imuPower = 16;
int imuGround = 17;
int clockPin = 6;
int dataPin1 = 7;
int dataPin2 = 8;
int dataPin3 = 9;
int dataPin4 = 10;
int dataPin5 = 11;
int switchPin = 12;
int ledPin = 13;
LPD8806 strip1 = LPD8806(nLEDs, dataPin1, clockPin);
LPD8806 strip2 = LPD8806(nLEDs, dataPin2, clockPin);
  // set up pins
LPD8806 strip3 = LPD8806(nLEDs, dataPin3, clockPin);
LPD8806 strip4 = LPD8806(nLEDs, dataPin4, clockPin);
LPD8806 strip5 = LPD8806(nLEDs, dataPin5, clockPin);
LPD8806 strips[5] = {strip1, strip2, strip3, strip4, strip5};

void setup() {
  pinMode(imuPower, OUTPUT);
  pinMode(imuGround, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(imuPower, HIGH);
  digitalWrite(imuGround, LOW);
  delay(2500);
  Serial.begin(115200);
  // begin I2C communication with IMU
  AHRS_Init();
  for(int i=0; i<nStrips; i++) {
    strips[i].begin();
  }
  timer=millis();
  delay(20);
}

void loop() {
  if((millis()-timer)>=20) {  // Main loop runs at 50Hz
    timer_old = timer;
    timer=millis();
    //AHRS_Update();
    //Measure_Shake();
    Read_Switch();
    //readings();
    //paint();
    rolling_angles();
  }
}

int mode = 1;
int nModes = 4;
void Read_Switch() {
  static int readState = LOW;
  int reading = digitalRead(switchPin);
  if (reading==LOW && readState==HIGH) {
    mode = (mode+1)%nModes;   
  }
  readState = reading; 
}

void paint() {
  //eventually this should use function pointers
  switch(mode) {
    case 1:
      shake_test();
    break;
    case 2:
      gyro_test();
      break;
    case 3:
      orient_test();
    break;
    default: // typically case 0
      no_pattern();
    break;
  }
  for(int i=0; i<nStrips; i++) {
    strips[i].show();
  }
}

void readings() {
  //Serial.print(" mode: ");
  //Serial.print(mode);
  //Serial.print(" shake: ");
  //Serial.print(shake);
  Serial.print(" yaw: ");
  Serial.print(uyaw);
  Serial.print(" roll: ");
  Serial.print(uroll);
  Serial.print(" pitch: ");
  Serial.print(upitch);
  Serial.println("");
}

