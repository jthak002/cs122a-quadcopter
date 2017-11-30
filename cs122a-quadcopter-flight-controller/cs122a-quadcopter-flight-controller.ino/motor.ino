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
  int throttle = 180;  //Set the throttle to maximum
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

void setSpeed(int speed){
  motor_fccw_speed=speed;
  motor_fcw_speed=speed;
  motor_bccw_speed=speed;
  motor_bcw_speed=speed;
  int angle=map(speed,0,100,0,180);
  motor_fccw.write(angle);
  motor_bcw.write(angle);
  motor_bccw.write(angle);
  motor_fcw.write(angle);
}
//********INDIVIDUAL SPEED SETTING FUNCTIONS********
void setSpeed_fccw(int speed)
{
  motor_fccw_speed=speed;
  int angle=map(speed,0,100,0,180);
  motor_fccw.write(angle); 
}
void setSpeed_bcw(int speed)
{
  motor_bcw_speed=speed;
  int angle=map(speed,0,100,0,180);
  motor_bcw.write(angle); 
}
void setSpeed_bccw(int speed)
{
  motor_bccw_speed=speed;
  int angle=map(speed,0,100,0,180);
  motor_bccw.write(angle); 
}
void setSpeed_fcw(int speed)
{
  
  int angle=map(speed,0,100,0,180);
  motor_fcw.write(angle); 
}
