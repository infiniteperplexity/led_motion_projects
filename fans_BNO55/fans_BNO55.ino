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
      
      /* Display the floating point data */
      Serial.print("X: ");
      Serial.print(event.orientation.x, 4); //+0 - +360 wraps
      Serial.print("\tY: ");
      Serial.print(event.orientation.y, 4); //-80 - +80 no wrap
      Serial.print("\tZ: ");
      Serial.print(event.orientation.z, 4); // -180 - +180 wraps
      Serial.println("");
      
      delay(100);
    }

