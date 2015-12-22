float buff_pitch = PI;
float buff_yaw = PI;
float buff_roll = PI;
float lastAcc[3]= {0,0,0};

void Measure_Shake() {
        shake = sqrt(pow(AN[0] - lastAcc[0],2) + pow(AN[1] - lastAcc[1],2) + pow(AN[2] - lastAcc[2],2));
        for(int i=0; i<3; i++) {
                lastAcc[i] = AN[i];
        }
}
//angle utilities
float unwind(float a) {
  while (a>=2*PI) {
    a-=(2*PI);
  }
  while (a<0) {
    a+=(2*PI);
  }
  return a;
}

//returns difference and average
void compare(float a, float b, float *r) {
  if (a>b) {
    if (a-b<PI) {
      r[0] = a-b;
      r[1] = unwind((a+b)/2);
    } else {
      r[0] = 2*PI+b-a;
      r[1] = unwind((2*PI+a+b)/2);
    }
  } else {
    if (b-a<PI) {
      r[0] = b-a;
      r[1] = unwind((a+b)/2);
    } else {
      r[0] = 2*PI+a-b;
      r[1] = unwind((2*PI+a+b)/2);
    }
  }
}
void bounds(float b[], int len, float *r) {
  float mx = b[0];
  float mn = b[0];
  for (int i=1; i<len; i++) {
    if (b[i]<mn) {
      mn = b[i];
    }
    if (b[i]>mx) {
      mx = b[i];
    }
  }
  r[0] = mn;
  r[1] = mx;
}


// alternate layer 1...doesn't work for shit
static int REGRESS = 10;
void update_buffers() {
  float x[2];
  compare(buff_yaw,uyaw,x);
  buff_yaw = unwind(buff_yaw+x[0]/REGRESS);
  compare(buff_pitch,upitch,x);
  buff_pitch = unwind(buff_pitch+x[0]/REGRESS);
  compare(buff_roll,uroll,x);
  buff_roll = unwind(buff_roll+x[0]/REGRESS);
}

// layer 1
// layer 2
static int LONGSIZE = 60;
float long_yaw[60];
float long_roll[60];
float long_pitch[60];
float max_roll, max_yaw, max_pitch, min_roll, min_yaw, min_pitch;
bool in_plane = true;
bool plane_flip = false;
float center_plane[2] = {0,0};
static int plane_threshold = 0.5;
void long_buffers() {
  //maybe should wait until filled
  //maybe needs to deal with wrapping
  for (int i = 0; i < LONGSIZE-1; i++) {
    long_yaw[i] = long_yaw[i+1];
    long_pitch[i] = long_yaw[i+1];
    long_roll[i] = long_roll[i+1];
  }
  long_yaw[LONGSIZE-1] = uyaw;
  long_pitch[LONGSIZE-1] = upitch;
  long_roll[LONGSIZE-1] = uroll;
  // I doubt this function exists in C
  float yaws[2], pitches[2], rolls[2];
  bounds(long_yaw,LONGSIZE, yaws);
  Serial.println(yaws[0]);
  compare(yaws[0],yaws[1],yaws);
  bounds(long_pitch,LONGSIZE,pitches);
  compare(pitches[0],pitches[1],pitches);
  bounds(long_roll,LONGSIZE,rolls);
  compare(rolls[0],rolls[1],rolls);
  //Serial.println(pitches[0]);
  if (pitches[0] < plane_threshold && rolls[0] < plane_threshold) {
    in_plane = true;
    //Serial.println("in plane");
    center_plane[0] = pitches[1];
    center_plane[1] = rolls[1];
  } else {
    in_plane = false;
    if (((pitches[1]-center_plane[0]) > (PI/2-plane_threshold)) || ((rolls[1]-center_plane[1]) > (PI/2-plane_threshold))) {
      plane_flip = true;
      //Serial.println("plane flip!");
      center_plane[0] = pitches[1];
      center_plane[1] = rolls[1];
    } else {
      //Serial.println("out of plane");
    }
  }
}
