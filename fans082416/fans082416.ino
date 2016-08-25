    #include <Arduino.h>
    #include <Wire.h>
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BNO055.h>
    #include <utility/imumaths.h>
    #include <Adafruit_NeoPixel.h>
    #include <EEPROM.h>
    long timer=0;   //general purpuse timer
    long timer_old;

    // Manual status flags
    bool fan_1_recalibrate = false;
    bool fan_1_skip_calibrate = false;
    bool fan_2_recalibrate = false;
    bool fan_2_skip_calibrate = false;


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
    int nStrips = 5;
    // Number of RGB LEDs in strand:
    int nLEDs;
    int fanNumber;
    Adafruit_NeoPixel strip1;
    Adafruit_NeoPixel strip2;
    Adafruit_NeoPixel strip3;
    Adafruit_NeoPixel strip4;
    Adafruit_NeoPixel strip5;
    Adafruit_NeoPixel strips[5];

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
      delay(250);
      if (Serial) {
        nLEDs = 4;
      } else {
        nLEDs = 8;
      }
      strip1 = Adafruit_NeoPixel(nLEDs, dataPin1);
      strip2 = Adafruit_NeoPixel(nLEDs, dataPin2);
      strip3 = Adafruit_NeoPixel(nLEDs, dataPin3);
      strip4 = Adafruit_NeoPixel(nLEDs, dataPin4);
      strip5 = Adafruit_NeoPixel(nLEDs, dataPin5);
      strips[0] = strip1;
      strips[1] = strip2;
      strips[2] = strip3;
      strips[3] = strip4;
      strips[4] = strip5;

      /* Initialise the sensor */
      if(!bno.begin())
      {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
      }
      // check callibration
      // but only for fan #1.  fan #2 is horribly miscalibrated for some reason
      int numAddress = 666;

      EEPROM.get(numAddress, fanNumber);
      if ((fanNumber==1 && fan_1_skip_calibrate==false) || (fanNumber==2 && fan_2_skip_calibrate==false)) {
        checkCalibration();
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
        button();
        paint();
      }
    }
  int mode = 0;
  int nModes = 2;
  void button() {
    static int readState = LOW;
    int reading = digitalRead(switchPin);
    if (reading==LOW && readState==HIGH) {
      mode = (mode+1)%nModes;
    }
    readState = reading;
  }

  void paint() {
    digitalWrite(ledPin,LOW);
    //eventually this should use function pointers
    switch(mode) {
      case 1:
        //multi_pattern();
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
