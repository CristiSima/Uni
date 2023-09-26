#define INTERUP_COUNT 10
int interupt_counter=INTERUP_COUNT;
ISR(TIMER1_COMPA_vect) {
  // cod întrerupere
  interupt_counter--;
  if(!interupt_counter)
  {
    interupt_counter=INTERUP_COUNT;
    Serial.println("5s have passed");
  }
}
 
void configure_timer1() {
  // exemplu de configurare pentru Timer 1 în mod CTC
  // care va genera întreruperi cu frecvența de 2Hz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 31249;            // compare match register 16MHz/256/2Hz-1
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
}
 
void init_timer1() {
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cli();  
  configure_timer1();
  init_timer1();
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:

}
