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
