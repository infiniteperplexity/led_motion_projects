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

float gx;
float gy;
float gz;

float az;
 float ax;
 float ay;
 float vx = 0;
 float vy = 0;
 float vz = 0;
 float tick = 0.02;
 
bool inPlane = false;
float pThresh = 2;
float sThresh = 5;
int outMax = 5;
int inMin = 10;
int nIn = 0;
int nOut = 0;
bool toggled = false;

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
      imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
      //gx works and it's the foldy plane...7 is reasonably high
      gx = gyro.x();
      //gy works and it's the twisty plane...same scale
      gy = gyro.y();
      //gz works and it's the twirly plane
      gz = gyro.z();
      // gather linear acceleration readings
      imu::Vector<3> linear = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
      // linear X works and is toward the USB port...12 is somewhat high
      ax = linear.x();
      // linear Y works and is toward the fan tines or handle
      ay = linear.y();
      // linear Z works and is the foldy plane
      az = linear.z();

      //estimate the velocity;
      static float lambda = 0.9;
      vx = lambda*vx + tick*ax;
      vy = lambda*vy + tick*ay;
      vz = lambda*vz + tick*az;
      
      toggled = false;
      Serial.println(nIn);
      if (abs(gz) < sThresh && (abs(gx) >= pThresh || abs(gy) >= pThresh)) {
        nOut+=1;
        if (nOut>=outMax) {
          nIn = 0;
          inPlane = false;
        }
      } else {
        nIn+=1;
        nOut=0;
        if (nIn >= inMin && inPlane==false) { 
          inPlane = true;
          toggled = true;
        }
      }
  }
    void readings() {
//      Serial.print("Yaw: ");
//      Serial.print(yaw);
//      Serial.print("\tPitch: ");
//      Serial.print(pitch);
//      Serial.print("\tRoll: ");
//      Serial.print(roll);
//      Serial.println("");

    }
    int mode = 4;
    void paint() {
      //eventually this should use function pointers
      switch(mode) {
        case 1:
          gyro_test();
        break;
        case 2:
          slide_test();
        break;
        case 3:
          plane_test();
        break;
        case 4:
          multi_pattern();
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

    void slide_test() {
      static int p = 0;
      p = (p+1)%2;
      int r = 0;
      int g = 0;
      int b = 0;
      float vvx = constrain(vx,-2,2);
      float vvy = constrain(vy,-2,2);
      float vvz = constrain(vz,-2,2);
      //Serial.println(vvx);
      r = p*map(abs(vvx),0,2,0,127);
      g = p*map(abs(vvy),0,2,0,127);
      b = p*map(abs(vvz),0,2,0,127);
      for(uint8_t i=0; i<nLEDs; i++) {
        for(int j = 0; j<nStrips; j++) {
          strips[j].setPixelColor(i,r,g,b);
        }
      }
    }
    
    void gyro_test() {
      int r = 0;
      int g = 0;
      int b = 0;
      static int p = 0;
      p = (p+1)%2;
      float xt = 5;
      float yt = 5;
      float zt = 5;
      if (abs(gy)>= yt) {
        r = p*127;
      }
      //else 
      if (abs(gz) >= zt) {
        g = p*127;
      }
      //else
      if (abs(gx) >= xt) {
        b = p*127;   
      }
      for(uint8_t i=0; i<nLEDs; i++) {
        for(int j = 0; j<nStrips; j++) {
          strips[j].setPixelColor(i,r,g,b);
        }
      }
    }
    
    void plane_test() {
      int r;
      int g;
      int b;
      //Serial.println(nIn);
      static int color = 0;
      switch(color) {
        case 0:
          r = 127;
          g = 0;
          b = 0;
        break;
        case 1:
          r = 127;
          g = 127;
          b = 0;
        break;
        case 2:
          r = 0;
          g = 127;
          b = 0;
        break;
        case 3:
          r = 0;
          g = 127;
          b = 127;
        break;
        case 4:
          r = 0;
          g = 0;
          b = 127;
        break;
        default:
          r = 127;
          g = 0;
          b = 127;
        break;
      }
      if (inPlane==true && toggled==true) {
        color = (color+1)%6;
      }
      for(uint8_t i=0; i<nLEDs; i++) {
        for(int j = 0; j<nStrips; j++) {
          strips[j].setPixelColor(i,r,g,b);
        }
      }
    }
    
    
   void multi_pattern() {
    int r = 0;
    int g = 0;
    int b = 0;
    static int color = 0;
    static int strobe_state = 1;
    static int sparkle = -1;
    if (inPlane==true && toggled==true) {
      color = (color+1)%6;
    }
    switch(color) {
      case 0:
        r = 127;
        g = 0;
        b = 0;
      break;
      case 1:
        r = 127;
        g = 127;
        b = 0;
      break;
      case 2:
        r = 0;
        g = 127;
        b = 0;
      break;
      case 3:
        r = 0;
        g = 127;
        b = 127;
      break;
      case 4:
        r = 0;
        g = 0;
        b = 127;
      break;
      default:
        r = 127;
        g = 0;
        b = 127;
      break;
    }
    float slidet = 4;
    if (abs(ax)>=slidet || abs(ay)>=slidet) {
      strobe_state = (strobe_state+1)%2;
    } else {
      strobe_state = 1;
    }
    float gt = 5;
    if (abs(gz)>=gt) {
      sparkle = (sparkle+1)%nLEDs;
    } else {
      sparkle = -1;
    }
    for(uint8_t i=0; i<nLEDs; i++) {
      for(int j = 0; j<nStrips; j++) {
        if (i==sparkle) {
          strips[j].setPixelColor(i,127,127,127);
        } else if (sparkle==-1) { 
          strips[j].setPixelColor(i,strobe_state*r,strobe_state*g,strobe_state*b);
        } else {
          strips[j].setPixelColor(i,0,0,0);
        }
      }
    }  
  }
