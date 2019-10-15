#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
void setup() {
  
  Serial.begin(9600);
  LoRa.setPins(10,7,2);
     if (!LoRa.begin(460E6)) {     
       
      Serial.println("start fail");
    }
    else
    {
      LoRa.setTxPower(17);
    }
}

void loop() {
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
}
String data = "";
void receiveEvent(int howMany){

  while (1 < Wire.available()) 
  {
    char c = Wire.read();
      if(c == 'A')
      {
        Serial.println(data);
        LoRa.beginPacket();
        LoRa.println(data);
        LoRa.endPacket();
        delay(500);
        data = "";
      }else
      {
        data += c;
      }
    }
  }
//      Serial.print(str1);
//      Serial.println("");
      

//     while (1 < Wire.available()) 
//  {
//    char d = Wire.read();
//    if(d!='\n'){
//      str2[j]+=d;
//      j++; 
//    }
//    else{
//      j=0;
//    }
//    }
      // Serial.print(String(str1) + String(str2));
//      Serial.println(String(str1));
      
//      
//      LoRa.print(String(str1) + String(str2));
//      LoRa.endPacket();
//}


 //send packet to ground_station
//void sendGround(String package)
//{
//  // slice String method
// int package_len = package.length()+1;
// char all_data [package_len] = "";
// package.toCharArray(all_data,package_len); 
// LoRa.beginPacket();
//   char strOut[30];
//  
//  int i =0;
//  while(i<=strOut.length())
//  {
//    if(i!= 30)
//    {
//      
//    }
//  i++;
//  }
//  
//} 
