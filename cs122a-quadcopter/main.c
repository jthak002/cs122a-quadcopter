#include <avr/io.h>
#include <avr/interrupt.h>
#include "C:\includes\io.c"

volatile unsigned char TimerFlag = 0;
unsigned long avr_timer_M = 1;
unsigned long avr_timer_cntcurr = 0;

unsigned char valB = 0x00;
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


void A2D_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    // ADEN: Enables analog-to-digital conversion
    // ADSC: Starts analog-to-digital conversion
    // ADATE: Enables auto-triggering, allowing for constant
    //	    analog to digital conversions.
}

enum SM1_States{Start1,read_joystick}SM1_State;
signed char adc_inputs[4];
unsigned char adcmux_state=0;
unsigned short input=0;
void SM1_Tick(){
    switch(SM1_State){
        case Start1:
            SM1_State=read_joystick;
            break;
        case read_joystick:
            SM1_State=read_joystick;
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


enum SM2_States{Start2,led_display}SM2_State;
void SM2_Tick(){
    switch(SM2_State){
        case Start2:
            SM2_State=led_display;
            break;
        case led_display:
            SM2_State=led_display;
            break;
        default:break;
    }
    switch(SM2_State){
        case led_display:
            LCD_Cursor(1);
            LCD_WriteData(48+abs(adc_inputs[0]));
            LCD_Cursor(3);
            LCD_WriteData(48+abs(adc_inputs[1]));
            LCD_Cursor(5);
            LCD_WriteData(48+abs(adc_inputs[2]));
            LCD_Cursor(7);
            LCD_WriteData(48+abs(adc_inputs[3]));
            LCD_Cursor(31);
            break;
        default:break;
    }
}    
int main(void)
{
    DDRB=0xFF;
    PORTB=0x00;
    DDRD=0xFF;
    PORTD=0x00;
    /* Replace with your application code */
    SM1_State=Start1;
    SM2_State=Start2;
    TimerSet(10);
    TimerOn();
    LCD_init();
    A2D_init();
    ADMUX=0;
    unsigned char counter=1; 
    LCD_DisplayString(1,"0 0 0 0");
    while(1)
    {
    //LCD_DisplayString(1,"Hello World!");    //LCD SCREEN CHECK
        SM1_Tick();
        if(counter%30==0){
            SM2_Tick();
            counter=1;
        }
        else{
            counter++;
        }                               
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}