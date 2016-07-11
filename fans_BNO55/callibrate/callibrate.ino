    #include <Arduino.h>
    #include <Wire.h>
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BNO055.h>
    #include <utility/imumaths.h>
    #include <Adafruit_NeoPixel.h>
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
    Adafruit_BNO055 bno = Adafruit_BNO055(55);
    
    
void setup() {
  // put your setup code here, to run once:
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
      delay(20);
displayCalStatus();
}

void loop() {
  // put your main code here, to run repeatedly:
displayCalStatus();
}

void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  
  uint8_t = read8(BNO055_CALIB_STAT_ADDR);
  
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
  Serial.println(mag, DEC);
}
