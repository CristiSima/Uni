#include "usart.h"

void setup()
{
  USART0_init();
  USART0_print("Hello\n");


  DDRD &= ~(1 << PD2); //setam bitul pe intrare
  PORTD |=(1<<PD2); // 
  
}

bool old_but=0;


void loop()
{
  
  int but = PIND & (1 << PD2); // citirea

  if(old_but && !but)
  {
    USART0_transmit('P');
    USART0_transmit('D');
    USART0_transmit('2');
    USART0_transmit(' ');
    USART0_transmit('a');
    USART0_transmit('p');
    USART0_transmit('a');
    USART0_transmit('s');
    USART0_transmit('a');
    USART0_transmit('t');
    USART0_transmit('\n');
  }

  if(!old_but && but)
  {
    USART0_transmit('P');
    USART0_transmit('D');
    USART0_transmit('2');
    USART0_transmit(' ');
    USART0_transmit('l');
    USART0_transmit('a');
    USART0_transmit('s');
    USART0_transmit('a');
    USART0_transmit('t');
    USART0_transmit('\n');
  }
  old_but=but;
}
