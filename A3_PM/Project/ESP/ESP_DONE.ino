#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

extern "C" {
  #include "user_interface.h"
}

const char index_html[]="<head><meta http-equiv=\"Content-Type\"content=\"text/html;charset=utf-8\"/><title>Wifi μDucky</title></head><body><div style=\"display:flex;align-items:center;flex-direction:column;\"><button style=\"border-radius:0;border:0px;padding:10px;background:lightgray;\"onclick=\"toggle_insta_send(this)\">INSTANT SEND</button><textarea name=\"payload\"id=\"payload\"style=\"width:70%;height:500px;margin:10px;\"placeholder=\"#P:WIN#r#R:WIN##sleep20ms#notepad#T:ENTER#\"></textarea><button style=\"border-radius:10px;padding:5px\"onclick=\"send_textarea()\">TYPE</button></div><script>var insta_send=false;const hex_encode=(text)=>text.split(\"\").map(c=>c.charCodeAt(0).toString(16).padStart(2,\"0\")).join(\"\");function send_textarea(){var textarea=document.getElementById(\"payload\");var contents=textarea.value;if(contents==\"\")return;textarea.value=\"\";console.log(contents);fetch(\"/type?type=\"+hex_encode(contents));}function toggle_insta_send(butt){insta_send=!insta_send;butt.style.background=insta_send?\"lightgreen\":\"lightgray\";}function timer(){setTimeout(timer,50);if(insta_send)send_textarea();}timer();</script></body>";

const char *ssid = "CineEstiSiDeUndeVii";
const char *password = "SuntOTastatura";

ESP8266WebServer server(80);

const char fake_beacon_ssid[]="Vulturul a parasit cuibul!!!!";
const char arm_beacon_ssid[]="Repet, Vulturul a parasit cuibul";

const int fake_beacon_ssid_len = sizeof(fake_beacon_ssid)-1;
const int arm_beacon_ssid_len = sizeof(arm_beacon_ssid)-1;

#define timestamp_offset 24
#define ssid_len_offset 37
#define ssid_offset 38

#define channel_offset (50)

uint8_t packet[128] = {
        0x80, 0x00, 0x00, 0x00, //Frame Control, Duration
/*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination address 
/*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //Source address - overwritten later
/*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //BSSID - overwritten to the same as the source address
/*22*/  0xc0, 0x6c, //Seq-ctl
/*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, //timestamp - the number of microseconds the AP has been active
/*32*/  0x64, 0x00, //Beacon interval
/*34*/  0x01, 0x04, //Capability info
/* SSID */
/*36*/  0x00, 0x06, /* SSID LEN*/
/*38*/  0x72, 0x72, 0x72, 0x72, 0x72, 0x72
/* SSID name !!! var-len !!! */
};
int packet_len;
                                               
const uint8_t postSSID[]={
    0x01, 0x08, 0x82, 0x84,
    0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01, 
    0x04
};
void build_fake_beacon(const char *ssid, byte channel)
{
    wifi_set_channel(channel);
    // set source MAC
    packet[10] = packet[16] = random(256);
    packet[11] = packet[17] = random(256);
    packet[12] = packet[18] = random(256);
    packet[13] = packet[19] = random(256);
    packet[14] = packet[20] = random(256);
    packet[15] = packet[21] = random(256);
    
    byte ssid_len=strlen(ssid);
    packet[ssid_len_offset]=ssid_len;
    memcpy(packet+ssid_offset, ssid, ssid_len);

    // append the rest
    memcpy(packet+ssid_offset+ssid_len, postSSID, sizeof(postSSID));
    
    packet[channel_offset+ssid_len] = channel;
    
    packet_len=51+ssid_len;
    
}
void ICACHE_FLASH_ATTR send_fake_beacon()
{
    wifi_send_pkt_freedom(packet, packet_len, 1);
}


void Serial_print_buffer(uint8_t *buff, uint16 len)
{
    Serial.print("Got ");
    if(len<100)
        Serial.print(" ");
    Serial.print(len);
    Serial.print(" : ");
    for(int i=0;i<len;i++)
    {
        if(buff[i]<0x10)
            Serial.print(0);
        Serial.print(buff[i], HEX);
    }
    Serial.print("\n");
}

void arm_webserver();
void wifi_promiscous_rx_callback(uint8_t *buff, uint16 len)
{
//    filter for beacons
//  not sure why I need the offset
    if(len<23 || memcmp(buff+12, "\x80\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF", 10) != 0)
        return;

    const int rx_beacon_ssid_len_offset = 49;
    const int rx_beacon_ssid_offset = 50;

    int ssid_len = buff[rx_beacon_ssid_len_offset];

//    Serial.print("Got beacon for network with name len: ");
//    Serial.println(ssid_len);

    if(ssid_len != arm_beacon_ssid_len || memcmp(buff+rx_beacon_ssid_offset, arm_beacon_ssid, arm_beacon_ssid_len)!=0)
        return;
    
//    Serial_print_buffer(buff, len);
    arm_webserver();
}

os_timer_t beacon_timer;
int OP_MODE=0;

#define OP_MODE_WAITING 1
#define OP_MODE_ARMED 2

void enter_waiting_mode()
{
    OP_MODE=OP_MODE_WAITING;
    
    os_timer_disarm(&beacon_timer);
    os_timer_setfn(&beacon_timer,(os_timer_func_t *)send_fake_beacon,0);
    os_timer_arm(&beacon_timer, 100, true);

    wifi_set_opmode(STATION_MODE);
//  idk
    wifi_promiscuous_enable(1);
    wifi_set_promiscuous_rx_cb(wifi_promiscous_rx_callback);
    build_fake_beacon(fake_beacon_ssid, 5);
}

void arm_webserver()
{
    os_timer_disarm(&beacon_timer);
    OP_MODE=OP_MODE_ARMED;

    wifi_promiscuous_enable(0);

//    Serial.println("ARMED");
    delay(500);
    WiFi.softAP(ssid, password, -1, true);
    server.on("/", handleRoot);
    server.on("/type", handleType);
    server.begin();
}

void handleRoot() {
  server.send(200, "text/html", index_html);
}

inline uint8_t hex_digit_value(char c)
{
    if ('0' <= c && c <= '9') return (c - '0');
    if ('A' <= c && c <= 'F') return (c - 'A' + 10);
    if ('a' <= c && c <= 'f') return (c - 'a' + 10);
    return -1;
}

void handleType()
{
    server.send(200, "text/plain", "OK");
    if(!server.hasArg("type"))
        return;

    String typed=server.arg("type");
    for(int i=0;i<typed.length();i+=2)
        Serial.write(hex_digit_value(typed[i])<<4|hex_digit_value(typed[i+1]));
//    Serial.println("");
    
}

void setup() {
    Serial.begin(115200);
    delay(500);
    
    enter_waiting_mode();

//    FOR DEV
//    arm_webserver();
}


void loop() { 
//    send_fake_beacon();
//    delay(100);
    if(OP_MODE==OP_MODE_ARMED)
        server.handleClient();
}
