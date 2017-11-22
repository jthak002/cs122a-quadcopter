#include <avr/io.h>
#include <avr/interrupt.h>
#include "C:\includes\io.c"
#include "C:\includes\usart.h"

/*********REMOTE CONTROL TRANSMISSION CODES*********/
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

//******STATUS_CODE_GLOBAL_VARIABLES
unsigned char RUN_CODE=0x00;
unsigned char DISABLE_QUAD=0x00;



/*************TIMER FUNCTIONS**************/
volatile unsigned char TimerFlag = 0;
unsigned long avr_timer_M = 1;
unsigned long avr_timer_cntcurr = 0;
void TimerOn(){
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    avr_timer_cntcurr = avr_timer_M;
    SREG = 0x80;
}

void TimerOff(){
    TCCR1B = 0x00;
}

void TimerISR(){
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
    avr_timer_cntcurr--;
    if (avr_timer_cntcurr == 0){
        TimerISR();
        avr_timer_cntcurr = avr_timer_M;      
    }
}

void TimerSet(unsigned long M){
    avr_timer_M = M;
    avr_timer_cntcurr = avr_timer_M;
    
}
/*************ADC FUNCTION**************/
void A2D_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    // ADEN: Enables analog-to-digital conversion
    // ADSC: Starts analog-to-digital conversion
    // ADATE: Enables auto-triggering, allowing for constant
    //	    analog to digital conversions.
}


/***************STATE_MACHINES**********************/
//READS THE DATA FROM THE JOYSTICK AND STORES IN THE adc_inputs[] ARRAY.
//DATA IS STORED AS -2,-1,0,1,2 BASED ON MAGNITUDE OF JOYSTICK PUSH
//adcmux_state IS USED TO STURE THE VALUE OF THE ADMUX REGISTER, WHICH IS USED TO 
//READ IN THE ADC VALUES FROM THE DIFFERENT ADC INPUT PINS A0,A1,A2 OR A4.
enum SM1_States{Start1,read_joystick}SM1_State;
signed char adc_inputs[4];
unsigned char adcmux_state=0;
unsigned short input=0;
void SM1_Tick(){
    switch(SM1_State){
        case Start1:
            if(RUN_CODE)
                SM1_State=read_joystick;
            else
                SM1_State=Start1;
            break;
        case read_joystick:
            if(RUN_CODE)
                SM1_State=read_joystick;
            else
                SM1_State=Start1;
            break;
        default:break;
    }
    switch(SM1_State){
        case read_joystick:
        input=ADC;
        if(adcmux_state==3)
        ADMUX=0;
        else
        ADMUX=adcmux_state+1;
        if(input>550 && input<828)
        adc_inputs[adcmux_state]=1;
        else if(input>828)
        adc_inputs[adcmux_state]=2;
        else if(input<250)
        adc_inputs[adcmux_state]=-2;
        else if(input>250 && input<470)
        adc_inputs[adcmux_state]=-1;
        else
        adc_inputs[adcmux_state]=0;
        if(adcmux_state==3)
        adcmux_state=0;
        else
        adcmux_state+=1;
        break;
        default:break;
    }
}
//UPDATES THE VALUES ONTO THE LCD SCREEN
//EITHER SHOWS 'CONNECTING...' OR THE VLAUES OF 
//THROTTLE,PITCH,YAW AND ROLL
enum SM2_States{Start2,led_prep,led_display}SM2_State;
void SM2_Tick(){
    switch(SM2_State){
        case Start2:
            if(RUN_CODE)
                SM2_State=led_prep;
            else
                SM2_State=Start2;
            break;
        case led_prep:
            SM2_State=led_display;
            break;
        case led_display:
            if(RUN_CODE)
                SM2_State=led_display;
            else
                SM2_State=Start2;
            break;
        default:break;
    }
    switch(SM2_State){
        
        case Start2:
            LCD_ClearScreen();
            LCD_DisplayString(1,"CONNECTING....");
            break;
            
        //Prepares the LCD Display for displaying controller Data
        case led_prep:
            LCD_ClearScreen();
            LCD_DisplayString(1,"Thrtl: 0 Yaw: 0");
            unsigned char second_string []={'P','t','c','h',':',' ','0',' ','R','o','l','l',':',' ','0'};
            for(int m=0;m<15;m++)
            {
                LCD_Cursor(17+m);
                LCD_WriteData(second_string[m]);
            }
            break;
            
        //DISPLAYS THE CONTROLLER DATA-->DATA THAT THE CONTROLLER SENDS OVER THE BLUETOOTH
        case led_display:
        //Displaying the THROTTLE_DATA
        LCD_Cursor(7);
        if(adc_inputs[3]<0){
            LCD_WriteData('-');
        }
        else
            LCD_WriteData(' ');
        LCD_Cursor(8);
        LCD_WriteData(48+abs(adc_inputs[3]));
        //Displaying the YAW_DATA
        LCD_Cursor(14);
        if(adc_inputs[2]<0)
            LCD_WriteData('-');
        else
            LCD_WriteData(' ');
        LCD_Cursor(15);
        LCD_WriteData(48+abs(adc_inputs[2]));
        //Displaying the PITCH_DATA
        LCD_Cursor(22);
        if(adc_inputs[1]<0)
            LCD_WriteData('-');
        else
            LCD_WriteData(' ');
        LCD_Cursor(23);
        LCD_WriteData(48+abs(adc_inputs[1]));
        //Displaying the ROLL_DATA
        LCD_Cursor(30);
        if(adc_inputs[0]<0)
            LCD_WriteData('-');
        else
            LCD_WriteData(' ');
        LCD_Cursor(31);
        LCD_WriteData(48+abs(adc_inputs[0]));   
        LCD_Cursor(32);          
        break;
        
        default:break;
    }
}
//TRANSMITS THE DATA FROM THE JOYSTIUCKS TO THE FLIGHT CONTROLLER. 
//EVERY OTHER CLOCK_PERIOD THE DATA FOR PITCH-ROLL OR THROTTLE-YAW IS TRANSMITTED
enum SM3_States{Start3, controller_send}SM3_State;
unsigned char send_val=0x00;
void SM3_Tick(){
    switch(SM3_State)
    {
        case Start3:
            if(RUN_CODE)
                SM3_State=controller_send;
            else
                SM3_State=Start3;
            break;
        case controller_send:
            if(RUN_CODE)
            SM3_State=controller_send;
            else
            SM3_State=Start3;
            break;
        default:break;
    }
    switch(SM3_State){
        case controller_send:
            //CHECK IF USER HAS INITIATED DISABLE_QUAD_VARIABLE TO 
            //GROUND THE QUADCOPTER VIA GRADUAL DESCENT 
            if(DISABLE_QUAD==0x00)
            {
                if(send_val)
                {
                    //TRANSMIT ROLL CONTROLLER INPUT TO QUAD
                    if(USART_IsSendReady(1))
                    {
                        //ADC_INPUTS[0] HAVE ROLL VALUES STORED IN THEM
                        switch(adc_inputs[0])
                        {
                            case -2:
                                USART_Send(ROLL_LEFT_2,1);
                                break;
                            case -1:
                                USART_Send(ROLL_LEFT_1,1);
                                break;
                            case 0:
                                USART_Send(ROLL_NEUTRAL,1);
                                break;
                            case 1:
                                USART_Send(ROLL_RIGHT_1,1);
                                break;
                            case 2:
                                USART_Send(ROLL_RIGHT_2,1);
                                break;
                            default:break;
                        }
                        while(!USART_HasTransmitted(1));
                    }
                    //TRANSMIT ROLL CONTROLLER INPUT TO QUAD
                    if(USART_IsSendReady(1))
                    {
                        switch(adc_inputs[1]){
                            case -2:
                                USART_Send(PITCH_BCK_2,1);
                                break;
                            case -1:
                                USART_Send(PITCH_BCK_1,1);
                                break;
                            case 0:
                                USART_Send(PITCH_NEUTRAL,1);
                                break;
                            case 1:
                                USART_Send(PITCH_FWD_1,1);
                                break;
                            case 2:
                                USART_Send(PITCH_FWD_2,1);
                                break;
                        }
                        while(!USART_HasTransmitted(1));
                    }
                } 
                else{                    
                    //TRANSMIT YAW CONTROLLER INPUT TO QUAD
                    if(USART_IsSendReady(1))
                    {
                        switch(adc_inputs[2]){
                            case -2:
                            USART_Send(YAW_LEFT_2,1);
                            break;
                            case -1:
                            USART_Send(YAW_LEFT_1,1);
                            break;
                            case 0:
                            USART_Send(YAW_NEUTRAL,1);
                            break;
                            case 1:
                            USART_Send(YAW_RIGHT_1,1);
                            break;
                            case 2:
                            USART_Send(YAW_RIGHT_2,1);
                            break;
                        }
                        while(!USART_HasTransmitted(1));
                    }
                    //TRANSMIT THROTTLE CONTROLLER INPUT TO QUAD
                    if(USART_IsSendReady(1))
                    {
                        switch(adc_inputs[3]){
                            case -2:
                            USART_Send(THROTTLE_DWN_2,1);
                            break;
                            case -1:
                            USART_Send(THROTTLE_DWN_1,1);
                            break;
                            case 0:
                            USART_Send(THROTTLE_NEUTRAL,1);
                            break;
                            case 1:
                            USART_Send(THROTTLE_UP_1,1);
                            break;
                            case 2:
                            USART_Send(THROTTLE_UP_2,1);
                            break;
                        }
                        while(!USART_HasTransmitted(1));
                    }
                }
                send_val=~send_val;                        
            }
            //TRANSMIT GROUNDING CODE
            else
            {
                if(USART_IsSendReady(1))
                USART_Send(ERROR_HALT,1);
                while(!USART_HasTransmitted(1));
            }
            break; 
            default:break;
    }    
}
//THIS STATE MACHINE CHECKS CONNECTIVITY OF THE BT MODULE
//THE CONTROLLER GOES INTO SEARCH MODE AS SOON AS THE BLUETOOTH CONNECTION IS LOST
//NORMAL FUNCTIONALITY IS RESUMED ONCE CONNECTION IS RE-ESTABLISHED
enum SM4_States{Start4, CHECK_CONNECTIVITY} SM4_State;
void SM4_Tick(){
    switch(SM4_State){
        case Start4:
            SM4_State=CHECK_CONNECTIVITY;
            break;
        case CHECK_CONNECTIVITY:
            SM4_State=CHECK_CONNECTIVITY;
            break;
        default: break;  
    }
    switch(SM4_State){
        case CHECK_CONNECTIVITY:
            if(PINC&0x01)
                RUN_CODE=0x01;
            else
                RUN_CODE=0x00;
            break; 
               
        default:break;
    }
}
//LISTS ALL THE STATE MACHINES FOR NORMAL FUNCTIONING OF THE 
//CONTROLLER.
/*
PORTA=ADC
PORTB(OUTPUT)=LCD_DATA_BUS
PORTD=(OUTPUT)LCD_CONTROL_BUS(PIN0&1)
      BLUETOOTH_UART(PIN2&3=TX|RX)
PORTC(INPUT)=STATE PIN OF THE BLUETOOTH MODULE
CLOCK CYCLE:10ms
PERIOD OF EXECUTION:
SM1_TICK()-10MS
SM2_TICK()-300MS
SM3_TICK()-310MS
SM4_TICK()-10MS
*/
int main(void)
{
    DDRB=0xFF;
    PORTB=0x00;
    DDRC=0x00;
    PORTC=0xFF;
    DDRD=0xFF;
    PORTD=0x00;
    /* Replace with your application code */
    SM1_State=Start1;
    SM2_State=Start2;
    SM3_State=Start3;
    SM4_State=Start4;
    TimerSet(10);
    TimerOn();
    LCD_init();
    A2D_init();
    initUSART(1);
    ADMUX=0;
    unsigned char counter=1;
    LCD_DisplayString(1,"CONNECTING....");
    while(1)
    {
        if(counter%21==0){
            SM2_Tick();
            counter++;
        }
        else if(counter%31==0)
        {
            SM3_Tick();
            counter=1;
        }
        else{
            SM1_Tick();
            SM4_Tick();
            counter++;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}