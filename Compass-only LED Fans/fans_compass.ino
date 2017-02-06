#include "LPD8806.h"
#include "SPI.h"
#include "EEPROM.h"
#include "Wire.h"
#include "LSM303.h"
#include "VirtualWire.h"

/**********************Setup*******************************/

const uint8_t NLEDS = 4;
const uint8_t NSTRIPS = 5;
uint8_t clock, pushbutton;
uint8_t datapins[5];
LPD8806 strips[5];
LSM303 compass;
int whichFan;
boolean useCompass = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  //tag which fan this is, must be set manually 
  //EEPROM.write(0,0); // (left fan)
  //EEPROM.write(0,1); // (right fan)
  whichFan = EEPROM.read(0);
  if (whichFan==0) { //Teensy (left fan)
    useCompass = true;
    compass.m_min.x = -789; compass.m_min.y = -419; compass.m_min.z = -578;
    compass.m_max.x = +283; compass.m_max.y = +562; compass.m_max.z = +380;
    //!!!Note: the order of pins on each fan is currently scrambled.
    datapins[0] = 16;
    datapins[1] = 18;
    datapins[2] = 13;
    datapins[3] = 14;
    datapins[4] = 15; 
    clock = 21;
    pushbutton = 17;
  } else if (whichFan==1) { // (right fan)
    useCompass = false;
    compass.m_min.x = -442; compass.m_min.y = -605; compass.m_min.z = -437;
    compass.m_max.x = +437; compass.m_max.y = +336; compass.m_max.z = +426;
    datapins[0] = 16;
    datapins[1] = 18;
    datapins[2] = 13;
    datapins[3] = 14;
    datapins[4] = 15; 
    clock = 21;
    pushbutton = 17;
  }
  for(uint8_t i=0; i<NSTRIPS; i++) {
    strips[i] = LPD8806(NLEDS,datapins[i],clock);
    strips[i].begin();
    strips[i].show();
  }
}

/*************************Main Loop**************************************/

void loop() {
  static long previousMillis = 0;
  const unsigned long delaytick = 10;
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > delaytick) {
    previousMillis = currentMillis;
    play_pattern(button_state());
  } 
}

int button_state() {
  static boolean prev_read = 0;
  if ((digitalRead(pushbutton)==1)&&(prev_read==0)) {
    prev_read = 1;
    return 1;
  } else if (digitalRead(pushbutton)==0) {
    prev_read = 0;
  }
  return 0; 
}

void play_pattern(int button_state) {
  const uint8_t NPATTERNS = 5;
  static uint8_t current_pattern = 0;
  if (button_state==1) {
    current_pattern=((current_pattern+1)%NPATTERNS);
  }
  switch (current_pattern) {
    case 1:
      rainbow_road();
    break;
    case 2:
      starry_night();
    break;
    case 3:
      strobe();
      break;
    case 4:
      color_rain();
      break;
    case 0:
    default:
      sleep();
  }
  for(uint8_t i=0; i<NSTRIPS; i++) {
    strips[i].show();
  }
  delay(10);
}

/*****************************************Patterns**************************/

void sleep() {
  for(uint8_t i = 0; i<NSTRIPS; i++) {
    for(uint8_t j = 0; j<NLEDS; j++) {
      strips[i].setPixelColor(j,0,0,0);
    }
  }
}


void wanderers() {
  const int NDOTS = 5;
  static int wander_strip[NDOTS] = {0,1,2,3,4};
  static int wander_light[NDOTS] = {0,2,3,1,2};
  static int wander_dir[NDOTS] = {1,-1,1,-1,1};
  uint32_t wander_color[NDOTS] = {
                              strips[0].Color(127,127,127),
                              strips[0].Color(127,0,0),
                              strips[0].Color(127,127,0),
                              strips[0].Color(0,127,0),
                              strips[0].Color(0,0,127)};
  
  for(uint8_t i = 0; i<NSTRIPS; i++) {
    for(uint8_t j = 0; j<NLEDS; j++) {
      strips[i].setPixelColor(j,0,0,0);
    }
  }
  for(uint8_t i = 0; i<NDOTS; i++) {
    wander_light[i]+=wander_dir[i];
    if((wander_light[i]<0)||(wander_light[i]>=4)) {
      wander_strip[i] = random(5);
      if (random(2)==0) {
        wander_light[i] = -1;
        wander_dir[i] = 1;
      } else {
        wander_light[i] = 4;
        wander_dir[i] = -1;
      }
    }
    strips[wander_strip[i]].setPixelColor(wander_light[i],wander_color[i]);
  }
}

void fire_and_ice() {
  compass.read();
  int heading = compass.heading((LSM303::vector){0,-1,0});
  int mapped = map(heading,0,383,0,359);
  uint32_t color1, color2, color3, color4;
  if (whichFan==0) {
    color1 = strips[0].Color(50,50,0);
    color2 = strips[0].Color(127,127,0);
    color3 = strips[0].Color(127,50,0);
    color4 = strips[0].Color(127,0,0);
  } else if (whichFan==1) {
    color1 = strips[0].Color(0,50,50);
    color2 = strips[0].Color(0,127,127);
    color3 = strips[0].Color(0,50,127);
    color4 = strips[0].Color(0,0,127);
  }
  static uint8_t flames[NSTRIPS] = {0,0,0,0,0};
  static int flame_point = -3; 
  static int dir = 1;
  flame_point = flame_point + dir;
  if ((flame_point==8)||(flame_point==-3)) {
    dir = 0-dir;
  }
  for(uint8_t i = 0; i<NSTRIPS; i++) {
    if(i==flame_point) {
      flames[i]=4;
    } else if(abs(flame_point-i)==1) {
      flames[i]=3;
    } else if(abs(flame_point-i)==2) {
      flames[i]=2;
    } else if(abs(flame_point-i)==3) {
      flames[i]=1;
    } else {
      flames[i]=0;
    }
    for(uint8_t j = 0; j<NLEDS; j++) {
      if(j==flames[i]-1) {
        strips[i].setPixelColor(j,(rainbow(mapped)));
      } else if(j==flames[i]-2) {
        strips[i].setPixelColor(j,(rainbow(mapped)));
      } else if(j==flames[i]-3) {
        strips[i].setPixelColor(j,(rainbow(mapped)));
      } else if(j==flames[i]-4) {
        strips[i].setPixelColor(j,(rainbow(mapped)));
      } else {
        strips[i].setPixelColor(j,0,0,0);
      }
    }
  }  
}


void color_rain() {
  compass.read();
  int heading = compass.heading((LSM303::vector){0,-1,0});
  int mapped = map(heading,0,383,0,359);
  static int drops[NSTRIPS] = {1,-1,3,7,5};
  static uint32_t colors[NSTRIPS] = {10,0,20,40,30};
  static int p = 0;
  static int k = 0;
  k=(k+1)%2;
  p=(p+1)%384;
  if (k==0) {
    for(uint8_t i = 0; i<NSTRIPS; i++) {
      if (drops[i]==8) {
        drops[i] = -1;
        colors[i]=(colors[i]+50)%384;
      } else {
        drops[i]+=1;
      } 
      for(uint8_t j = 0; j<NLEDS; j++) {
        if(((drops[i]>=0))&&(j<=drops[i])&&(j>=(drops[i]-5))) {
          strips[i].setPixelColor(j,(rainbow(mapped)));
        } else {
          strips[i].setPixelColor(j,0,0,0);
        }
      }
    }
  }  
}

void starry_night() {
  compass.read();
  int heading = compass.heading((LSM303::vector){0,-1,0});
  int mapped = map(heading,0,383,0,359);
  int blinkrate = 4;
  for(uint8_t i = 0; i<NSTRIPS; i++) {
    for(uint8_t j = 0; j<NLEDS; j++) {
      if (random(blinkrate)==0) {
        strips[i].setPixelColor(j,(rainbow(mapped)));
      } else {
        strips[i].setPixelColor(j,0,0,0);
      }
    }
  } 
}


void strobe() {
  compass.read();
  int heading = compass.heading((LSM303::vector){0,-1,0});
  int mapped = map(heading,0,383,0,359);
  static uint8_t p, c = 0;
  p = (p+1)%2;
  if (p==1) {
    c = (c+1)%6;
  }
  uint32_t color;
  switch (c) {
    case 0:
      color = strips[0].Color(127,0,0);
      break;
    case 1:
      color = strips[0].Color(127,127,0);
      break;
    case 2:
      color = strips[0].Color(0,127,0);
      break;
    case 3:
      color = strips[0].Color(0,127,127);
      break;
    case 4:
      color = strips[0].Color(0,0,127);
      break;
    case 5:
      color = strips[0].Color(127,0,127);
      break;
  }
  for(uint8_t i = 0; i<NSTRIPS; i++) {
    for(uint8_t j = 0; j<NLEDS; j++) {
      if (p==0) {
        strips[i].setPixelColor(j,(rainbow(mapped)));
      } else {
        strips[i].setPixelColor(j,0,0,0);
      }
    }
  } 
}

void rainbow_road() {
  compass.read();
  int heading = compass.heading((LSM303::vector){0,-1,0});
  int mapped = map(heading,0,359,0,383);
  for(uint8_t i = 0; i<NSTRIPS; i++) {
    for(uint8_t j = 0; j<NLEDS; j++) {
      strips[i].setPixelColor(j,(rainbow(mapped)));
    }
  }
}

void snakes() {
  static int p = 0;
  static int k = 0;
  k=(k+1)%4;
  if (k==0) {
    p = (p+1)%6;
  }
  uint32_t bandColor;
  uint32_t stripeColor;
  if(whichFan==1) {
    bandColor = strips[0].Color(127,127,0);
    stripeColor = strips[0].Color(127,0,0);
  } else if (whichFan==0) {
    bandColor = strips[0].Color(127,127,0);
    stripeColor = strips[0].Color(0,0,127);
  }
  uint32_t color;
  int c;
  for(uint8_t i = 0; i < 20; i++) {
    c = (i+p)%6;
    if((c>=2)&&(c<=3)) {
     color = stripeColor;
    } else if((c==1)||(c==4)) {
     color = bandColor;
    } else {
      color = strips[0].Color(0,0,0);
    }
    if((i>=0)&&(i<4)) {
      strips[0].setPixelColor(i%4,color);
    } else if((i>=4)&&(i<8)) {
      strips[1].setPixelColor(4-(i%4),color);
    } else if((i>=8)&&(i<12)) {
      strips[2].setPixelColor(i%4,color);
    } else if((i>=12)&&(i<16)) {
      strips[3].setPixelColor(4-(i%4),color);
    } else if((i>=16)&&(i<20)) {
      strips[4].setPixelColor(i%4,color);
    }
  }
}

/*****************Helper functions***************************/

uint32_t rainbow(uint16_t pos)
{
  //0-384
  byte r, g, b;
  switch(pos / 128)
  {
    case 0:
      r = 127 - pos % 128;   //Red down
      g = pos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - pos % 128;  //green down
      b = pos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - pos % 128;  //blue down 
      r = pos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strips[0].Color(r,g,b));
}
