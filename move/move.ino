#include<Wire.h>
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClient.h>
#include <Adafruit_Sensor.h>    // Adafruit  sensor library
#include <Adafruit_ADXL345_U.h> 

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

#define BLYNK_TEMPLATE_ID "TMPLys5_ytct"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "pV9UC8akeogAXjUQMXOM7cgT20Xxe3B6"

char url[] = "http://maker.ifttt.com/trigger/theft/json/with/key/dc7nk9hR4mIbsvkkYF77Y3"; 
char auth[] = "pV9UC8akeogAXjUQMXOM7cgT20Xxe3B6";       // You should get Auth Token in the Blynk App.
char ssid[] = "..";                       // Your WiFi credentials.
char pass[] = "deoa zabe na";
 
const int adxl345=0x53;
double AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ,roll,pitch,rollF = 0,pitchF = 0;
 
int minVal=265;
int maxVal=402;
 
double x;
double y;
double z;
 
void setup(){
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(adxl345);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
  Blynk.begin(auth, ssid, pass);
  //Off-set Calibration
  //X-axis
  Wire.beginTransmission(adxl345);
  Wire.write(0x1E);
  Wire.write(1);
  Wire.endTransmission();
  delay(10);
  //Y-axis
  Wire.beginTransmission(adxl345);
  Wire.write(0x1F);
  Wire.write(-2);
  Wire.endTransmission();
  delay(10);

  //Z-axis
  Wire.beginTransmission(adxl345);
  Wire.write(0x20);
  Wire.write(-9);
  Wire.endTransmission();
  delay(10);
 }
void loop(){
  Blynk.run();

  Wire.beginTransmission(adxl345);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(adxl345,6,true);
  AcX=(Wire.read()|Wire.read()<<8);
  AcX = AcX/256;
  AcY=(Wire.read()|Wire.read()<<8);
  AcY = AcY/256;
  AcZ=(Wire.read()|Wire.read()<<8);
  AcZ = AcZ/256;
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);

  roll = atan(AcY / sqrt(pow(AcX, 2) + pow(AcZ, 2))) * 180 / PI;
  pitch = atan(AcX/ sqrt(pow(AcY, 2) + pow(AcZ, 2))) * 180 / PI;

  rollF = 0.94 * roll + 0.06 * roll;
  pitchF = 0.94 * pitch + 0.06 * pitch;
   
  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  Serial.print("Roll= ");
  Serial.println(roll);

  Serial.print("pitch= ");
  Serial.println(pitch);

//  rollF = roll, pitchF = pitch;

  

//  Serial.print("x= ");
//  Serial.println(AcX);
//
//  Serial.print("y= ");
//  Serial.println(AcY);
//
//  Serial.print("z= ");
//  Serial.println(AcZ);

//  sensors_event_t event;
// accel.getEvent(&event);
// Serial.print("Xacc: ");
// Serial.print(event.acceleration.x);
// Serial.print("  ");
//  Serial.print("Yacc: ");
// Serial.print(event.acceleration.y);  
// Serial.print("  ");
//  Serial.print("Zacc: ");
// Serial.print(event.acceleration.z);
// Serial.print("  ");
// Serial.println("m/s^2 ");
   
//  Serial.print("AngleX= ");
//  Serial.println(x);
//   
//  Serial.print("AngleY= ");
//  Serial.println(y);
//   
//  Serial.print("AngleZ= ");
//  Serial.println(z);
//  Serial.println("-----------------------------------------");
   
  Blynk.virtualWrite(V2, roll);
  Blynk.virtualWrite(V4, pitch);
//  Blynk.virtualWrite(V6, z);
   
  if(1){
    if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient wf;
    http.begin(wf,url);               // Specify request destination
        int httpCode = http.GET();          // Send the request
        Serial.println("\n\n\nSecurity Alert!!!\n\n\n");   

        if (httpCode > 0) {
          //Check the returning code

          String payload = http.getString();   // Get the request response payload
          Serial.println(payload);             // Print the response payload

        }

        http.end();               
  }
  delay(1000);
  }
  rollF = roll, pitchF = pitch;
}
