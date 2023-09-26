void setup() {
  #define LED PB5
  // put your setup code here, to run once:
  DDRB |= (1 << LED);
  
  DDRD &= ~(1 << PD2); //setam bitul pe intrare
  PORTD |=(1<<PD2); // pullup
  
  DDRD &= ~(1 << PD3); //setam bitul pe intrare
  PORTD |=(1<<PD3); // pullup
}

int delay_interval=500;
int time_passed=0;
#define QUANTUM 50

void loop() {
  // put your main code here, to run repeatedly:
  int decrease_time = PIND & (1 << PD2); // citirea
  int increase_time = PIND & (1 << PD3); // citirea
  
  if (increase_time == 0)
    delay_interval+=50;
 
  if (decrease_time == 0 && delay_interval>150)
    delay_interval-=50;

  time_passed+=QUANTUM;
  if(time_passed>=delay_interval)
  {
    PORTB ^= (1 << LED);
    time_passed=0;
  }
  delay(QUANTUM);
}
