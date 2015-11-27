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
int ledGround = 20;
int dataPin  = 21;
int clockPin = 22;
int ledPower = 23;
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);


void setup() {
  // set up pins
  pinMode(imuPower, OUTPUT);
  pinMode(imuGround, OUTPUT);
  digitalWrite(imuPower, HIGH);
  digitalWrite(imuGround, LOW);
  pinMode(ledPower, OUTPUT);
  pinMode(ledGround, OUTPUT);
  digitalWrite(ledPower, HIGH);
  digitalWrite(ledGround, LOW);
  delay(2500);
  Serial.begin(115200);
  // begin I2C communication with IMU
  AHRS_Init();
  strip.begin();
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
    readings();
    paint();
    strip.show();
  }
}

void readings() {
  //Serial.print(pitch);
  //Serial.print(" ");
  //Serial.print(roll);
  //Serial.print(" ");
  //Serial.print(yaw);
  //Serial.println("");
  //Serial.println(shake);
  //Serial.print(trueAcc[0]);
  //Serial.print(velocity[0]);
  Serial.print(position[0]);
  Serial.print(" ");
  //Serial.print(trueAcc[1]);
  //Serial.print(velocity[1]);
  Serial.print(position[1]);
  Serial.print(" ");
  //Serial.println(trueAcc[2]);
  //Serial.println(velocity[2]);
  Serial.println(position[2]);
}

void paint() {
  //int r = map(100*pitch,-50*PI,50*PI,0,127);
  //int g = map(100*abs(roll),0,100*PI,0,127);
  //int b = map(100*abs(yaw),0,100*PI,0,127);
  int r = map(abs(shake),-50*PI,50*PI,0,127);
  int g = map(10*abs(y),0,100*PI,0,127);
  int b = map(10*abs(z),0,100*PI,0,127);
  for(uint8_t i=0; i<nLEDs; i++) {
    strip.setPixelColor(i,r,g,b);
  }
}
