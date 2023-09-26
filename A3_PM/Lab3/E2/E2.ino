#define BUT1 PD2
#define BUT2 PD4
#define LED PD7

bool state=0;
int delay_interval=100;

ISR(INT0_vect)
{
  // cod întrerupere externă PD2 /INT0
  // verificare tranziție pe front crescător, descrescător sau oricare
  // (după cum este configurat INT0)
  if (digitalRead(BUT1)==0){
     delay_interval+=100;
  }
}

ISR(PCINT2_vect) {
  // cod întrerupere de tip pin change
  if (digitalRead(BUT2)==0){
     // întreruperea a fost generată de pinul PD4 / PCINT20
     // verificăm nivelul logic al pinului
     if(delay_interval>100)
      delay_interval-=100;
  }
  // întreruperea a fost generată de alt pin
}

int ts;

void setup() {
  // put your setup code here, to run once:
  
  pinMode(LED, OUTPUT);
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  Serial.begin(9600);
//  attachInterrupt(digitalPinToInterrupt(BUT1), stinge, FALLING);
//  attachInterrupt(0, aprinde, FALLING);

//  Serial.println(digitalPinToInterrupt(BUT1));
  Serial.println(digitalPinToInterrupt(BUT2));

  // întreruperi externe
  EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
  // întreruperi de tip pin change (activare vector de întreruperi)
  PCICR |= (1 << PCIE2); 

   // întrerupere externă
  EIMSK |= (1 << INT0);     // Turns on INT0
  // întrerupere de tip pin change
  PCMSK2 |= (1 << PCINT20); // Turns on PCINT20 (PD4)
  // activare întreruperi globale
  sei();
  ts = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - ts) >= delay_interval)
    {
      ts = millis();
      state=!state;
      digitalWrite(LED, state);
    }
  delay(10);
  
}
