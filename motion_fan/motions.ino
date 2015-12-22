float bpitch = -99;
float byaw = -99;
float broll = -99;
float lastAcc[3]= {0,0,0};

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
