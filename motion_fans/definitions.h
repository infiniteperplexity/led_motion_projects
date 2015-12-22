#include <Arduino.h>
#include "Wire.h"
#include "L3G.h"
#include "LSM303.h" //causes mysterious error
#include "SPI.h" // Comment out this line if using Trinket or Gemma

#define GRAVITY 256  //this equivalent to 1G in the raw data coming from the accelerometer 
#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi
// L3G4200D gyro: 2000 dps full scale
// 70 mdps/digit; 1 dps = 0.07
#define Gyro_Gain_X 0.07 //X axis Gyro gain
#define Gyro_Gain_Y 0.07 //Y axis Gyro gain
#define Gyro_Gain_Z 0.07 //Z axis Gyro gain
#define Gyro_Scaled_X(x) ((x)*ToRad(Gyro_Gain_X)) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Y(x) ((x)*ToRad(Gyro_Gain_Y)) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Z(x) ((x)*ToRad(Gyro_Gain_Z)) //Return the scaled ADC raw data of the gyro in radians for second
// LSM303 magnetometer calibration constants; use the Calibrate example from
// the Pololu LSM303 library to find the right values for your board
#define M_X_MIN -5548
#define M_Y_MIN -183
#define M_Z_MIN -4880
#define M_X_MAX 186
#define M_Y_MAX 5640
#define M_Z_MAX 849
#define Kp_ROLLPITCH 0.02
#define Ki_ROLLPITCH 0.00002
#define Kp_YAW 1.2
#define Ki_YAW 0.00002
#define OUTPUTMODE 1
extern float pitch;
extern float roll;
extern float yaw;
extern int SENSOR_SIGN[9];
extern float G_Dt;
extern int AN[6];
extern int AN_OFFSET[6];
extern int gyro_x;
extern int gyro_y;
extern int gyro_z;
extern int accel_x;
extern int accel_y;
extern int accel_z;
extern int magnetom_x;
extern int magnetom_y;
extern int magnetom_z;
extern float c_magnetom_x;
extern float c_magnetom_y;
extern float c_magnetom_z;
extern float MAG_Heading;
extern float Accel_Vector[3];
extern float Gyro_Vector[3];
extern float Omega_Vector[3];
extern float Omega_P[3];
extern float Omega_I[3];
extern float Omega[3];
extern float errorRollPitch[3];
extern float errorYaw[3];
extern unsigned int counter;
extern byte gyro_sat;
extern float DCM_Matrix[3][3]; 
extern float Update_Matrix[3][3];
extern float Temporary_Matrix[3][3];
L3G gyro;
LSM303 compass;
float Vector_Dot_Product(float vector1[3],float vector2[3]);
void Vector_Cross_Product(float vectorOut[3], float v1[3],float v2[3]);
void Vector_Scale(float vectorOut[3],float vectorIn[3], float scale2);
void Vector_Add(float vectorOut[3],float vectorIn1[3], float vectorIn2[3]);
void Matrix_Multiply(float a[3][3], float b[3][3],float mat[3][3]);
void Compass_Heading();
void I2C_Init();
void Gyro_Init();
void Read_Gyro();
void Accel_Init();
void Read_Accel();
void Compass_Init();
void Callibrate();
void Normalize(void);
void AHRS_Init();
void Read_Compass();
void Drift_correction(void);
void Matrix_update(void);
void Euler_angles(void);
void AHRS_Update();

float upitch;
float uroll;
float uyaw;
float shake;
extern float last_acc[3];
extern float buff_pitch ;
extern float buff_yaw;
extern float buff_roll;

