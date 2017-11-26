//*********TESTING ENV VARIABLES*********
//COMMENT AND UNCOMMENT AS DESIRED
//#define TEST_MOTORS 
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

//********IMU CONFIGURATION*******
#define YAW 0
#define PITCH 2
#define ROLL 1

//CONFIG DECLARATIONS
#define TEST_IMU
