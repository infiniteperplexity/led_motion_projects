float bpitch = -99;
float byaw = -99;
float broll = -99;
float vx = 0;
float vy = 0;
float vz = 0;
float lastAcc[3]= {0,0,0};
bool in_plane = true;
bool plane_flip = false;

void Measure_Shake() {
        shake = sqrt(pow(AN[0] - lastAcc[0],2) + pow(AN[1] - lastAcc[1],2) + pow(AN[2] - lastAcc[2],2));
        for(int i=0; i<3; i++) {
                lastAcc[i] = AN[i];
        }
}
////angle utilities...remember these are for absolutes, not changes
//normalize angle to the range 0-2*PI
float unwind(float a) {
  while (a>=2*PI) {
    a-=(2*PI);
  }
  while (a<0) {
    a+=(2*PI);
  }
  return a;
}
//calculate the shortest distance between two angles, may be negative
float diff(float a, float b) {
  a = unwind(a);
  b = unwind(b);
  if (a>b) {
    if (a-b<PI) {
      return(a-b);
    } else {
      return(2*PI+b-a);
    }
  } else {
    if (b-a<PI) {
      return(a-b);
    } else {
      return(-2*PI+b-a);
    }
  }
}
// nudge A one Kth of the way toward B
float nudge(float a, float b, float k) {
  return unwind(a + diff(a,b)/k);
}

void rolling_angles() {
  if (bpitch == -99) {
    bpitch = upitch;
  } else {
    bpitch = nudge(bpitch, upitch, 10);
  }
  if (broll == -99) {
    broll = uroll;
  } else {
    broll = nudge(broll, uroll, 10);
  }
  if (byaw == -99) {
    byaw = uyaw;
  } else {
    byaw = nudge(byaw, uyaw, 10);
  }
}

void plane_break() {
  static int threshold = PI/8;
  if ((abs(diff(upitch,bpitch))<threshold) && (abs(diff(uroll,broll))<threshold)) {
    in_plane = true;
  } else if ((abs(diff(upitch,bpitch))>(PI-threshold)) || (abs(diff(uroll,broll))>(PI-threshold))) {
    in_plane = true;
    plane_flip = 1 - plane_flip;
    bpitch = upitch;
    broll = uroll;
  } else {
    in_plane = false;
  }
}


void reckon() {
  static int lambda = 0.8;
  vx = lambda*vx + G_Dt*accel_x;
  vy = lambda*vy + G_Dt*accel_y;
  vz = lambda*vz + G_Dt*accel_z;
}

void slide() {
  static int gthresh = 100; //totally no idea
  static int vthresh = 100; //totally have no idea
  slide_x = false;
  slide_y = false;
  slide_z = false;
  if (gyro_x < gthresh && gyro_y < gthresh && gyro_z < gthresh) {
    if (vx > vthresh) {
      slide_x = true;
    }
    if (vy > vthresh) {
      slide_y = true;
    }
    if (vz > vthresh) {
      slide_z = true;
    }
  }
}

