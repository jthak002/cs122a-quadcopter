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
#define CONNECTION_STATE 8
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
//DUE TO THE WRONG PLACEMENT OF THE IMU ON MY QUAD COPTER  
#define YAW 0
#define PITCH 2
#define ROLL 1
//********THROTTLE SPEEDS*******
#define THROTTLE_MAX 70
#define THROTTLE_MIN 38
#define THROTTLE_SPEED__1 44
#define THROTTLE_SPEED__2 43
#define THROTTLE_HOVER 46
#define THROTTLE_SPEED_1 49
#define THROTTLE_SPEED_2 52
//CONFIG DECLARATIONS
//UN-COMMENT THE TESTS THAT YOU WANT TO RUN
//#define TEST_IMU
//#define TEST_CONTROLLER
#define TEST_PID
