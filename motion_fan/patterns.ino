void no_pattern() {
  for(uint8_t i=0; i<nLEDs; i++) {
    strip1.setPixelColor(i,0,0,0);
    strip2.setPixelColor(i,0,0,0);
    strip3.setPixelColor(i,0,0,0);
    strip4.setPixelColor(i,0,0,0);
    strip5.setPixelColor(i,0,0,0);
  }
}


void shake_test() {
  static int color = 0;
  static bool shakeState = 0;
  float threshold = 1500;
  //may need some timing conditionals here
  if (shake>=threshold) {
    if (shakeState==0) {
      color=(color+1)%6;
    }
    shakeState = 1;
  } else {
    shakeState = 0;
  }
  int r;
  int g;
  int b;
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
  for(uint8_t i=0; i<nLEDs; i++) {
    strip1.setPixelColor(i,r,g,b);
    strip2.setPixelColor(i,r,g,b);
    strip3.setPixelColor(i,r,g,b);
    strip4.setPixelColor(i,r,g,b);
    strip5.setPixelColor(i,r,g,b);
  }  
}
  
void gyro_test() {
  int r = 0;
  int g = 0;
  int b = 0;
  static int p = 0;
  p = (p+1)%2;
  int xThresh = 2000;
  int yThresh = 2000;
  int zThresh = 2000;
  if (gyro_x >= xThresh) {
    r = 127;
  } else {
    r = 1;
  }
  if (gyro_y >= yThresh) {
    g = 127;
  } else {
    g = 1;
  }
  if (gyro_z >= zThresh) {
    b = 127;   
  } else {
    b = 1;
  }
  if ((p%2)==0) {
    r = 0;
    g = 0;
    b = 0;
  }
  for(uint8_t i=0; i<nLEDs; i++) {
    strip1.setPixelColor(i,r,g,b);
    strip2.setPixelColor(i,r,g,b);
    strip3.setPixelColor(i,r,g,b);
    strip4.setPixelColor(i,r,g,b);
    strip5.setPixelColor(i,r,g,b);
  }
}

void orient_test() {
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
    strip1.setPixelColor(i,r,g,b);
    strip2.setPixelColor(i,r,g,b);
    strip3.setPixelColor(i,r,g,b);
    strip4.setPixelColor(i,r,g,b);
    strip5.setPixelColor(i,r,g,b);
  }
}
