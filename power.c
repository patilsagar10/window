#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SET_Bit(PORT,BIT) PORT|=(1<<BIT)
#define CLR_Bit(PORT,BIT) PORT&=~(1<<BIT)
#define SET_bit(PORT,BIT)  PORT|=(1<<BIT)
#define CLR_bit(PORT,BIT)  PORT&=~(1<<BIT)
volatile int flag=0;

long readUltrasonicDistance(int pin)

{

  SET_bit(DDRD,pin);

  CLR_bit(PORTD,pin);

  _delay_ms(2);

  SET_bit(PORTD,pin);

  _delay_ms(5);

  CLR_bit(PORTD,pin);

  CLR_bit(DDRD,pin);

  return pulseIn(pin, HIGH);

}
 

int main()
{
  long  m, cm;
  SET_Bit(DDRC,PC2);
  SET_Bit(DDRD,PD5);
  DDRD&=~(1<<2);
  PORTD|=(1<<2);
  EICRA|=(1<<ISC00); 
  EIMSK|=(1<<INT0);
  sei();
 
  while(1)
  { 
     if(flag==1)  
     {
       cm = 0.01723 * readUltrasonicDistance(PD1);
       power_window_clockwise();
    
       if (cm<=50)
       {
        power_window_anticlockwise();
       }
     } 
    else 
     {
       power_window_anticlockwise();}
     } 
 

    


  return 0;
}
   




void power_window_clockwise()
{
    SET_Bit(PORTC,PC2);     //Motor rotates in Clockwise direction(Window moves upwards) 
   CLR_Bit(PORTD,PD5);
  
}




void power_window_anticlockwise()
{
  SET_bit(PORTD,PD5);     //Motor rotates in anti-clockwise direction(Window moves downwards)
  CLR_Bit(PORTC,PC2);
 
}
ISR(INT0_vect)
{
   if(!(PIND&(1<<PD2))) 
      flag=1;
   else
      flag=0;

}
