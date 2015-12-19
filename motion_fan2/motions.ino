// layer 1
static int BUFFSIZE = 10;
float yaw_buffer[10] = {0,0,0,0,0,0,0,0,0,0};
float roll_buffer[10] = {0,0,0,0,0,0,0,0,0,0};
float pitch_buffer[10] = {0,0,0,0,0,0,0,0,0,0};
void update_buffers() {
  //maybe should wait until filled
  //maybe needs to deal with wrapping
  for (int i = 0; i < BUFFSIZE-1; i++) {
    yaw_buffer[i] = yaw_buffer[i+1];
    pitch_buffer[i] = pitch_buffer[i+1];
    roll_buffer[i] = roll_buffer[i+1];
  }
  yaw_buffer[BUFFSIZE-1] = yaw;
  pitch_buffer[BUFFSIZE-1] = pitch;
  roll_buffer[BUFFSIZE-1] = roll;
  // I doubt this function exists in C
  buff_yaw = 0;
  buff_pitch = 0;
  buff_roll = 0;
  for (int i = 0; i<BUFFSIZE; i++) {
    buff_yaw+=yaw_buffer[i];
    buff_pitch+=pitch_buffer[i];
    buff_roll+=roll_buffer[i];
  }
  buff_yaw/=BUFFSIZE;
  buff_pitch/=BUFFSIZE;
  buff_roll/=BUFFSIZE;
}

float unwind(float a) {
  while (a>=PI) {
    a-=PI;
  }
  while (a<=-PI) {
    a+=PI;
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
  for (int i = 0; i < BUFFSIZE-1; i++) {
    long_yaw[i] = long_yaw[i+1];
    long_pitch[i] = long_yaw[i+1];
    long_roll[i] = long_roll[i+1];
  }
  long_yaw[LONGSIZE-1] = buff_yaw;
  long_pitch[LONGSIZE-1] = buff_pitch;
  long_roll[LONGSIZE-1] = buff_roll;
  // I doubt this function exists in C
  float yaws[2], pitches[2], rolls[2];
  bounds(long_yaw,LONGSIZE, yaws);
  compare(yaws[0],yaws[1],yaws);
  bounds(long_pitch,LONGSIZE,pitches);
  compare(pitches[0],pitches[1],pitches);
  bounds(long_roll,LONGSIZE,rolls);
  compare(rolls[0],rolls[1],rolls);
  if (pitches[0] < plane_threshold && rolls[0] < plane_threshold) {
    in_plane = true;
    center_plane[0] = pitches[1];
    center_plane[1] = rolls[1];
  } else {
    in_plane = false;
    if (((pitches[1]-center_plane[0]) > (PI/2-plane_threshold)) || ((rolls[1]-center_plane[1]) > (PI/2-plane_threshold))) {
      plane_flip = true;
      center_plane[0] = pitches[1];
      center_plane[1] = rolls[1];
    }
  }
}
