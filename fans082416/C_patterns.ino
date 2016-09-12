/*
Types of pattern...
- Sparkles chasing up or down tines
- Gradients chasing up or down tines
- Sparkles in place
- Strobing solid colors
- Strobing between colors
- Changing solid colors
- Square grid-based patterns
- Fade to brighter or dimmer
- Fade across rainbow
*/
/*
Types of motion...
- Integrated linear velocity
- Differentiated compass velocity
- Angular velocity (x, y, z)
- Linear acceleration
- Compass orientation
- Absolute linear acceleration or velocity relative to gravity?
- Halts in angular velocity (i.e. plane change)
- Stillness
*/
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

    void compass_test() {
      int r;
      int g;
      int b;
      int yyaw = yaw*100;
      if (yyaw>=0 and yyaw<(200*PI/3)) {
        r = map(yyaw,0,200*PI/3,127,0);
        g = map(yyaw,0,200*PI/3,0,127);
        b = 0;
      } else if (yyaw>=(200*PI/3) && yyaw<(100*PI)) {
        r = 0;
        g = map(yyaw,200*PI/3,100*PI,127,64);
        b = map(yyaw,200*PI/3,100*PI,0,63);
      } else if (yyaw>=(-100*PI) && yyaw<(-200*PI/3)) {
        r = 0;
        g = map(yyaw,-100*PI,-200*PI/3,63,0);
        b = map(yyaw,-100*PI,-200*PI/3,64,127);
      } else {
        r = map(yyaw,-200*PI/3,0,0,127);
        g = 0;
        b = map(yyaw,-200*PI/3,0,127,0);
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
    // Measure linear movement, with a maximum
    float brightness = max(abs(vx),abs(vy),abs(vz),1.0);
    // re-map that from 50% to 100%.
    brightness = brightness*100;
    brightness = map(brightness,0,100,50,100);
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
          strips[j].setPixelColor(i,brightness*r,brightness*g,brightness*b);
        } else {
          strips[j].setPixelColor(i,0,0,0);
        }
      }
    }
  }
