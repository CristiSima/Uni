#include "Keyboard.h"
#include <string.h>
#include "keyboard_decode.h"

#define atoi32_t atol
#define atoi16_t atoi

#define handle_special_entry(token_name, key_code_val) else if(!strcasecmp(token, token_name)) key_code = key_code_val;

void handle_special(const char *token)
{    
    if(!strncasecmp(token, "SLEEP", 5))
    {
        int16_t wait_time=atoi16_t(token+5);
//        Serial.print("Sleep: ");
//        Serial.println(wait_time);
        delay(wait_time);
        return;
    }

    char target_action;
    uint16_t key_code;
    if(token[1]==':')
    {
        target_action=token[0];
        token+=2;
    }
    else
//        tap by default
        target_action='T';

    if(!strncmp(token, "0x", 2))
    {
        key_code = strtol(token+2, NULL, 16);
    }
    else if(token[0]=='F' && '0' <= token[1] && token[1] <='9')
    {
        uint8_t F_key_index=token[1]-'0';
        if(token[2])
            F_key_index=F_key_index*10+token[2]-'0';
        key_code=KEY_F1 + F_key_index - 1;
    }
    handle_special_entry("ENTER", KEY_RETURN)

    handle_special_entry("WIN", KEY_LEFT_GUI)
    handle_special_entry("CTRL", KEY_RIGHT_CTRL)
    handle_special_entry("SHIFT", KEY_LEFT_SHIFT)
    handle_special_entry("ALT", KEY_LEFT_ALT)
    
    handle_special_entry("LEFT", KEY_LEFT_ARROW)
    handle_special_entry("RIGHT", KEY_RIGHT_ARROW)
    handle_special_entry("UP", KEY_UP_ARROW)
    handle_special_entry("DOWN", KEY_DOWN_ARROW)
    
    handle_special_entry("BACKSPACE", KEY_BACKSPACE)
    handle_special_entry("CAPS", KEY_CAPS_LOCK)
    handle_special_entry("NUM_LOCK", KEY_NUM_LOCK)
    
    handle_special_entry("CONTEXT", KEY_MENU)
    handle_special_entry("ESC", KEY_ESC)
    
    else
//        unknow
        return;

    if(target_action == 'T')
    {
        Keyboard.press(key_code);
        delay(10);
        Keyboard.release(key_code);
    }
    else if(target_action == 'P')
        Keyboard.press(key_code);
    else if(target_action == 'R')
        Keyboard.release(key_code);
}

volatile int neutralised = 0;

ISR(PCINT0_vect)
{
    if(neutralised)
        return;
    neutralised=1;
    Keyboard.end();
//    Serial.println("NEUTRALIZED");
}

ISR(USART1_RX_vect)
{
    append_char(UDR1);
}

void test()
{
    reset();
    append("#P:WIN#r#R:WIN##SLEEP50ms#notepad#ENTER##sleep100ms#You Got hacked");

    
    const char *token;
    uint8_t is_special=0;
    do
    {
        token=get_next(&is_special);
        if(is_special)
            handle_special(token);
        else
            Keyboard.print(token);
    }
    while(token!=NULL);
}

void conf_interupts()
{
    cli();

    // PCINT0 (PB)
    PCICR = 1;              // enable
    PCMSK0 = (1 << PCINT7); // PB7 / P11
    
    sei();
}


void conf_hw_serial()
{
    cli();
    UCSR1A = 0;
    // RX enable, RX interupt, 
    UCSR1B = (1<<RXCIE1) | (1<<RXEN1) ;
    // no parity, 1 stop, 
    UCSR1C = (3<<UCSZ10);
//    BAUD: 115.2k
    UBRR1L=8;

    sei();
}

void setup() {
    reset();
    //Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
//    delay(5000);
    //while(!Serial);
//    Serial.println("Compiled on "  __TIME__);
    delay(100);
    Keyboard.begin();
    Keyboard.releaseAll();
    conf_hw_serial();
    delay(1000);
//    Serial.println("START");
//    test();
    //Keyboard.end();

}

void loop() {
    // put your main code here, to run repeatedly:
    const char *token;
    uint8_t is_special=0;
    do
    {
        token=get_next(&is_special);
        
        if(!token)
            continue;
            
        if(is_special)
            handle_special(token);
        else
            Keyboard.print(token);
    }
    while(1);
}
