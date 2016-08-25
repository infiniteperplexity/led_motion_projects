  int yaw;
  int pitch;
  int roll;
  float gx;
  float gy;
  float gz;
  float az = 0;
  float ax = 0;
  float ay = 0;
  float ax0;
  float ay0;
  float az0;
  float cx=0;
  float cy=0;
  float cz=0;
  float cx0;
  float cy0;
  float cz0;
  float cvx;
  float cvy;
  float cvz;

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
      imu::Vector<3> compass = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
      cx0 = cx;
      cy0 = cy;
      cz0 = cz;
      cx = compass.x();
      cy = compass.y();
      cz = compass.z();
      Serial.print(cx);
      Serial.print("  ");
      Serial.print(cy);
      Serial.print("  ");
      Serial.print(cz);
      Serial.println("  ");
      cvx = cx-cx0;
      cvy = cy-cy0;
      cvz = cz-cz0;
      //estimate the velocity;
      estimate_velocity();

      // Detect plane breaks - works pretty well!
      static float toggletimer = 0;
      float toggletimeout = 500;
      if (toggletimer>0) {
        toggletimer = toggletimer-tick;
      }
      toggled = false;
      if (abs(gz) < sThresh && (abs(gx) >= pThresh || abs(gy) >= pThresh)) {
        nOut+=1;
        if (nOut>=outMax) {
          nIn = 0;
          inPlane = false;
        }
      } else if (toggletimer>0){
        nIn+=1;
        nOut=0;
        if (nIn >= inMin && inPlane==false) {
          inPlane = true;
          toggled = true;
          toggletimer = toggletimeout;
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
