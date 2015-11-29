#include "Wire.h"
#include "L3G.h"
#include "LSM303.h" //causes mysterious error
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#include "LPD8806.h"

long timer=0;   //general purpuse timer
long timer_old;
long timer24=0; //Second timer used to print values

// Euler angles
float pitch; // -PI/2 to PI/2, no wrap
float roll; // -PI to PI, wraps around
float yaw; // -PI to PI, wraps around

//other readings
float twist;
float dip;
float tilt;
float x;
float y;
float z;
float vx;
float vy;
float vz;
float shake;
float lastAcc[3]= {0,0,0};
float trueAcc[3] = {0,0,0};
float velocity[3] = {0,0,0};
float position[3] = {0,0,0};

// Example to control LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 8;
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
LPD8806 strip3 = LPD8806(nLEDs, dataPin3, clockPin);
LPD8806 strip4 = LPD8806(nLEDs, dataPin4, clockPin);
LPD8806 strip5 = LPD8806(nLEDs, dataPin5, clockPin);

void setup() {
  // set up pins
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
  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip4.begin();
  strip5.begin();
  timer=millis();
  delay(20);
}
void loop() {
  if((millis()-timer)>=20) {  // Main loop runs at 50Hz
    timer_old = timer;
    timer=millis();
    AHRS_Update();
    Measure_Shake();
    Dead_Reckon();
    Read_Switch();
    readings();
    //paint();
  }
}
  `
void readings() {
  Serial.print(" shake: ");
  Serial.print(shake);
  Serial.print(" gyro x:");
  Serial.print(gyro_x);
  Serial.print(" gyro y:");
  Serial.print(gyro_y);
  Serial.print(" gyro z:");
  Serial.print(gyro_z);
  Serial.println("");
}

int mode = 1;
int nModes = 4;
void Read_Switch() {
  static bool readState = LOW;
  bool reading = digitalRead(switchPin);
  if (reading==HIGH && readState==LOW) {
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
    case 3:
      orient_test();
    break;
    default: // typically case 0
      no_pattern();
    break;
  }
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  strip5.show();  
}
