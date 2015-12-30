float bpitch = -99;
float byaw = -99;
float broll = -99;
float vx = 0;
float vy = 0;
float vz = 0;
float lastAcc[3]= {0,0,0};
float trueAcc[3] = {0,0,0};
bool in_plane = true;
bool plane_flip = false;

void Vector_by_Matrix(float a[3], float b[3][3], float v[3]) {
  float op[3];
  for(int i=0; i<3; i++) {
    for(int j=0; j<3; j++) {
      op[j]=a[j]*b[j][i];
      //op[j]=a[j]*b[i][j];
    }
    v[i]=op[0]+op[1]+op[2];
  }
}
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
  return unwind(a + diff(b,a)/k);
}

void rolling_angles() {
  uyaw = yaw + PI;
  upitch = roll + PI;
  uroll = (yaw>=0) ? (pitch+PI/2) : (1.5*PI-pitch);
  // nudge is pulling inexhorably toward teetering across 0/2PI
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
  static float thresh = PI/8;
  if ((abs(diff(upitch,bpitch))<thresh) && (abs(diff(uroll,broll))<thresh)) {
    in_plane = true;
  } else if ((abs(diff(upitch,bpitch))>(PI-thresh)) || (abs(diff(uroll,broll))>(PI-thresh))) {
    in_plane = true;
    plane_flip = 1 - plane_flip;
    bpitch = upitch;
    broll = uroll;
  } else {
    in_plane = false;
  }
}


void reckon() {
  //these vectors are definitely not adjusted for gravity, but they do seem to be the relative directions
  static int lambda = 0.9999;
  vx = lambda*vx + G_Dt*trueAcc[0];
  vy = lambda*vy + G_Dt*trueAcc[1];
  vz = lambda*vz + G_Dt*trueAcc[2];
}

/*
x
nose down: -1, 0, 0  ; -1, 0, 0 
tips down: -0.5, -1, 1
flat: 0, 0, 0 ; flipped: 0, 0, 0
*/
void slide() {
  float rG[3];
  Vector_by_Matrix(G,DCM_Matrix,rG);
  trueAcc[0] = accel_x - rG[0];
  trueAcc[1] = accel_y - rG[1];
  trueAcc[2] = accel_z - rG[2];
  static int gthresh = 1000; //totally no idea
  static float vthresh = 3; //totally have no idea
  slide_x = false;
  slide_y = false;
  slide_z = false;
  if (gyro_x < gthresh && gyro_y < gthresh && gyro_z < gthresh) {
    if (abs(vx) > vthresh) {
      slide_x = true;
    }
    if (abs(vy) > vthresh) {
      slide_y = true;
    }  
    if (abs(vz) > vthresh) {
      slide_z = true;
    }
  }
}



