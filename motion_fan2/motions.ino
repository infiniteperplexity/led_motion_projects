////okay...say we wanted to detect "in-plane" vs "break-plane"
// yaw is irrelevant
// you'd ideally want to take the "max" of roll and pitch
// pitch goes -PI/2 to PI/2 and wraps
// roll goes -PI to PI and doesn't wrap
// so for pitch, you'd do...ugh...well...from 0 you'd do abs(change)
// for roll you'd also...well...depends what it means by "does't wrap"

// so whata layers do we need?
//// we need first-degree buffering to eliminate pure noise
//  - seems simple...we do a rolling average, or else one of those other filters from the signals class
//// then we need an algorithm to lock in the "home plane"
//  - so I figure we take the min and max of the rolling average.  and...we need to re-map this somehow.  we need a function...i dunno.
//    anyway, if the min and max are close enough together, we "lock in"
//// finally we need to detect a "plane break"
//  - that's when we're outside the bounds of the plane in term of either pitch or roll...for a long enough time.
//// what about "opposite plane"?  I think there we'd simply detect if we were in the opposite plane in terms of either pitch or roll.

// so I can't actually recall what "doesn't wrap" means.  I assume it means it bounces back down, but...that's terrible and useless.
//unfortunately this impacts everything

//so...


// layer 1

static int BUFFSIZE = 10;
float yaw_buffer[BUFFSIZE] = [0,0,0,0,0,0,0,0,0,0];
float roll_buffer[BUFFSIZE] = [0,0,0,0,0,0,0,0,0,0];
float pitch_buffer[BUFFSIZE] = [0,0,0,0,0,0,0,0,0,0];
float buff_yaw;
float buff_pitch;
float buff_roll;
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

/*
// layer 2

static int LONGSIZE = 60;
float[LONGSIZE] long_yaw = [0,0,0,0,0,0,0,0,0,0];
float[LONGSIZE] long_roll = [];
float[LONGSIZE] long_pitch = [];
float max_roll, max_yaw, max_pitch, min_roll, min_yaw, min_pitch;
bool in_plane = true;
float[2] center_plane = [0,0];
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
  max_yaw = max(long_yaw);
  max_pitch = max(long_pitch);
  max_roll = max(long_roll);
  min_yaw = min(long_yaw);
  min_pitch = min(long_pitch);
  min_roll = min(long_roll);
  if (max_pitch - min_pitch < plane_threshold && max_roll - min_roll < plane_threshold) {
    in_plane = true;
    center_plane = [(max_pitch + min_pitch)/2,(max_roll+min_roll)/2];
  } else {
    in_plane = false;
    if ((max_pitch + min_pitch)/2 - center_plane[0] > PI/2) || (max_roll + min_roll)/2 - center_plane[1] > PI/2) {
      plane_flip = true;
    }
  }
}
*/
