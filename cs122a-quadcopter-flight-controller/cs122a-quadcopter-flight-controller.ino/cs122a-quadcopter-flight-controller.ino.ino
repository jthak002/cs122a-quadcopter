#include <SoftwareSerial.h>
#include <Servo.h>
#include "config.h"
/*******ROTOR TERMINOLOGY*********
//(1)fccw- FRONT COUNTER CLOCKWISE (DIGITAL PIN 5)
//(4)fcw- FRONT CLOCKWISE (DIGITAL PIN 6)
//(3)bcw- BACK CLOCKWISE (DIGITAL PIN 9)
//(2)bccw- BACK COUNTER CLOCKWISE (DIGITAL PIN 10)
 (4)      (1)
   \      /
    \    /
     QUAD
     /   \
    /     \
  (3)     (2)
*/
//*********GLOBAL VARIABLES**************
SoftwareSerial BTSerial(11,12); //Bluetooth connection variable
Servo motor_fccw;       
Servo motor_bcw;
Servo motor_bccw;
Servo motor_fcw;
int motor_fccw_speed=0;  //FRONT CCW MOTOR(1) SPEED
int motor_bcw_speed=0;   //BACK CLOCKWISE MOTOR(2) SPEED
int motor_bccw_speed=0;  //BACK COUNTER CLOCKWISE MOTOR(3) SPEED
int motor_fcw_speed=0;   //FRONT CLOCKWISE MOTOR(4) SPEED
int motor_curr_speed=0;  //THROTTLE LEVEL(_1,_2,_HOVER,__1,__2)

float ypr[3]; //current yaw/pitch/role obtained out of the MPU
float i_ypr[3]; //initial yaw/pitch/roll on a flat surface--CALIBRATION

bool arm=false; //Mtotr arming not checked until 
//*******FUNCTION DECLARATIONS***********
void mpu_Calibrate();
void motor_Calibrate();
void mpu_acquisition();
int BT_Recv_Controller_Data();
//PID Functions
void throttle_pid_controller();
/*
void yaw_pid_controller();
void pitch_pid_controller();
void roll_pid_controller();*/
//*******MAIN ARDUINO PROGRAM******
void setup() {
 // put your setup code here, to run once: 
  //initializing Arduino Serial Monitor
  Serial.begin(9600);
  //Initializing Bluetooth Module
  BTSerial.begin(38400);
  Serial.println(F("BEGINNING CALIBRATION..."));
  mpu_Calibrate();  //calibrating 
  motor_Calibrate();
  pinMode(13,OUTPUT);
  delay(5000);
  mpu_acquisition();
  i_ypr[YAW]=ypr[YAW];
  i_ypr[PITCH]=ypr[PITCH];
  i_ypr[ROLL]=ypr[ROLL];
  Serial.println(F("CALIBRATION COMPLETE."));
}

void loop() {
  digitalWrite(13,HIGH);
  mpu_acquisition();
  int bt_value=BT_Recv_Controller_Data();
  if(bt_value!=-1 && bt_value!=ERROR_HALT && motor_curr_speed!=0)//ERROR CODES FOR LOSS OF CONNECTIVITY AND EMERGENCY SHUTDOWN 
  {
    //CONTROLLER SHORT CODES: THE LAST 4 BITS DENOTE THE
    //CONTROL BEING USED: 0x1 is THROTTLE, 0x3 is YAW
    //                    0x4 is PITCH, 0xC is ROLL 
    switch((bt_value&0xF0)>>4)
    {
      case 0x1://THROTTLE
        throttle_pid_controller();
        break;
      case 0x3://YAW
        //yaw_pid_controller();
        break;
      case 0x4://ROLL
        //pitch_pid_controller();
        break;
      case 0xC:
        //roll_pid_controller();
        break;
    }
  }
}
