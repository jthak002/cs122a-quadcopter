int BT_Recv_Controller_Data(){
  unsigned char controller_value=0;
  //CHECK CONNECTIVITY OF BLUETOOTH CONTROLLER
  if(digitalRead(CONNECTION_STATE)==LOW)
  {
    #ifdef TEST_CONTROLLER
      Serial.println("ERROR--->CONNECTION LOST");
    #endif
    setSpeed(0);
    return -1;
  }
  if(BTSerial.available())
  {      
    controller_value=BTSerial.read();
    switch(controller_value){
      case THROTTLE_UP_1:
          #ifdef TEST_CONTROLLER
            Serial.println("THROTTLE_UP_1");
          #endif
          if(motor_curr_speed!=THROTTLE_SPEED_1)
          {
            if(!arm)
                arm=true;
            motor_curr_speed=THROTTLE_SPEED_1;
            setSpeed(THROTTLE_SPEED_1);
          }
          break;
        case THROTTLE_UP_2:
          #ifdef TEST_CONTROLLER
            Serial.println("THROTTLE_UP_2");
          #endif
          if(motor_curr_speed!=THROTTLE_SPEED_2)
          {
            if(!arm)
                arm=true;
            motor_curr_speed=THROTTLE_SPEED_2;
            setSpeed(THROTTLE_SPEED_2);
          }
          break;
        case THROTTLE_DWN_1:
          #ifdef TEST_CONTROLLER
            Serial.println("THROTTLE_DWN_1");
          #endif
          if(motor_curr_speed!=THROTTLE_SPEED__1)
            {
              if(!arm)
                arm=true;
              motor_curr_speed=THROTTLE_SPEED__1;
              setSpeed(THROTTLE_SPEED__1);
            }
          break;
        case THROTTLE_DWN_2:
          #ifdef TEST_CONTROLLER
            Serial.println("THROTTLE_DWN_2");
          #endif
          if(motor_curr_speed!=THROTTLE_SPEED__2)
            {
              if(!arm)
                arm=true;
              motor_curr_speed=THROTTLE_SPEED__2;
              setSpeed(THROTTLE_SPEED__2);
            }
          break;
        case THROTTLE_NEUTRAL:
          #ifdef TEST_CONTROLLER
            Serial.println("THROTTLE_NEUTRAL");
          #endif
          if(arm)
          {
            if(motor_curr_speed!=THROTTLE_HOVER)
            {
              motor_curr_speed=THROTTLE_HOVER;
              setSpeed(THROTTLE_HOVER);
            } 
          }
          else
            setSpeed(0);
        break;
        case PITCH_FWD_1:
        #ifdef TEST_CONTROLLER
          Serial.println("PITCH_FWD_1");
        #endif
        break;
        case PITCH_FWD_2:
        #ifdef TEST_CONTROLLER
          Serial.println("PITCH_FWD_2");
        #endif
        setSpeed(0);
        break;
        case PITCH_NEUTRAL:
        #ifdef TEST_CONTROLLER
          Serial.println("PITCH_NEUTRAL");
        #endif
        break;
        case PITCH_BCK_1:
        #ifdef TEST_CONTROLLER
          Serial.println("PITCH_BCK_1");
        #endif
        break;
        case PITCH_BCK_2:
        #ifdef TEST_CONTROLLER
          Serial.println("PITCH_BCK_2");
        #endif
        break;
        case ROLL_LEFT_2:
        #ifdef TEST_CONTROLLER
          Serial.println("ROLL_LEFT_2");
        #endif
        break;
        case ROLL_LEFT_1:
        #ifdef TEST_CONTROLLER
          Serial.println("ROLL_LEFT_1");
        #endif
        break;
        case ROLL_NEUTRAL:
        #ifdef TEST_CONTROLLER
          Serial.println("ROLL_NEUTRAL");
        #endif
        break;
        case ROLL_RIGHT_1:
        #ifdef TEST_CONTROLLER
          Serial.println("ROLL_RIGHT_1");
        #endif
        break;
        case ROLL_RIGHT_2:
        #ifdef TEST_CONTROLLER
          Serial.println("ROLL_RIGHT_2");
        #endif
        break;
        case YAW_LEFT_2:
        #ifdef TEST_CONTROLLER
          Serial.println("YAW_LEFT_2");
        #endif
        break;
        case YAW_LEFT_1:
        #ifdef TEST_CONTROLLER
          Serial.println("YAW_LEFT_1");
        #endif
        break;
        case YAW_NEUTRAL:
        #ifdef TEST_CONTROLLER
          Serial.println("YAW_NEUTRAL");
        #endif
        break;
        case YAW_RIGHT_1:
        #ifdef TEST_CONTROLLER
          Serial.println("YAW_RIGHT_1");
        #endif
        break;
        case YAW_RIGHT_2:
        #ifdef TEST_CONTROLLER
          Serial.println("YAW_RIGHT_2");
        #endif
        break;
        case ERROR_HALT:
        Serial.println("ERROR_HALT");
        setSpeed(0);
        break;
        default: break;
    }
    return controller_value;
  } 
}
