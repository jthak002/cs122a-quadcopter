#define PITCH_KP 1
#define PITCH_KI 0
#define PITCH_KD 0

#define ROLL_KP 1
#define ROLL_KI 1
#define ROLL_KD 1
void throttle_pid_controller()
{
  //Variables to store the initial values of the motors speeds
  int imotor_fccw_speed=motor_fccw_speed;
  int imotor_bcw_speed=motor_bcw_speed;
  int imotor_bccw_speed=motor_bccw_speed;
  int imotor_fcw_speed=motor_fcw_speed;
  //flag values for pid correction
  bool pitch_pid_f=false;
  bool pitch_pid_b=false;
  bool roll_pid_l=false;
  bool roll_pid_r=false;
  //PID Error Variable
  float err=(ypr[PITCH]-i_ypr[PITCH])*180/M_PI;
  //Pitch backward condition. Need to apply MORE power to BACK ROTORS
  //OR LESS Power to the FRONT ROTORS
  //ERROR is less than 0. multiply by -1 to add to actuator speed
  float actuator_speed=0;
  if(err>0) 
  {
    actuator_speed= PITCH_KP*(err);
    if(motor_fccw_speed==THROTTLE_MAX && motor_fcw_speed==THROTTLE_MAX) //PREVENT FRONT MOTOR DEADLOCKS AT THROTTLE_MAX
    {
      if(motor_fccw_speed-actuator_speed>THROTTLE_MIN)
        motor_fccw_speed=THROTTLE_MIN;
      else
        motor_fccw_speed-=actuator_speed;
      if(motor_fcw_speed-actuator_speed>THROTTLE_MIN)
        motor_fcw_speed=THROTTLE_MIN;
      else
        motor_fcw_speed-=actuator_speed;
      pitch_pid_f=true;
    }
    else if(motor_bccw_speed<=motor_fccw_speed && motor_bcw_speed<=motor_fcw_speed)
    {
      if(motor_bcw_speed+actuator_speed>=THROTTLE_MAX)
        motor_bcw_speed=THROTTLE_MAX;
      else
        motor_bcw_speed+=actuator_speed;
      if(motor_bccw_speed+actuator_speed>=THROTTLE_MAX)
        motor_bccw_speed=THROTTLE_MAX;
      else
        motor_bccw_speed+=actuator_speed;
      pitch_pid_b=true;
    }
    else if(motor_fccw_speed>motor_bccw_speed && motor_fcw_speed>motor_fccw_speed)
    {
      if(motor_fcw_speed-actuator_speed<=THROTTLE_MIN)
        motor_fcw_speed=THROTTLE_MIN;
      else
        motor_fcw_speed-=actuator_speed;
      if(motor_fccw_speed-actuator_speed<=THROTTLE_MIN)
        motor_fccw_speed=THROTTLE_MIN;
      else
        motor_fcw_speed-=actuator_speed;
      pitch_pid_f=true;
    }
    else if(motor_bccw_speed==THROTTLE_MIN && motor_bcw_speed==THROTTLE_MIN)// PREVENT BACK MOTOR DEADLOCK AT THROTTLE_MIN
    {
      if(motor_bccw_speed+actuator_speed>=THROTTLE_MAX)
        motor_bccw_speed=THROTTLE_MAX;
      else 
        motor_bccw_speed+=actuator_speed;
      if(motor_bcw_speed+actuator_speed>=THROTTLE_MAX)
        motor_bcw_speed=THROTTLE_MAX;
      else 
        motor_bcw_speed+=actuator_speed;
      pitch_pid_b=true;
    }
    else //UN-COMMON CASE: NEXT ROUND WILL BRING THE MOTORS INTO THE OTHER TWO CASES
    {
      if(motor_bcw_speed+actuator_speed>=THROTTLE_MAX)
        motor_bcw_speed=THROTTLE_MAX;
      else
        motor_bcw_speed+=actuator_speed;
      if(motor_bccw_speed+actuator_speed>=THROTTLE_MAX)
        motor_bccw_speed=THROTTLE_MAX;
      else
        motor_bccw_speed+=actuator_speed;
      pitch_pid_b=true;
    }    
  }
  //PITCH FORWARD CONDITION
  //MORE power to front motors or 
  else
  {
    actuator_speed=PITCH_KP*(err);
    if(motor_fccw_speed==THROTTLE_MIN && motor_fcw_speed==THROTTLE_MIN)// PREVENT FRONT MOTOR DEADLOCK AT THROTTLE_MIN
    {
      if(motor_fccw_speed+actuator_speed>=THROTTLE_MAX)
        motor_fccw_speed=THROTTLE_MAX;
      else 
        motor_fccw_speed+=actuator_speed;
      if(motor_fcw_speed+actuator_speed>=THROTTLE_MAX)
        motor_fcw_speed=THROTTLE_MAX;
      else 
        motor_fcw_speed+=actuator_speed;
      pitch_pid_f=true;
    }
    else if(motor_fcw_speed<=motor_bcw_speed && motor_fccw_speed<=motor_bccw_speed)
    {
      if(motor_fcw_speed-actuator_speed>=THROTTLE_MAX)
        motor_fcw_speed=THROTTLE_MAX;
      else
        motor_fcw_speed-=actuator_speed;
      if(motor_fccw_speed-actuator_speed>=THROTTLE_MAX)
        motor_fccw_speed=THROTTLE_MAX;
      else
        motor_fccw_speed-=actuator_speed;
      pitch_pid_f=true;
    }
    else if(motor_bccw_speed==THROTTLE_MAX && motor_bcw_speed==THROTTLE_MAX) //PREVENT BACK MOTOR DEADLOCKS AT THROTTLE_MAX
    {
      if(motor_bccw_speed-actuator_speed>THROTTLE_MIN)
        motor_bccw_speed=THROTTLE_MIN;
      else
        motor_bccw_speed-=actuator_speed;
      if(motor_bcw_speed-actuator_speed>THROTTLE_MIN)
        motor_bcw_speed=THROTTLE_MIN;
      else
        motor_bcw_speed-=actuator_speed;
      pitch_pid_b=true;
    } 
    else if(motor_fcw_speed>motor_bcw_speed && motor_fccw_speed>motor_bccw_speed)
    {
      //since actuator speed is negative 
      if(motor_bcw_speed+actuator_speed< THROTTLE_MIN)
        motor_bcw_speed=THROTTLE_MIN;
      else
        motor_bcw_speed+=actuator_speed;
      if(motor_bccw_speed+actuator_speed< THROTTLE_MIN)
        motor_bccw_speed=THROTTLE_MIN;
      else
        motor_bccw_speed+=actuator_speed;
      pitch_pid_b=true;
    }
    else
    {
      if(motor_fcw_speed-actuator_speed>=THROTTLE_MAX)
        motor_fcw_speed=THROTTLE_MAX;
      else
        motor_fcw_speed-=actuator_speed;
      if(motor_fccw_speed-actuator_speed>=THROTTLE_MAX)
        motor_fccw_speed=THROTTLE_MAX;
      else
        motor_fccw_speed-=actuator_speed;
      pitch_pid_f=true;      
    }
  }
  //Normal no pitch abnormality
  if(err==0)
  {
    pitch_pid_f=false;
    pitch_pid_b=false;
  }
  //**********PITCH PID ENDS HERE****************
  //**************ROLL PID****************
  err=(ypr[ROLL]-i_ypr[ROLL])*180/M_PI;
  if(err<0) //ROLL RIGHT
  {
    actuator_speed=ROLL_KP*err;
    if(imotor_bccw_speed==THROTTLE_MAX && imotor_fcw_speed==THROTTLE_MAX)           //PREVENT LEFT MOTORS FROM BEING STUCK AT MAX
    {
      if(imotor_bccw_speed+actuator_speed<THROTTLE_MIN)
        motor_bccw_speed=getAvg(motor_bccw_speed,THROTTLE_MIN);
      else
        motor_bccw_speed=getAvg(motor_bccw_speed,imotor_bccw_speed+actuator_speed);
      if(imotor_fcw_speed+actuator_speed<THROTTLE_MIN)
        motor_fcw_speed=getAvg(motor_fcw_speed,THROTTLE_MIN);
      else
        motor_fcw_speed=getAvg(motor_fcw_speed,imotor_fcw_speed+actuator_speed);
    }
    else if(imotor_fccw_speed+imotor_bcw_speed < imotor_fcw_speed+imotor_bccw_speed)  //THE SPEED OF THE RIGHT MOTORS < SPEED OF THE LEFT MOTORS, REDUCE LEFT SPEED
    {
      if(imotor_fcw_speed + actuator_speed<THROTTLE_MIN)
        motor_fcw_speed=getAvg(motor_fcw_speed,THROTTLE_MIN);
      else
        motor_fcw_speed=getAvg(motor_fcw_speed,imotor_fcw_speed+actuator_speed);
      if(imotor_bccw_speed + actuator_speed < THROTTLE_MIN)
        motor_bccw_speed=getAvg(motor_bccw_speed,THROTTLE_MIN);
      else
        motor_bccw_speed=getAvg(imotor_bccw_speed+actuator_speed,motor_bccw_speed);
    }
    else          //INCREASING THE SPEED OF THE RIGHT MOTORS
    {
      if(imotor_fccw_speed-actuator_speed>THROTTLE_MAX)
        motor_fccw_speed=getAvg(motor_fccw_speed,THROTTLE_MAX);
      else
        motor_fccw_speed=getAvg(motor_fccw_speed, imotor_fccw_speed-actuator_speed);
      if(imotor_bcw_speed-actuator_speed>THROTTLE_MAX)
        motor_bcw_speed=getAvg(motor_bcw_speed,THROTTLE_MAX);
      else
        motor_bcw_speed=getAvg(motor_bcw_speed, imotor_bcw_speed-actuator_speed);
    }
  }
  else if(err>0) //ROLL LEFT
  {
    if(imotor_fcw_speed + imotor_bccw_speed <imotor_fccw_speed+imotor_bcw_speed)  //RIGHT MOTORS SPEED GREATER THAN LEFT MOTORS SPEED
                                                                                  //REDUCE RIGHT MOTORS SPEED
    {
      if(imotor_fccw_speed-actuator_speed < THROTTLE_MIN)
        motor_fccw_speed=getAvg(motor_fccw_speed,THROTTLE_MIN);
      else
        motor_fccw_speed=getAvg(motor_fccw_speed,imotor_fccw_speed-actuator_speed);
      if(imotor_bcw_speed-actuator_speed < THROTTLE_MIN)
        motor_bcw_speed=getAvg(motor_bcw_speed,THROTTLE_MIN);
      else
        motor_bcw_speed=getAvg(motor_bcw_speed,imotor_bcw_speed-actuator_speed);
    }
    else                                                                          //LEFT MOTORS SPEED LESS THAN RIGHT MOTORS SPEED
                                                                                  //INCREASE LEFT MOTORS SPEED
    {
      if(imotor_fcw_speed+actuator_speed>THROTTLE_MAX)
        motor_fcw_speed=getAvg(motor_fcw_speed,THROTTLE_MAX);
      else
        motor_fcw_speed=getAvg(motor_fcw_speed,imotor_fcw_speed+actuator_speed);
      if(imotor_bccw_speed+actuator_speed>THROTTLE_MAX)
        motor_bccw_speed=getAvg(motor_bccw_speed,THROTTLE_MAX);
      else
        motor_bccw_speed=getAvg(motor_bccw_speed,imotor_bccw_speed+actuator_speed);
    }
  }
  else //perfect position
  {
    roll_pid_l=false;
    roll_pid_r=false;
  }
  
  //*********WRITING_PID VALUES*****************
  setSpeed_fccw(motor_fccw_speed);
  setSpeed_bcw(motor_bcw_speed);
  setSpeed_bccw(motor_bccw_speed);
  setSpeed_fcw(motor_fcw_speed);
  //********TESTING PID**************************
  #ifdef TEST_PID
    Serial.println(actuator_speed);
    Serial.print(F("MOTOR_FCCW: "));
    Serial.println(motor_fccw_speed);
    Serial.print(F("MOTOR_BCW: "));
    Serial.println(motor_bcw_speed);
    Serial.print(F("MOTOR_BCCW: "));
    Serial.println(motor_bccw_speed);
    Serial.print(F("MOTOR_FCW: "));
    Serial.println(motor_fcw_speed);
  #endif
}
int getAvg(int a, int b)
{
  return (a+b)/2;
}

