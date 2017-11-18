#include <SoftwareSerial.h>
#include <Servo.h>

//********REMOTE CONTORL TRANSMISSION CODES
#define THROTTLE_UP_1 0x11
#define THROTTLE_UP_2 0x13
#define THROTTLE_DWN_1 0x14
#define THROTTLE_DWN_2 0x1C
#define THROTTLE_NEUTRAL 0x10

#define YAW_LEFT_1 0x34
#define YAW_LEFT_2 0x3C
#define YAW_RIGHT_1 0x31
#define YAW_RIGHT_2 0x33
#define YAW_NEUTRAL 0x30

#define PITCH_FWD_1 0x41
#define PITCH_FWD_2 0x43
#define PITCH_BCK_1 0x44
#define PITCH_BCK_2 0x4C
#define PITCH_NEUTRAL 0x40

#define ROLL_LEFT_1 0xC1
#define ROLL_LEFT_2 0xC2
#define ROLL_RIGHT_1 0xC4
#define ROLL_RIGHT_2 0xCC
#define ROLL_NEUTRAL 0xC0

#define ERROR_HALT 0xFF

//********MOTOR_PIN DEFINITIONS********
//1 is front CCW
//2 is back CW
//3 is back CCW
//4 is front CW
#define FRONT_RIGHT_CCW 5
#define BACK_RIGHT_CW 6
#define BACK_LEFT_CCW 9
#define FRONT_LEFT_CW 10

//*********BLUETOOTH PIN DEFINITION*****
#define BT_RCV 11
#define BT_TX 12

//********IMU I2C CONNECTION PINS*******

//*********GLOBAL VARIABLES**************
SoftwareSerial BTSerial(11,12); //Bluetooth connection variable
//*******FORWARD DECLARATIONS***********
void BT_Recv_Controller_Data();
void setup() {
 // put your setup code here, to run once: 
  Serial.begin(9600); //for testing purposes only
  BTSerial.begin(38400);  //initializing the bluetooth connection
                          //at 38400 BAUD rate. already paired to controller
  
}

void loop() {
  // put your main code here, to run repeatedly:
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
        break;
        case THROTTLE_UP_2:
        Serial.println("THROTTLE_UP_2");
        break;
        case THROTTLE_DWN_1:
        Serial.println("THROTTLE_DWN_1");
        break;
        case THROTTLE_DWN_2:
        Serial.println("THROTTLE_DWN_2");
        break;
        case THROTTLE_NEUTRAL:
        Serial.println("THROTTLE_NEUTRAL");
        break;
        case PITCH_FWD_1:
        Serial.println("PITCH_FWD_1");
        break;
        case PITCH_FWD_2:
        Serial.println("PITCH_FWD_2");
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

