    #include <Arduino.h>
    #include <Wire.h>
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BNO055.h>
    #include <utility/imumaths.h>
    #include <Adafruit_NeoPixel.h>
    #include <EEPROM.h>
    long timer=0;   //general purpuse timer
    long timer_old;


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
      int fanNumber;
      EEPROM.get(numAddress, fanNumber);
      if (fanNumber==1) {
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
        readings();
        Read_Switch();
        paint();
      }
    }

float gx;
float gy;
float gz;

float az = 0;
 float ax = 0;
 float ay = 0;
 float ax0;
 float ay0;
 float az0;
 float jx;
 float jy;
 float jz;
 float vx = 0;
 float vy = 0;
 float vz = 0;
 float tick = 0.02;

 float vy100 = 0;
 float vy995 = 0;
 float vy99 = 0;
 float vy95 = 0;
 float vy9 = 0;
 float vy8 = 0;

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
      estimate_velocity();

      // Detect plane breaks - works pretty well!
      toggled = false;
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

  void estimate_velocity() {
    // discrimination window
    float dwin = 0.1;
    float dwinx = dwin;
    float dwiny = dwin;
    float dwinz = dwin;
    float aax = ax;
    float aay = ay;
    float aaz = az;
    if (abs(ax)<dwinx) {
      aax = 0;
    }
    if (abs(ay)<dwiny) {
      aay = 0;
    }
    if (abs(az)<dwinz) {
      aaz = 0;
    }
    // leaky integrator
    float leak = 0.98;
    float leakx = leak;
    float leaky = leak;
    float leakz = leak;
    float dvx = tick*(aax+ax0)/2;
    float dvy = tick*(aay+ay0)/2;
    float dvz = tick*(aaz+az0)/2;
    vx = leakx*vx + dvx;
    vy = leaky*vy + dvy;
    vz = leakz*vz + dvz;
    //kill velocity and acceleration during spin
    
    float sping = 4;
    float spingx = sping;
    float spingy = sping;
    float spingz = sping;
    float spindelay = 0.25;
    static float gtimer = 0;
    if (abs(gx)>spingx || abs(gy)>spingy || abs(gz)>spingz) {
      gtimer = spindelay;
    }
    //stop detection
    float sthresh = 0.5;
    float sthreshx = sthresh;
    float sthreshy = sthresh;
    float sthreshz = sthresh;
    float stopdelay = 0.1;
    static float stoptimer = stopdelay;
    if (aax < sthreshx && aay < sthreshy && aaz < sthreshz) {
      stoptimer = max(0,stoptimer-tick);
    } else {
      stoptimer = stopdelay;
    }
    gtimer = max(0,gtimer-tick);
    if (
        gtimer>0
        ||
        stoptimer==0
      ) {
      aax = 0;
      aay = 0;
      aaz = 0;
      vx = 0;
      vy = 0;
      vz = 0;
    }
    // save previous acceleration
    ax0 = aax;
    ay0 = aay;
    az0 = aaz;
  }

  void readings() {
    //Serial.println("");
  }

  int mode = 0;
  int nModes = 2;
  void Read_Switch() {
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
      digitalWrite(ledPin,HIGH);

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
    float slidet = 0.4;
    if (abs(vx)>=slidet || abs(vy)>=slidet || abs(vz)>=slidet) {
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


// Code from Adafruit's sensor library

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
    */
/**************************************************************************/
void displaySensorDetails(void)
{
    sensor_t sensor;
    bno.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print("Sensor:       "); Serial.println(sensor.name);
    Serial.print("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
    Serial.print("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
    Serial.print("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
    */
/**************************************************************************/
void displaySensorStatus(void)
{
    /* Get the system status values (mostly for debugging purposes) */
    uint8_t system_status, self_test_results, system_error;
    system_status = self_test_results = system_error = 0;
    bno.getSystemStatus(&system_status, &self_test_results, &system_error);

    /* Display the results in the Serial Monitor */
    Serial.println("");
    Serial.print("System Status: 0x");
    Serial.println(system_status, HEX);
    Serial.print("Self Test:     0x");
    Serial.println(self_test_results, HEX);
    Serial.print("System Error:  0x");
    Serial.println(system_error, HEX);
    Serial.println("");
    delay(500);
}

/**************************************************************************/
/*
    Display sensor calibration status
    */
/**************************************************************************/
void displayCalStatus(void)
{
    /* Get the four calibration values (0..3) */
    /* Any sensor data reporting 0 should be ignored, */
    /* 3 means 'fully calibrated" */
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);

    /* The data should be ignored until the system calibration is > 0 */
    Serial.print("\t");
    if (!system)
    {
        Serial.print("! ");
    }

    /* Display the individual values */
    Serial.print("Sys:");
    Serial.print(system, DEC);
    Serial.print(" G:");
    Serial.print(gyro, DEC);
    Serial.print(" A:");
    Serial.print(accel, DEC);
    Serial.print(" M:");
    Serial.print(mag, DEC);
}

/**************************************************************************/
/*
    Display the raw calibration offset and radius data
    */
/**************************************************************************/
void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
{
    Serial.print("Accelerometer: ");
    Serial.print(calibData.accel_offset_x); Serial.print(" ");
    Serial.print(calibData.accel_offset_y); Serial.print(" ");
    Serial.print(calibData.accel_offset_z); Serial.print(" ");

    Serial.print("\nGyro: ");
    Serial.print(calibData.gyro_offset_x); Serial.print(" ");
    Serial.print(calibData.gyro_offset_y); Serial.print(" ");
    Serial.print(calibData.gyro_offset_z); Serial.print(" ");

    Serial.print("\nMag: ");
    Serial.print(calibData.mag_offset_x); Serial.print(" ");
    Serial.print(calibData.mag_offset_y); Serial.print(" ");
    Serial.print(calibData.mag_offset_z); Serial.print(" ");

    Serial.print("\nAccel Radius: ");
    Serial.print(calibData.accel_radius);

    Serial.print("\nMag Radius: ");
    Serial.print(calibData.mag_radius);
}

#define BNO055_SAMPLERATE_DELAY_MS (100)

void checkCalibration() {
  int eeAddress = 0;
  long bnoID;
  bool foundCalib = false;
  EEPROM.get(eeAddress, bnoID);
  adafruit_bno055_offsets_t calibrationData;
  sensor_t sensor;
  /*
    *  Look for the sensor's unique ID at the beginning oF EEPROM.
    *  This isn't foolproof, but it's better than nothing.
  */
  bno.getSensor(&sensor);
  if (bnoID != sensor.sensor_id) {
    Serial.println("\nNo Calibration Data for this sensor exists in EEPROM");
    delay(500);
  } else {
    Serial.println("\nFound Calibration for this sensor in EEPROM.");
    eeAddress += sizeof(long);
    EEPROM.get(eeAddress, calibrationData);
    displaySensorOffsets(calibrationData);
    Serial.println("\n\nRestoring Calibration data to the BNO055...");
    bno.setSensorOffsets(calibrationData);
    Serial.println("\n\nCalibration data loaded into BNO055");
    foundCalib = true;
  }

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Optional: Display current status */
  displaySensorStatus();

  sensors_event_t event;
  bno.getEvent(&event);
  if (foundCalib){
    Serial.println("Move sensor slightly to calibrate magnetometers");
    /*while (!bno.isFullyCalibrated()) {
      bno.getEvent(&event);
      delay(BNO055_SAMPLERATE_DELAY_MS);
    }*/
  } else {
    Serial.println("Please Calibrate Sensor: ");
    while (!bno.isFullyCalibrated()) {
      bno.getEvent(&event);
      Serial.print("X: ");
      Serial.print(event.orientation.x, 4);
      Serial.print("\tY: ");
      Serial.print(event.orientation.y, 4);
      Serial.print("\tZ: ");
      Serial.print(event.orientation.z, 4);
      /* Optional: Display calibration status */
      displayCalStatus();
      /* New line for the next sample */
      Serial.println("");
      /* Wait the specified delay before requesting new data */
      delay(BNO055_SAMPLERATE_DELAY_MS);
    }
  }
  Serial.println("\nFully calibrated!");
  Serial.println("--------------------------------");
  Serial.println("Calibration Results: ");
  adafruit_bno055_offsets_t newCalib;
  bno.getSensorOffsets(newCalib);
  displaySensorOffsets(newCalib);

  if (foundCalib==false) {
    Serial.println("\n\nStoring calibration data to EEPROM...");

    eeAddress = 0;
    bno.getSensor(&sensor);
    bnoID = sensor.sensor_id;

    EEPROM.put(eeAddress, bnoID);

    eeAddress += sizeof(long);
    EEPROM.put(eeAddress, newCalib);
    Serial.println("Data stored to EEPROM.");

    Serial.println("\n--------------------------------\n");
    delay(500);
  }
}
