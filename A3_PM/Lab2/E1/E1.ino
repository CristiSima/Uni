#define BUT 2
bool led_state;

void USART0_init()
{
  /* seteaza baud rate la 9600 */
  UBRR0 = 103;

  /* porneste transmitatorul */
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);

  /* seteaza formatul frame-ului: 8 biti de date, 1 bit de stop, fara paritate */
  UCSR0C &= ~(1 << USBS0);
  UCSR0C |= (3 << UCSZ00);
}

void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  USART0_init();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUT, INPUT);
  led_state=0;

  print("asdas22d");
}

int mode;
int ts=0;
#define MODE_OFF 0
#define MODE_ON 1
#define MODE_BLINK 2


void write(char data)
{
  while (!(UCSR0A & (1 << UDRE0)))
    ;

  /* pune datele in buffer; transmisia va porni automat in urma scrierii */
  UDR0 = data;
}

void print(String str)
{
  for(int i=0;i<str.length();i++)
    write(str.charAt(i));
}

void println(String str)
{ 
    print(str);
    write('\n');
} 


int read()
{

//  return Serial.read();
  while (!(UCSR0A & (1 << RXC0)))
    ;

  /* returneaza datele din buffer */
  return UDR0;
}

String readLine() 
{
  String line="";
  int next_char;
//  Serial.println("reading");
  while((next_char=read())!='\n')
  {
    if(next_char == -1)
      continue;
    line.concat((char)next_char);
  }

  return line;
}

void proccess_command()
{
   String command = readLine();
   if(command == "on")
   {
    mode=MODE_ON;
    led_state=1;
   }
   else if(command == "off")
   {
    mode=MODE_OFF;
    led_state=0;
   }
   else if(command == "blink")
   {
    mode=MODE_BLINK;
    ts=millis();
   }
   else if(command == "get")
   {
      print("Ledu-i ");
      println(String(led_state));
   }
   else
   {
      print("Ce vrei ma? Ce ii aia ");
      println(command);
   }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(UCSR0A & (1 << RXC0))
    proccess_command();

  if(mode==MODE_OFF)
  {
    digitalWrite(LED_BUILTIN, 0);
  }
  else if(mode==MODE_ON)
  {
    digitalWrite(LED_BUILTIN, 1);
  }
  else
  {
    if ((millis() - ts) >= 100)
    {
      ts = millis();
      led_state=!led_state;
      digitalWrite(LED_BUILTIN, led_state);
    }
  }
}
