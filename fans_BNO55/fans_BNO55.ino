    #include <Wire.h>
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BNO055.h>
    #include <utility/imumaths.h>

    Adafruit_BNO055 bno = Adafruit_BNO055(55);

    void setup(void)
    {
      pinMode(15, OUTPUT);
      pinMode(17, OUTPUT);
      //pinMode(switchPin, INPUT);
      //pinMode(ledPin, OUTPUT);
      digitalWrite(15, HIGH);
      digitalWrite(17, LOW);
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
    }

    void loop(void)
    {
      /* Get a new sensor event */
      sensors_event_t event;
      bno.getEvent(&event);

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



      float vx = (read8(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR) << 8) | (read8(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR ));
      float vy = (read8(BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR) << 8) | (read8(BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR ));
      float vz = (read8(BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR) << 8) | (read8(BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR ));

      Serial.print("VX: ");
      Serial.print(vx);
      Serial.print("\tVY: ");
      Serial.print(vy);
      Serial.print("\tVZ: ");
      Serial.print(vz);
      Serial.println("");

      delay(100);
    }

/*    BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR                     = 0X28,
    BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR                     = 0X29,
    BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR                     = 0X2A,
    BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR                     = 0X2B,
    BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR                     = 0X2C,
    BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR                     = 0X2D,

VECTOR_LINEARACCEL   = BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR,

offsets_type.gyro_offset_x = (read8(GYRO_OFFSET_X_MSB_ADDR) << 8) | (read8(GYRO_OFFSET_X_LSB_ADDR));
    */
