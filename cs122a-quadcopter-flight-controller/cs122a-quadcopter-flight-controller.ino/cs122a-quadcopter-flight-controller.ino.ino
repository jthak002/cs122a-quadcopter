#include <SoftwareSerial.h>
#include <Servo.h>
#include "config.h"
//*********GLOBAL VARIABLES**************
SoftwareSerial BTSerial(11,12); //Bluetooth connection variable
Servo motor_fccw;
Servo motor_bcw;
Servo motor_bccw;
Servo motor_fcw;

int throttle=0;
int pos=0;
//*******FUNCTION DECLARATIONS***********
void BT_Recv_Controller_Data();
void mpu_Calibrate();
void mpu_acquisition();
void motor_Calibrate();

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
  Serial.println(F("CALIBRATION COMPLETE."));
}

void setSpeed(int speed){
  int angle=map(speed,0,100,0,180);
  motor_fccw.write(angle);
  motor_bcw.write(angle);
  motor_bccw.write(angle);
  motor_fcw.write(angle);
}

void loop() {
  digitalWrite(13,HIGH);
  mpu_acquisition();
}

void BT_Recv_Controller_Data(){
  unsigned char controller_value=0;
  if(BTSerial.available())
  {
    controller_value=BTSerial.read();
    switch(controller_value){
      case THROTTLE_UP_1:
        Serial.println("THROTTLE_UP_1");
        setSpeed(33);
        break;
        case THROTTLE_UP_2:
        Serial.println("THROTTLE_UP_2");
        setSpeed(35);
        break;
        case THROTTLE_DWN_1:
        Serial.println("THROTTLE_DWN_1");
        setSpeed(28);
        break;
        case THROTTLE_DWN_2:
        Serial.println("THROTTLE_DWN_2");
        setSpeed(25);
        break;
        case THROTTLE_NEUTRAL:
        Serial.println("THROTTLE_NEUTRAL");
        setSpeed(30);
        break;
        case PITCH_FWD_1:
        Serial.println("PITCH_FWD_1");
        break;
        case PITCH_FWD_2:
        Serial.println("PITCH_FWD_2");
        setSpeed(0);
        pos=1;
        break;
        case PITCH_NEUTRAL:
        Serial.println("PITCH_NEUTRAL");
        break;
        case PITCH_BCK_1:
        Serial.println("PITCH_BCK_1");
        break;
        case PITCH_BCK_2:
        Serial.println("PITCH_BCK_2");
        break;
        case ROLL_LEFT_2:
        Serial.println("ROLL_LEFT_2");
        break;
        case ROLL_LEFT_1:
        Serial.println("ROLL_LEFT_1");
        break;
        case ROLL_NEUTRAL:
        Serial.println("ROLL_NEUTRAL");
        break;
        case ROLL_RIGHT_1:
        Serial.println("ROLL_RIGHT_1");
        break;
        case ROLL_RIGHT_2:
        Serial.println("ROLL_RIGHT_2");
        break;
        case YAW_LEFT_2:
        Serial.println("YAW_LEFT_2");
        break;
        case YAW_LEFT_1:
        Serial.println("YAW_LEFT_1");
        break;
        case YAW_NEUTRAL:
        Serial.println("YAW_NEUTRAL");
        break;
        case YAW_RIGHT_1:
        Serial.println("YAW_RIGHT_1");
        break;
        case YAW_RIGHT_2:
        Serial.println("YAW_RIGHT_2");
        break;
        case ERROR_HALT:
        Serial.println("ERROR_HALT");
        break;
        default: break;
    }
  } 
}
void motor_Calibrate()
{
  motor_fccw.attach(FRONT_RIGHT_CCW,600,2250); 
  motor_bcw.attach(BACK_RIGHT_CW,600,2250);
  motor_bccw.attach(BACK_LEFT_CCW,600,2250);
  motor_fcw.attach(FRONT_LEFT_CW,600,2250);
  //attach the ESC to pin 5, 6, 9 and 10
  //Due to problems with the ESC recognising the maximum
  //position at the default settings, the figures after
  //the pin number are the microsecond signals for the 
  //minimum and maximum that the ESC will recognise.
  // 600 and 2250 work.
  throttle = 180;  //Set the throttle to maximum
  motor_fccw.write(throttle); 
  motor_bcw.write(throttle);
  motor_bccw.write(throttle);
  motor_fcw.write(throttle);
  //Set the ESC signal to maximum
  //At this point the ESC's power should be connected.
  delay(10000);  //Allow the user time to connect the battery to
  //the ESC.  
  throttle = 0;  //Set the throttle to zero
  motor_fccw.write(throttle);
  motor_bcw.write(throttle);
  motor_bccw.write(throttle);
  motor_fcw.write(throttle);  
  //Set the ESC signal to the zero position.
  delay(10000);  // allow a 10 second delay for the ESC to signal
  //that it is done. 
}

