#include <SoftwareSerial.h>

SoftwareSerial BTserial(10,11);
char recv_data=0;
void setup() 
{
    BTserial.begin(38400);  
}
 
void loop()
{
    if (BTserial.available())
    {  
        recv_data=BTserial.read();
        BTserial.write(recv_data);
        recv_data=0;
    }
}
