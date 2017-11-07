#include <avr/io.h>
#include <avr/interrupt.h>
#include "C:\includes\io.c"

volatile unsigned char TimerFlag = 0;
unsigned long avr_timer_M = 1;
unsigned long avr_timer_cntcurr = 0;

typedef struct tasks{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*tickFnctn)(int); 
    }task;

//CHANGE THIS ACCORDING TO THE NUMBER OF TASKS INSIDE THE MICROCONTROLLER
//AND THE GCD OF ALL THE TASKS TIME PERIOD
//**************************************************
const unsigned short tasksNum=0;
const unsigned long taskPeriodGCD=0;
task taskList[4]; //task taskList[tasksNum]-->change to value of tasksNum
//**************************************************


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
    unsigned char i;
    for(i=0;i<tasksNum;i++)
    {
        if(taskList[i].elapsedTime>=taskList[i].period)
        {
            taskList[i].elapsedTime=0;
            taskList[i].state=taskList[i].tickFnctn(taskList[i].state);
        }   
        taskList[i].elapsedTime+=taskPeriodGCD;
    }
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

//*****************STATE_MACHINES**************************


//**********END_OF_STATE_MACHINE_CODE**********************

int main(){
    DDRB=0xFF;PORTB=0x00;
    DDRD=0xFF;PORTD=0x00;
    LCD_init();
    LCD_DisplayString(1,"Hello World!");
    TimerSet(10);
    TimerOn();
    while(1){
        
    }
    return 0;
}