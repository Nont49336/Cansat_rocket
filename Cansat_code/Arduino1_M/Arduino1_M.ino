#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

Adafruit_BME280 bme280;

File DAT_F;

int counter = 0;
int ml_REF3V3 = A1; //ml8511 3.3V OUTPUT pin
int ml_UVOUT = A0; // ml8511 UVOUT pin
const int chipSelect = 10;
static const int RXPin = 9, TXPin =  8;
static const uint32_t GPSBaud = 4800;

TinyGPSPlus gps;

float alt_v1 ; //for Altitude
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(9600);
pinMode(6,OUTPUT);
  if(!bme280.begin())
  {
    //Serial.println("BME280 Error!");
  }
  if (!SD.begin(chipSelect)) {
    //Serial.println("\nCard Error");  
  }
 
    Wire.begin(); // join i2c bus (address optional for master
  pinMode(6, OUTPUT);
  ss.begin(GPSBaud);
 alt_v1 = bme280.readAltitude(1013.25);
//Code Updated
//  tone(6,2000);
//  delay(100);
//  noTone(6);
}
    
void loop() {
  //Declare CharArray
  char chdata1[32];
  char chdata2[32];
  char chdata3[32];
  char chdata4[32];

  //Declare
  float tem_v = bme280.readTemperature();
  float pres_v = bme280.readPressure()/ 100.0F;
  float hum_v  = bme280.readHumidity();
  float alt_F = bme280.readAltitude(1013.25) - alt_v1;
  if(String(alt_F) == " NAN")
  {
    alt_F = 0.00;
  }
  float uv_v = uv_measure();
  
  counter++;
  
  //float to String

//temp,humi,uv,cutter
  String data1 = String(alt_F)  + "," + String(tem_v) + "," +  String(hum_v)+ "," + String(uv_v)+",A"+'\n';
//TeamID,counter,alti
  String data2 = "58," + String(counter)+","+'\n';
  //GPSLat
  String data3 = GPSGetInfoLat() + ","+'\n';
  //GPSLng
  String data4 = GPSGetInfoLng() + ","+'\n';
  
  //String toCharArray
  data1.toCharArray(chdata1,(data1.length()+1));
  data2.toCharArray(chdata2,(data2.length()+1));
  data3.toCharArray(chdata3,(data3.length()+1));
  data4.toCharArray(chdata4,(data4.length()+1));
    
  Wire.beginTransmission(1);
  Wire.write(chdata2);
  //Serial.println(data2);
  Wire.endTransmission(); //stop transmitting
  delay(125);
  
  Wire.beginTransmission(1);
  Wire.write(chdata3);
  //Serial.println(data3);
  Wire.endTransmission(); //stop transmitting
  delay(125);
  
  Wire.beginTransmission(1);
  Wire.write(chdata4);
//  Serial.println(data4);
  Wire.endTransmission(); //stop transmitting
  delay(125);
  
  Wire.beginTransmission(1);
  Wire.write(chdata1);
  //Serial.println(data1);
  Wire.endTransmission(); //stop transmitting
  delay(125);
//Sending BME280

  DAT_F = SD.open("DATA.txt", FILE_WRITE);
  if (DAT_F) {
    DAT_F.print(data2);
    DAT_F.print(data3);
    DAT_F.print(data4);
    DAT_F.println(data1);
    DAT_F.flush();
    DAT_F.close(); 
}
digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
}


//GY8511 Start
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;
  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
  return (runningValue);
}
//The Arduino Map function but for floats
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float uv_measure()
{
  float uvLevel = averageAnalogRead(ml_UVOUT);
  float refLevel = averageAnalogRead(ml_REF3V3);
  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);
  uvIntensity = constrain(uvIntensity, 0.00, 15.00);

return uvIntensity;
} 
//GY8511 End


//GPS
String GPSGetInfoLat()
{
  String Coor1 = "";
   while (ss.available())
   {
    gps.encode(ss.read());
   }
  //if(1)
   if (gps.location.isValid())
   {
   float lati = gps.location.lat();
   Coor1 = String(lati,6);
  }
  else
  {
    Coor1 = String(0.00);
  } 
  return Coor1;
  }
  

  String GPSGetInfoLng()
{
  String Coor2 = "";
   while (ss.available())
   {
    gps.encode(ss.read());
   }
  //if(1)
   if (gps.location.isValid())
   {
   float longi = gps.location.lng();
   Coor2 = String(longi,6);
  }
  else
  {
    Coor2 = String(0.00);
  } 
  return Coor2;
  }
