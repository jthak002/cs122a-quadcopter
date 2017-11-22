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
int throttlePin=0;
int pos=0;
//*******FUNCTION DECLARATIONS***********
void BT_Recv_Controller_Data();


//*******MAIN ARDUINO PROGRAM******
void setup() {
 // put your setup code here, to run once: 
  Serial.begin(9600);
  BTSerial.begin(38400);
  motor_fccw.attach(5);
  pinMode(13,OUTPUT);
  motor_bcw.attach(6);
  motor_bccw.attach(9);
  motor_bcw.attach(10);
  for(int i=0;i<20;i++)
  {
    if(i>30)
      setSpeed(20);
    else
      setSpeed(i);
    delay(1000);
  }
}

void setSpeed(int speed){
  int angle=map(speed,0,100,0,180);
  motor_fccw.write(angle);
  motor_bcw.write(angle);
}
void loop() {
  // put your main code here, to run repeatedly:
  if(pos==0)
    BT_Recv_Controller_Data(); 
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
