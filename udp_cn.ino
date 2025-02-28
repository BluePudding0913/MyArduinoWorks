#include <WiFi.h>
#include <WiFiUDP.h>
#define EAP_IDENTITY "ID"
#define EAP_PASSWORD "PW"
const char *ssid = "SSID"; //SSID
static WiFiUDP wifiUdp; 
IPAddress remoteIP; // 相手のIPアドレス
int port;
//const int led = 4;

static void WiFi_setup()
{
 static const int kLocalPort = 7000;  //自身のポート
 WiFi.begin(ssid,WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_IDENTITY, EAP_PASSWORD);//EAPのネットワーク
 while(WiFi.status() != WL_CONNECTED){
    delay(400);
    Serial.print(".");
  }

 wifiUdp.begin(kLocalPort);
}
static void Serial_setup()
{
 Serial.begin(115200);
 Serial.println("");
}
void setup() {
 Serial_setup();
 WiFi_setup();
 //pinMode(led, OUTPUT);
}
void loop() 
{
 int j,k;
 char i[64];
 while(1){
  if (wifiUdp.parsePacket()) {
  
   j = wifiUdp.read(i,64);
   i[j]= '\0';
   
   remoteIP = wifiUdp.remoteIP();
   port = wifiUdp.remotePort(); 
      
   Serial.print(remoteIP);
   Serial.print(" / ");
   Serial.print(port);
   Serial.print(" / ");
   
   Serial.print(i); // UDP通信で来た値を表示
   Serial.print("\n");
   k=atoi(i);
   //digitalWrite(led, k); 
 }
  
 delay(100);
 }
}