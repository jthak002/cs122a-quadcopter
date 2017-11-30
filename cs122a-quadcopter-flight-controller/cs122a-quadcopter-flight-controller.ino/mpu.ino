#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "config.h"
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

//#define OUTPUT_READABLE_YAWPITCHROLL
// this outputs the
// yaw/pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.

//#define OUTPUT_READABLE_WORLDACCEL
// acceleration components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.

//control variables for the MPU
bool dmpReady=false; 
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
//orientation/motion variables
Quaternion q;
//float ypr[3];
VectorFloat gravity;
#ifdef GET_WORLD_ACCELERATION_VECTOR
VectorInt16 aa;
VectorInt16 aaReal;
VectorInt16 aaWorld;
#endif

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void mpu_Calibrate(){
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif


    //initialize the DMP
    devStatus = mpu.dmpInitialize();
    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(32);
    mpu.setYGyroOffset(29);
    mpu.setZGyroOffset(14);
    mpu.setZAccelOffset(631);

    //Testing the MPU OUTPUT

    if(devStatus==0){
      //turn on DMP once ready
      mpu.setDMPEnabled(true);
      //ARDUINO INTERRUPT DETECTION
      //creating interrupt for DMP data processing
      attachInterrupt(0,dmpDataReady,RISING);
      mpuIntStatus=mpu.getIntStatus();
      //making DMP ready to use
      dmpReady=true;
      //get packet size for later comparision
      packetSize = mpu.dmpGetFIFOPacketSize();
    }
    else
    {
    //ERROR ---> SEND ERROR BITS OVER BLUETOOTH
    }
}
void mpu_acquisition(){
  if(!dmpReady)
    return;     //Invalid state of the DMP CANNOT PROCESS VALUES
  if(!mpuInterrupt && fifoCount < packetSize)
    return; //The buffer has not been filled yet
  else{
    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt=false;
    mpuIntStatus=mpu.getIntStatus();
    // get current FIFO count
    fifoCount = mpu.getFIFOCount();
    if((mpuIntStatus&0x10) || (fifoCount ==1024)){
      //BUFFER OVERFLOW CONDITION
      mpu.resetFIFO();
      //reset the FIFO stack to start again
    }
    else if (mpuIntStatus&0x02){
      
      //Indicator of Normal operation. data pending in buffer.
      while(fifoCount<packetSize){
        //wait until the buffer fills up with enough data to fill up one packet
        fifoCount=mpu.getFIFOCount();
      }
      
      //read a packet from the MPU's buffer
      mpu.getFIFOBytes(fifoBuffer,packetSize);
      //if more than one packet is available we should directly enter the  
      fifoCount-=packetSize;

      //GETTING THE YAW/PITCH/ROLL VALUES FROM THE DMP
      mpu.dmpGetQuaternion(&q,fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      mpu.resetFIFO();
      #ifdef GET_WORLD_ACCELERATION_VECTOR
        mpu.dmpGetAccel(&aa,fifoBuffer);
        mpu.dmpGetGravity(&gravity,&q);
        mpu.getLinearAccel(&aaReal,&aa,&gravity);
        mpu.getLinearAccelInWorld(&aaWorld,&aaReal,&q)l
      #endif
      #ifdef TEST_IMU
          Serial.print(ypr[YAW]*180/M_PI);
          Serial.print("\t");
          Serial.print(ypr[PITCH]*180/M_PI);
          Serial.print("\t");
          Serial.println(ypr[ROLL]*180/M_PI);
      #endif
    }
  }  
}

