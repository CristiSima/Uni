#include "SPI.h"
 
void setup(void)
{ 
  byte masterSend = 1;
  byte masterRecv=0;
  Serial.begin(9600);
 
  /* Inițializarea pinului de Slave Select */
  pinMode(SS, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, INPUT);
  digitalWrite(SS, HIGH);
 
  /* Inițializarea SPI-ului */
//  SPI.begin();
  /* set master mode */
  SPCR |= (1 << MSTR);
  
  /* set prescaler 16 */
  SPCR |=  (1 << SPR0);
  
  /* enable SPI  */
  SPCR |= (1 << SPE);

 
  /* Selectarea Slave-ului */
  digitalWrite(SS, LOW);
 
  /* Trimiterea byte-ului masterSend către Slave-ul selectat.
   * Salvarea byte-ului primit de la Slave-ul selectat în masterRecv. */
  char buff[10]="*_*\n";
//  SPI.transfer(buff, 3); 
  for(int i=0;i<4;i++)
//    Serial.print(SPI.transfer(buff[i]));
  {
    /* Start transmission */
    SPDR = buff[i];
    
    /* Wait for transmission complete */
    while(!(SPSR & (1 << SPIF))); 
    Serial.print(SPDR);
  }
 
  /* Deselectarea Slave-ului */
  digitalWrite(SS, HIGH);

  Serial.println(masterRecv);
}
void loop() {
}
