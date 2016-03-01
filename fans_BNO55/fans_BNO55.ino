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

    int yaw;
    int pitch;
    int roll;
    float vyaw;
    float vpitch;
    float vroll;

    
    Adafruit_BNO055 bno = Adafruit_BNO055(55);

    void setup(void) {
      pinMode(imuPower, OUTPUT);
      pinMode(imuGround, OUTPUT);
      pinMode(switchPin, INPUT);
      pinMode(ledPin, OUTPUT);
      digitalWrite(imuPower, HIGH);
      digitalWrite(imuGround, LOW);
      delay(10);
      Serial.begin(9600);
      /* Initialise the sensor */
      if(!bno.begin())
      {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
      }
      delay(1000); // is this delay necessary?
      bno.setExtCrystalUse(true);
      for(int i=0; i<nStrips; i++) {
        strips[i].begin();
      }
      timer=millis();
      delay(20);
    }

    void loop(void) {
      if((millis()-timer)>=20) {  // Main loop runs at 50Hz
        timer_old = timer;
        timer=millis();
 
        sensors();
        readings();
        paint();
      }
    }

    void sensors() {
      /* Get a new sensor event */
      sensors_event_t event;
      bno.getEvent(&event);
      // Scale all three Euler angles to 0-360
      yaw = event.orientation.x;
      pitch = event.orientation.z+180;
      roll = (event.orientation.x>180) ? (event.orientation.y+90) : (270-event.orientation.y);
      // gather gyroscope readings
      vyaw = event.gyro.z;
      vpitch = event.gyro.y;
      vroll = event.gyro.x;
      // gather linear acceleration readings
      // gather compass readings
    }
    void readings() {
      Serial.print("Yaw: ");
      Serial.print(yaw);
      Serial.print("\tPitch: ");
      Serial.print(pitch);
      Serial.print("\tRoll: ");
      Serial.print(roll);
      Serial.println("");



      Serial.print("V Yaw: ");
      Serial.print(vyaw);
      Serial.print("\tV Pitch: ");
      Serial.print(vpitch);
      Serial.print("\tV Roll: ");
      Serial.print(vroll);
      Serial.println(""); 
    }
    int mode = 0;
    void paint() {
      //eventually this should use function pointers
      switch(mode) {
        case 1:
          gyro_test();
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

    void no_pattern() {
      for(uint8_t i=0; i<nLEDs; i++) {
        for(int j = 0; j<nStrips; j++) {
          strips[j].setPixelColor(i,0,0,0);
        }
      }
    }
    
    void gyro_test() {
      int r = 0;
      int g = 0;
      int b = 0;
      static int p = 0;
      p = (p+1)%2;
      int rollThresh = 2000;
      int pitchThresh = 2000;
      int yawThresh = 2000;
      if (vroll>= rollThresh) {
        r = 127;
      } else {
        r = 1;
      }
      if (vpitch >= rollThresh) {
        g = 127;
      } else {
        g = 1;
      }
      if (vyaw >= rollThresh) {
        b = 127;   
      } else {
        b = 1;
      }
      if ((p%2)==0) {
        r = 0;
        g = 0;
        b = 0;
      }
      for(uint8_t i=0; i<nLEDs; i++) {
        for(int j = 0; j<nStrips; j++) {
          strips[j].setPixelColor(i,r,g,b);
        }
      }
    }
