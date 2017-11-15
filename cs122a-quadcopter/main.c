#include <avr/io.h>
#include <avr/interrupt.h>
#include "C:\includes\io.c"
#include "C:\includes\usart.h"

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

unsigned char joystick_transmit[4];
unsigned char usart_send=0;
unsigned char usart_receive=0;
enum SM3_States{Start3,usart_send_receive}SM3_State;
void SM3_Tick(){
    switch(SM3_State){
        case Start3:
        SM3_State=usart_send_receive;
        break;
        case usart_send_receive:
        SM3_State=usart_send_receive;
        break;
        default:break;
    }
    switch(SM3_State){
        case usart_send_receive:
        if(USART_IsSendReady(1))
        {
            usart_send=0;
            usart_send=usart_send|(0x03&(abs(adc_inputs[0])));
            usart_send=usart_send|(0x0C&(abs(adc_inputs[1])<<2));
            usart_send=usart_send|(0x30&(abs(adc_inputs[2])<<4));
            usart_send=usart_send|(0xC0&(abs(adc_inputs[3])<<6));
            USART_Send(usart_send,1);
            while(!USART_HasTransmitted(1));
        }
        if(USART_HasReceived(1))
        {
            joystick_transmit[0]=0;
            usart_receive=USART_Receive(1);
            joystick_transmit[0]=usart_receive&0x03;
            joystick_transmit[1]=(usart_receive>>2)&0x03;
            joystick_transmit[2]=(usart_receive>>4)&0x03;
            joystick_transmit[3]=(usart_receive>>6)&0x03;
            USART_Flush(1);
        }
        
        break;
        default:break;
    }
}
enum SM4_States{Start4,lcd_display2}SM4_State;
void SM4_Tick(){
    switch(SM4_State){
        case Start4:
        SM4_State=lcd_display2;
        break;
        case lcd_display2:
        SM4_State=lcd_display2;
        break;
        default:break;
    }
    switch(SM4_State){
        case lcd_display2:
        LCD_Cursor(17);
        LCD_WriteData(48+abs(joystick_transmit[0]));
        LCD_Cursor(19);
        LCD_WriteData(48+abs(joystick_transmit[1]));
        LCD_Cursor(21);
        LCD_WriteData(48+abs(joystick_transmit[2]));
        LCD_Cursor(23);
        LCD_WriteData(48+abs(joystick_transmit[3]));
        LCD_Cursor(31);
        break;
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
    SM3_State=Start3;
    SM4_State=Start4;
    TimerSet(10);
    TimerOn();
    LCD_init();
    A2D_init();
    initUSART(1);
    ADMUX=0;
    unsigned char counter=1;
    LCD_DisplayString(1,"0 0 0 0");
    LCD_DisplayString(17,"0 0 0 0");
    while(1)
    {
        //LCD_DisplayString(1,"Hello World!");    //LCD SCREEN CHECK
        SM1_Tick();
        SM3_Tick();
        if(counter%30==0){
            SM2_Tick();
            SM4_Tick();
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