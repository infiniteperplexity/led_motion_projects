old lurch detection code in case I need it
/*

    ////alternate lurch
    float lurchdelay = 0.1;
    static float lurchtimer = 0;
    //exponentially weighted moving average
    static float ewmavx = vx;
    static float ewmavy = vy;
    static float ewmavz = vz;
    float alpha = 0.05;
    float alphax = alpha;
    float alphay = alpha;
    float alphaz = alpha;
    ewmavx = (1-alphax)*ewmavx + alphax*vx;
    ewmavy = (1-alphay)*ewmavy + alphay*vy;
    ewmavz = (1-alphaz)*ewmavz + alphaz*vz;
    //dot product
    float ewmalength= sqrt(ewmavx*ewmavx+ewmavy*ewmavy+ewmavz*ewmavz);
    float dotproduct = (vx*ewmavx + vy*ewmavy +vz*ewmavz)/ewmalength;
    float ewmathresh = 0.02;
    // test for the overcorrective "lurch"
    if (dotproduct<0 && ewmalength>ewmathresh) {
      Serial.println(ewmalength, DEC);
      lurchtimer = lurchdelay;
    } 
    //kill velocity and acceleration after lurch
    /*float vthresh = 1;
    static int vxdir = 0;
    static int vydir = 0;
    static int vzdir = 0;

    if (vy>0 && abs(vy)>abs(vx) && abs(vy)>abs(vz)) {
      if (vydir == -1) {
        vydir = 0;
        lurchtimer = lurchdelay;
      } else {
        vydir = 1;
      }
    }
    if (vy<0 && abs(vy)>abs(vx) && abs(vy)>abs(vz)) {
      if (vydir == 1) {
        vydir = 0;
        lurchtimer = lurchdelay;
      } else {
        vydir = -1;
      }
    }
    if (vx>0 && abs(vx)>abs(vy) && abs(vx)>abs(vz)) {
      if (vxdir == -1) {
        vxdir = 0;
        lurchtimer = lurchdelay;
      } else {
        vxdir = 0;
      }
    }
    if (vx<0 && abs(vx)>abs(vy) && abs(vx)>abs(vz)) {
      if (vxdir == 1) {
        vxdir = 0;
        lurchtimer = lurchdelay;
      } else {
        vxdir = -1;
      }
    }
    if (vz>0 && abs(vz)>abs(vy) && abs(vz)>abs(vx)) {
      if (vzdir == -1) {
        vzdir = 0;
        lurchtimer = lurchdelay;
      } else {
        vzdir = 0;
      }
    }
    if (vz<0 && abs(vz)>abs(vy) && abs(vz)>abs(vx)) {
      if (vzdir == 1) {
        vzdir = 0;
        lurchtimer = lurchdelay;
      } else {
        vzdir = -1;
      }
    } */
    
    */
