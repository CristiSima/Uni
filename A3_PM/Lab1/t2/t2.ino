void setup() {
  #define LED PB5
  // put your setup code here, to run once:
  DDRB |= (1 << LED);
  DDRD &= ~(1 << PD2); //setam bitul pe intrare
  PORTD |=(1<<PD2);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value_to_read = PIND & (1 << PD2); // citirea
  if (value_to_read == 0) {
    PORTB ^= (1 << LED);
    delay(500);
  }

  

}
