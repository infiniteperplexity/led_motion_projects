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
  float threshold = 50;
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
      r = 255;
      g = 0;
      b = 0;
    break;
    case 1:
      r = 255;
      g = 255;
      b = 0;
    break;
    case 2:
      r = 0;
      g = 255;
      b = 0;
    break;
    case 3:
      r = 0;
      g = 255;
      b = 255;
    break;
    case 4:
      r = 0;
      g = 0;
      b = 255;
    break;
    default:
      r = 255;
      g = 0;
      b = 255;
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
  static int r = 255;
  static int g = 0;
  static int b = 0;
  static int rStep = -1;
  static int gStep = 1;
  static int bStep = 0;
  int xThresh = 50;
  int yThresh = 50;
  int zThresh = 50;
  if (gyro_x >= xThresh) {
    if (r==0 && rStep==-1) {
      rStep = 1;
    } else if (r==255 && rStep==1) {
      rStep = -1;
    }
    r+=rStep;
  }
  if (gyro_y >= yThresh) {
    if (g==0 && gStep==-1) {
      gStep = 1;
    } else if (g==255 && gStep==1) {
      gStep = -1;
    }
    g+=gStep;
  }
  if (gyro_z >= zThresh) {
    if (b==0 && bStep==-1) {
      bStep = 1;
    } else if (b==255 && bStep==1) {
      bStep = -1;
    }
    b+=bStep;    
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
  if (yaw>=0 and yaw<(2*PI/3)) {
    r = map(yaw,0,2*PI/3,255,0);
    g = map(yaw,0,2*PI/3,0,255);
    b = 0;    
  } else if (yaw>=(2*PI/3) && yaw<PI) {
    r = 0;
    g = map(yaw,2*PI/3,PI,255,128);
    b = map(yaw,2*PI/3,PI,0,127);
  } else if (yaw>=-PI && yaw<(-2*PI/3)) {
    r = 0;
    g = map(yaw,-PI,-2*PI/3,127,0);
    b = map(yaw,-PI,-2*PI/3,128,255);
  } else {
    r = map(yaw,-2*PI/3,0,0,255);
    g = 0;
    b = map(yaw,-2*PI/3,0,255,0);
  }
  for(uint8_t i=0; i<nLEDs; i++) {
    strip1.setPixelColor(i,r,g,b);
    strip2.setPixelColor(i,r,g,b);
    strip3.setPixelColor(i,r,g,b);
    strip4.setPixelColor(i,r,g,b);
    strip5.setPixelColor(i,r,g,b);
  }
}
