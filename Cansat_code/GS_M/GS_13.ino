#include <SPI.h>
#include <LoRa.h>
#define LED_red 4
#define LED_white 3
#define buzzer 5 
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");
  LoRa.setPins(10,7,2);
  pinMode(LED_red,OUTPUT);
  pinMode(LED_white,OUTPUT);
  pinMode(buzzer,OUTPUT);
  if (!LoRa.begin(460E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
         
    }else
    {
         for(int i = 0;i<3 ;i++)
    {
        digitalWrite(LED_red,HIGH);
        tone(buzzer,1000);
        delay(50);
        digitalWrite(LED_red,LOW);
        noTone(buzzer);
        delay(50);
  }
}
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
  alert_system();
    // print RSSI of packet
  }
}
void alert_system()
{
  tone(buzzer,1000);
  delay(90);
  noTone(buzzer);
  digitalWrite(LED_white,HIGH);
  delay(80);
  digitalWrite(LED_white,LOW);
}
