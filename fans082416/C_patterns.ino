
    void no_pattern() {
      for(uint8_t i=0; i<nLEDs; i++) {
        for(int j = 0; j<nStrips; j++) {
          strips[j].setPixelColor(i,0,0,0);
        }
      }
      digitalWrite(ledPin,HIGH);

    }

    void compass_test() {
      int r;
      int g;
      int b;
      Serial.println(yaw);
      if (yaw>=0 and yaw<120) {
        r = map(yaw,0,120,127,0);
        g = map(yaw,0,120,0,127);
        b = 0;
      } else if (yaw>=120 && yaw<180) {
        r = 0;
        g = map(yaw,120,180,127,64);
        b = map(yaw,120,180,0,63);
      } else if (yaw>=180 && yaw<240) {
        r = 0;
        g = map(yaw,180,240,63,0);
        b = map(yaw,180,240,64,127);
      } else {
        r = map(yaw,240,360,0,127);
        g = 0;
        b = map(yaw,240,360,127,0);
      }
      for(uint8_t i=0; i<nLEDs; i++) {
        for(int j = 0; j<nStrips; j++) {
          strips[j].setPixelColor(i,r,g,b);
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

    void acel_test() {
      int r = 0;
      int g = 0;
      int b = 0;
      static int p = 0;
      p = (p+1)%2;
      float xt = 5;
      float yt = 5;
      float zt = 5;
      if (abs(ay)>= yt) {
        r = p*127;
      }
      //else
      if (abs(az) >= zt) {
        g = p*127;
      }
      //else
      if (abs(ax) >= xt) {
        b = p*127;
      }
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
      float gr = constrain(abs(gx),0,10)/10.0;
      float gg = constrain(abs(gy),0,10)/10.0;
      float gb = constrain(abs(gz),0,10)/10.0;
      gr = gr*gr;
      gg = gg*gg;
      gb = gb*gb;
      
      // what function dims the low end?
      Serial.println(gr);
      for(uint8_t i=0; i<nLEDs; i++) {
        for(int j = 0; j<nStrips; j++) {
          strips[j].setPixelColor(i,gr*127,gg*127,gb*127);
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
    static int sparkle = -1;
    if (inPlane==true && toggled==true) {
      Serial.println("Switch it!");
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
    // Measure linear movement, with a maximum
    float brightness = max(abs(vx),max(abs(vy),abs(vz)));
    brightness = constrain(brightness,0,2);
    // re-map that from 50% to 100%.
    brightness = brightness*50;
    brightness = map(brightness,0,100,10,100);
    brightness = brightness / 100;
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
          strips[j].setPixelColor(i,r,g,b);
          //strips[j].setPixelColor(i,brightness*r,brightness*g,brightness*b);
        } else {
          strips[j].setPixelColor(i,0,0,0);
        }
      }
    }
  }
