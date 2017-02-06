/******** Set parameters *******/

//define update speed
const uint16_t WAIT = 25;

//include libraries for gyroscope and LED strip
#include "Wire.h"
#include "L3G4200D.h"
#include "LPD8806.h"
#include "SPI.h"

//define device pins
#define LPD_DI 11
#define LPD_CI 10
#define L3G_VIN 2
#define L3G_GND 3

//define device constants
#define BAUD 9600
#define GYRO_MIN -0x8000
#define GYRO_MAX 0x8000
#define GYRO_RANGE 0x10000
#define GYRO_SCALE 2000
//if you're staring at the switch, clockwise is Y+ on the gyro
#define NUM_LEDS 38
#define NUM_COLORS 128

//create objects for gyroscope and LED strip
L3G4200D gyro;
LPD8806 strip = LPD8806(NUM_LEDS, LPD_DI, LPD_CI);


/******** Set-Up *******/
long previousMillis = 0;
void setup()
{
  //allocate power and ground pins
  pinMode(L3G_GND, OUTPUT);
  pinMode(L3G_VIN, OUTPUT);
  digitalWrite(L3G_GND, LOW); 
  digitalWrite(L3G_VIN, HIGH);
  
  //enable devices
  Serial.begin(BAUD);
  Wire.begin();
  gyro.enableDefault();
  if (GYRO_SCALE == 250) {gyro.writeReg(0x23,0x00);}
  else if (GYRO_SCALE == 500) {gyro.writeReg(0x23,0x10);}
  else {gyro.writeReg(0x23,0x30);}
  strip.begin();
  strip.show();
}

/******** Main Loop *******/


void loop()
{
  static uint8_t mode;
  gyro.read();
  mode = rotate_mode(check_input());
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > WAIT) {
    previousMillis = currentMillis;   
    play_mode(mode);
  }
}

/******** Select Mode *******/

uint8_t rotate_mode(int direct) {
  #define NMODES 9
  static uint8_t mode = 0;
  mode = (mode+NMODES+direct)%NMODES;
  return mode;
}

void play_mode(uint8_t mode) {
   switch(mode) {
     case 0:
       rainbow_road();
       break;
     case 1:
       collidoscope();    
       break;
     case 2:
       sheet_lightning(); 
       break;
     case 3: 
       lava_flow();
       break;
     case 4:
       helix_nebula();
       break;
     case 5:
       strange_angles();
       break;
     case 6:
       strobe();
       break;
     case 7:
       discotech();
       break;
     case 8:
       beam_me_up();
       break;
   }
   strip.show();
}

/******** Define Modes (Patterns) *******/


void beam_me_up() {
  float rotation = sqrt(sq(gyro.g.x)+sq(gyro.g.z));
  static uint8_t c = 0;
  uint32_t color;
  switch(c) {
     case 0:
       color = strip.Color(NUM_COLORS-1,0,0);
       break;
     case 1:
       color = strip.Color(NUM_COLORS-1,NUM_COLORS-1,0);
       break;
     case 2:
       color = strip.Color(0,NUM_COLORS-1,0);
       break;
     case 3:
       color = strip.Color(0,NUM_COLORS-1,NUM_COLORS-1);
       break;
     case 4:
       color = strip.Color(0,0,NUM_COLORS-1);
       break;
     case 5:
       color = strip.Color(NUM_COLORS-1,0,NUM_COLORS-1);
       break;
   }
  static int dir = 1;
  static boolean moving = false;
  if (rotation > 2000) {
    if (moving == false) {
      dir = 0-dir; 
    }
    moving = true;
  } else {
    moving = false;
  }
  
  int rate = map(rotation,0,2000,4,1);
  rate = constrain(rate,1,4);
  static uint8_t k = 0;
  k += 1;
  if (k>=rate) {
    k = 0;
  }
  static int p = 0;
  if (k==0) {
    p = (p+20+dir)%20;
  }
  if(p==0) {
    c = (c+1)%6;
  }
    Serial.println(p);
  for(uint8_t i=0;i<NUM_LEDS;i++) {
    if (dir==1) {
      if((i<=(18+p))&&(i>=(19-p))) {
        strip.setPixelColor(i,color);
      } else {
        strip.setPixelColor(i,0,0,0);
      }
    } else {
        if((i>=(18+p))||(i<=(19-p))) {
        strip.setPixelColor(i,color);
      } else {
        strip.setPixelColor(i,0,0,0);
      }
    }
  }
  
}

void discotech() {
  //looks great as-is
  static uint8_t c = 0;
  static uint8_t p = 0;
  c = (c+1)%6;
  float rotation = sqrt(sq(gyro.g.x)+sq(gyro.g.z));
  if (rotation>2000) {
    p = (p+1)%3;
  }
  uint32_t color;
  switch(c) {
     case 0:
       color = strip.Color(NUM_COLORS-1,0,0);
       break;
     case 1:
       color = strip.Color(NUM_COLORS-1,NUM_COLORS-1,0);
       break;
     case 2:
       color = strip.Color(0,NUM_COLORS-1,0);
       break;
     case 3:
       color = strip.Color(0,NUM_COLORS-1,NUM_COLORS-1);
       break;
     case 4:
       color = strip.Color(0,0,NUM_COLORS-1);
       break;
     case 5:
       color = strip.Color(NUM_COLORS-1,0,NUM_COLORS-1);
       break;
   }
  for(uint8_t i=0;i<NUM_LEDS;i++) {
    if (p==0) {
      if ((i>=31)||(i<=6)) {
        strip.setPixelColor(i,color);
      } else {
        strip.setPixelColor(i,0,0,0);
      }
    } else if (p==1) {
      if ((i<=13 && i>6)||(i<31 && i>=24)) {
        strip.setPixelColor(i,color);
      } else {
        strip.setPixelColor(i,0,0,0);
      }
    } else if (p==2) {
      if (i>13 && i <24) {
        strip.setPixelColor(i,color);
      } else {
        strip.setPixelColor(i,0,0,0);
      }
    }
  }
}

void strobe() {
  //I think this is good now
  static uint8_t p = 0;
  static uint8_t color = 0;
  //add hoc solution to a problem
  static boolean is_off = false;
  float rotation = sqrt(sq(gyro.g.x)+sq(gyro.g.z));
  int rate = map(rotation,0,2000,16,2);
  rate = constrain(rate,2,16);
  p = (p+1)%rate;
  if (p==0) {
    color = (color+1)%6;
  }
  if ((p < (rate/2)) || ((rotation<=2000)&&(is_off==true))) {
    for(uint8_t i=0;i<NUM_LEDS;i++) {
      if (color==0) {
        strip.setPixelColor(i,NUM_COLORS-1,0,0);
      } else if (color==1) {
        strip.setPixelColor(i,NUM_COLORS-1,NUM_COLORS-1,0);
      } else if (color==2) {
        strip.setPixelColor(i,0,NUM_COLORS-1,0);
      } else if (color==3) {
        strip.setPixelColor(i,0,NUM_COLORS-1,NUM_COLORS-1);
      } else if (color==4) {
        strip.setPixelColor(i,0,0,NUM_COLORS-1);
      } else if (color==5) {
        strip.setPixelColor(i,NUM_COLORS-1,0,NUM_COLORS-1);
      }
    }
    is_off = false;
  } 
  else if (rotation>2000) {
    for(uint8_t i=0;i<NUM_LEDS;i++) {
      strip.setPixelColor(i,0,0,0);
    }
    is_off = true;
  }
}

void rainbow_road() {
  //great as-is
  static uint16_t p = 0;
  static boolean moving = false;
  float rotation = sqrt(sq(gyro.g.x)+sq(gyro.g.z));
  static int dir = 1;
  if (rotation > 2000) {
    if (moving == false) {
      dir = 0-dir; 
    }
    moving = true;
  } else {
    moving = false;
  }
  
  p = (p+384+(dir*16))%384;
  uint8_t r, g, b;
  uint16_t k;
  for(uint8_t i=0;i<NUM_LEDS;i++) {
    k = (p+(i*8))%(384);
    if (k<NUM_COLORS) {
      r = NUM_COLORS - 1 - k % NUM_COLORS;
      g = k % NUM_COLORS;
      b = 0;
    }
    else if (k<(NUM_COLORS*2)) {
      r = 0;
      g = NUM_COLORS - 1 - k % NUM_COLORS;
      b = k % NUM_COLORS;
    }
    else {
      r = k % NUM_COLORS;
      g = 0;
      b = NUM_COLORS - 1 - k % NUM_COLORS;
    }
    strip.setPixelColor(i,strip.Color(r,g,b));
  }
}


void strange_angles() {
  //great as-is
  static int bouncers[] = {6,12,18,24,30,36};
  static int bouncesp[] = {1,2,-1,3,-2,1};
  for (uint8_t i=0;i<6;i++) {
    bouncers[i]+=bouncesp[i];
    if ((bouncers[i] >= NUM_LEDS) || (bouncers[i] < 0)) {
      bouncesp[i] = 0-bouncesp[i];
    }
  }
  
  static float angle_x = 0.0;
  static float angle_z = 0.0;
  angle_x += (float(GYRO_SCALE)/GYRO_MAX)*gyro.g.x*(WAIT/1000.0);
  angle_z += (float(GYRO_SCALE)/GYRO_MAX)*gyro.g.z*(WAIT/1000.0);
  while(angle_x<0) {angle_x+=360;}
  while(angle_x>=360) {angle_x-=360;}
  while(angle_z<0) {angle_z+=360;}
  while(angle_z>=360) {angle_z-=360;}
  float delta_x = 180-abs(180-angle_x);
  float delta_z = 180-abs(180-angle_z);
  uint8_t total = constrain(sqrt((delta_x*delta_x)+(delta_z*delta_z)),0,180);
  uint8_t totalmap = total*(NUM_COLORS-1)/180;
  
  Serial.println(total);
  boolean pixel_used;
  for (uint8_t i=0;i<NUM_LEDS;i++) {
    pixel_used = false;
    for (uint8_t j=0;j<6;j++) {
      if (i==bouncers[j]) {
        strip.setPixelColor(i,totalmap,0,NUM_COLORS-1-totalmap);
        pixel_used = true;
      }
    }
    if (pixel_used==false) {
      strip.setPixelColor(i,0,0,0);
    }
  }
}

void sheet_lightning() {
  uint8_t color = random(2);
  float rotation = sqrt(sq(gyro.g.x)+sq(gyro.g.z));
 int rate = max(0,map(rotation,0,1000,10,0));
  static uint16_t k = 0;
  if (rate==0) {
    k = 0;
  } else {
    k = (k+1)%rate;
  }
  if (k==0) {
    for(uint8_t i=0;i<NUM_LEDS;i++) {
      if(random(6)==1)
        color = 1-color;
        uint8_t r = random(50);
        if(r==1)
          strip.setPixelColor(i,NUM_COLORS-1,0,0);
        else if(r==2)
          strip.setPixelColor(i,0,NUM_COLORS-1,0);
        else if(color==0)
          strip.setPixelColor(i,NUM_COLORS-1,NUM_COLORS-1,0);
        else strip.setPixelColor(i,0,0,NUM_COLORS-1);
     } 
  } 
  else if (rotation>=2000) {
    for(uint8_t i=0;i<NUM_LEDS;i++) {
      strip.setPixelColor(i,0,0,0);
    }
  }
}

void collidoscope() {
  static uint8_t pos[] = {0,3,6,9,12,15};
  static int speeds[] = {1,1,1,1,1,1};
  float rotation = sqrt(sq(gyro.g.x)+sq(gyro.g.z));
  if (rotation > 2000) {
    for (uint8_t i = 0;i<6;i++) {
      if (pos[i]==18) {
        speeds[i]=-1;
      } else if (pos[i]==0) {
        speeds[i]=1;
      }
      pos[i]+=speeds[i];
    }
  }
  for(uint8_t i=0;i<NUM_LEDS;i++) {
    if(i==18+pos[0] || i==18-pos[0]) {
      strip.setPixelColor(i,NUM_LEDS-1,0,0);
    } else if(i==18+pos[1] || i==18-pos[1]) {
      strip.setPixelColor(i,NUM_LEDS-1,NUM_LEDS-1,0);
    } else if(i==18+pos[2] || i==18-pos[2]) {
      strip.setPixelColor(i,0,NUM_LEDS-1,0);
    } else if(i==18+pos[3] || i==18-pos[3]) {
      strip.setPixelColor(i,0,NUM_LEDS-1,NUM_LEDS-1);
    } else if(i==18+pos[4] || i==18-pos[4]) {
      strip.setPixelColor(i,0,0,NUM_LEDS-1);
    } else if(i==18+pos[5] || i==18-pos[5]) {
      strip.setPixelColor(i,NUM_LEDS-1,0,NUM_LEDS-1);
    } else {
      strip.setPixelColor(i,0,0,0);
    }
  } 
}

void helix_nebula() {
  //great as-is
  static uint8_t displace = 0;
  const uint8_t black = 3;
  float rotation = sqrt(sq(gyro.g.x)+sq(gyro.g.z));
  uint8_t rot_disp_map = map(rotation,0,4000,0,12);
  if(displace < rot_disp_map) {
    displace+=1;
  } else if (displace > rot_disp_map) {
    displace-=1;
  }
  uint8_t j;
  uint8_t fleck;
  for(uint8_t i=0;i<NUM_LEDS;i++) {
    if (random(NUM_LEDS) <= 1) {
      fleck = random(NUM_COLORS/4);
      strip.setPixelColor(i,fleck,fleck,fleck);
    } else {
      j = abs(i - (NUM_LEDS/2));
      if (j > displace+black) {
        strip.setPixelColor(i,gradient(j,displace+black,NUM_LEDS/2,0,0,0,0,NUM_COLORS-1,NUM_COLORS-1));
      } else if (j >= displace) {
        strip.setPixelColor(i,0,0,0);
      } else {
        strip.setPixelColor(i,gradient(j,displace,0,0,0,0,NUM_COLORS-1,0,0));
      } 
    }
  }
}

void lava_flow() {
  //great as-is
  static int edge = 0;
  const uint8_t red = 20;
  const uint8_t yellow = 5;
  const uint8_t dimness = 50;
  static int speed = 1;
  
  static boolean moving = false;
  float rotation = sqrt(sq(gyro.g.x)+sq(gyro.g.z));
  if (rotation > 2000) {
    if (moving == false) {
      speed = 0-speed; 
    }
    moving = true;
  } else {
    moving = false;
  }
  
  uint16_t r;
  if (edge>=NUM_LEDS+yellow+red+red+yellow) {
    edge = 0;
  } else if (edge<0) {
    edge = NUM_LEDS+yellow+red+red+yellow;
  }
  edge += speed;
  
  for (uint8_t i=0; i<NUM_LEDS;i++) {
    if (rotation>=2000) {
        r = random(NUM_LEDS*4);
    } else {
      r = 0;
    }
    switch(r) {
      case 1:
        strip.setPixelColor(i,0,0,NUM_COLORS-1);
        break;
      case 2:
        strip.setPixelColor(i,NUM_COLORS-1,0,NUM_COLORS-1);
        break;
      case 3:
        strip.setPixelColor(i,0,NUM_COLORS-1,0);
        break;
      case 4:
        strip.setPixelColor(i,0,0,0);
        break;
      default:
        if (i <= edge && i > edge-yellow) {
          strip.setPixelColor(i,gradient(i,edge,edge-yellow,0,0,0,NUM_COLORS-1,NUM_COLORS-1-dimness,0));
        } else if ((i <= edge-yellow) && (i > edge-yellow-red)) {
          strip.setPixelColor(i,gradient(i,edge-yellow,edge-yellow-red,NUM_COLORS-1,NUM_COLORS-1-dimness,0,NUM_COLORS-1,0,0));
        } else if ((i <= edge-yellow-red) && (i > edge-yellow-red-red)) {
          strip.setPixelColor(i,gradient(i,edge-yellow-red,edge-yellow-red-red,NUM_COLORS-1,0,0,NUM_COLORS-1,NUM_COLORS-1-dimness,0));
        } else if ((i <= edge-yellow-red-red) && (i > edge-yellow-red-red-yellow)) {
          strip.setPixelColor(i,gradient(i,edge-yellow-red-red,edge-yellow-red-red-yellow,NUM_COLORS-1,NUM_COLORS-1-dimness,0,0,0,0));
        } else {
          strip.setPixelColor(i,0,0,0);
        }
    }
  }
}


/******** Helper Functions *******/

uint32_t gradient(int pixel, int pixel1, int pixel2, uint16_t red1, uint16_t green1, uint16_t blue1, uint16_t red2, uint16_t green2, uint16_t blue2) {
  uint8_t red, green, blue;
  if (((pixel < pixel1) && (pixel1 < pixel2)) || ((pixel > pixel1) && (pixel1 > pixel2))) {
    red = red1;
    green = green1;
    blue = blue1;
  } else if (((pixel < pixel2) && (pixel1 > pixel2)) || ((pixel > pixel2) && (pixel1 < pixel2))) {
    red = red2;
    green = green2;
    blue = blue2;
  } else {
    red = powerdim(map(pixel, pixel1, pixel2, red1, red2));
    green = powerdim(map(pixel, pixel1, pixel2, green1, green2));
    blue = powerdim(map(pixel, pixel1, pixel2, blue1, blue2));
  }
  return strip.Color(red, green, blue);
}

uint8_t powerdim(uint8_t x) {
  return pow(x/float((NUM_COLORS-1)),1.5)*(NUM_COLORS-1);
}

int check_input() {
  static boolean cw = false;
  static boolean ccw = false;
  static long input_time = 0;
  #define ROTARY_THRESHOLD 2000
  #define ROTARY_DELAY 500
  int return_value = 0;
  float rotation = sqrt(sq(gyro.g.x)+sq(gyro.g.z));
  if ((gyro.g.y > ROTARY_THRESHOLD || gyro.g.y < -ROTARY_THRESHOLD) && (rotation < ROTARY_THRESHOLD)) {
     if (cw==false) {
       cw = true;
       ccw = false;
       input_time = millis();
     }
     if (millis()-input_time > ROTARY_DELAY) {
       cw = false;
       return_value = 1;
     }
  } else {
    if (cw==true || ccw==true)
    cw = false;
    ccw = false;
    return_value = 0;
  }
  return return_value;
}
