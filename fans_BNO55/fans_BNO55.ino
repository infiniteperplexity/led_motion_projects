    #include <Arduino.h>
    #include <Wire.h>
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BNO055.h>
    #include <utility/imumaths.h>
    #include <SPI.h>
    #include "LPD8806.h"
    long timer=0;   //general purpuse timer
    long timer_old;
    // Number of RGB LEDs in strand:
    int nLEDs = 4;
    int nStrips = 5;
    // Chose 2 pins for output; can be any valid output pins:
    int imuPower = 15;
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
    LPD8806 strips[5] = {strip1, strip2, strip3, strip4, strip5};

    Adafruit_BNO055 bno = Adafruit_BNO055(55);

    void setup(void)
    {
      pinMode(imuPower, OUTPUT);
      pinMode(imuGround, OUTPUT);
      //pinMode(switchPin, INPUT);
      //pinMode(ledPin, OUTPUT);
      digitalWrite(imuPower, HIGH);
      digitalWrite(imuGround, LOW);
      delay(10);
      Serial.begin(9600);
      Serial.println("Orientation Sensor Test"); Serial.println("");
    
      /* Initialise the sensor */
      if(!bno.begin())
      {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
      }

      delay(1000);

      bno.setExtCrystalUse(true);
      delay(2500);
      for(int i=0; i<nStrips; i++) {
        strips[i].begin();
      }
      delay(2500);
    }

    void loop(void)
    {
      /* Get a new sensor event */
      sensors_event_t event;
      bno.getEvent(&event);
/*
      // Scale all three Euler angles to 0-360
      int yaw = event.orientation.x;
      int pitch = event.orientation.z+180;
      int roll = (event.orientation.x>180) ? (event.orientation.y+90) : (270-event.orientation.y);

      Serial.print("Yaw: ");
      Serial.print(yaw);
      Serial.print("\tPitch: ");
      Serial.print(pitch);
      Serial.print("\tRoll: ");
      Serial.print(roll);
      Serial.println("");

      float vyaw = event.gyro.z;
      float vpitch = event.gyro.y;
      float vroll = event.gyro.x;

      Serial.print("V Yaw: ");
      Serial.print(vyaw);
      Serial.print("\tV Pitch: ");
      Serial.print(vpitch);
      Serial.print("\tV Roll: ");
      Serial.print(vroll);
      Serial.println("");
  */
      paint();


      //float vx = (read8(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR) << 8) | (read8(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR ));
      //float vy = (read8(BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR) << 8) | (read8(BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR ));
      //float vz = (read8(BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR) << 8) | (read8(BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR ));

      //Serial.print("VX: ");
      //Serial.print(vx);
      //Serial.print("\tVY: ");
      //Serial.print(vy);
      //Serial.print("\tVZ: ");
      //Serial.print(vz);
      //Serial.println("");

      delay(100);
    }
    
    void paint() {
      for(uint8_t i=0; i<nLEDs; i++) {
        for(uint8_t j = 0; j<nStrips; j++) {
          strips[j].setPixelColor(i,100,0,0);
          Serial.println(j);
        }
      }
      for(uint8_t j = 0; j<nStrips; j++) {
        strips[j].show();
      }
    }
