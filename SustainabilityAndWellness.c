#include <Adafruit_ESP8266.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#define DEBUG true
#include <dht.h>  // Include library
#define outPin 3  // Defines pin number to which the sensor is connected

dht DHT;      // Creates a DHT object
int bpm = 0;                
int led_pin = 13;                 

#define SSID "Rajhari"     // "SSID-WiFiname"
#define PASS "hari12345" // "password"
#define IP "184.106.153.149"      // thingspeak.com ip

String msg = "GET /update?key=FIQ91UC4ONBY733J"; 

SoftwareSerial esp8266(7,8);


String tempC;

String feran;

int error;

void setup()

{

  Serial.begin(115200); //or use default 115200.
  esp8266.begin(115200);
  
   pinMode(led_pin,OUTPUT);



  Serial.begin(115200);           

  interruptSetup();  
  Serial.println("AT");
  esp8266.println("AT");

  delay(5000);

  if(esp8266.find("OK"))
  {
    connectWiFi();
  }
}

void loop()
{
 
  //Read temperature and humidity values from DHT sensor:
  start: //label
    error=0;
    
   int readData = DHT.read11(outPin);
heart_rate= analogread(bpm);
  float t = DHT.temperature;  // Read temperature
  
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print("°C | ");
Serial.print("BPM: ");
Serial.println(heart_rate);
delay(200); //  take a break

 
 
     
    char buffer[10];
    // there is a useful c function called dtostrf() which will convert a float to a char array
    //so it can then be printed easily.  The format is: dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf);
      
    feran = dtostrf(t, 4, 1, buffer);
    
    tempC = dtostrf(heart_rate, 4, 1, buffer);
    
    updateTemp();
    //Resend if transmission is not completed
    if (error==1)
    {
      goto start; //go to label "start"
    } 
  delay(3600);
}

void updateTemp()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(2000);
  if(esp8266.find("Error"))
  {
    return;
  }
  cmd = msg ;
  cmd += "&field1=";    //field 1 for temperature
  cmd += feran;
  cmd += "&field2=";    //field 2 for pulse rate
  cmd += tempC;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
  if(esp8266.find(">"))
  {
    Serial.print(cmd);
    esp8266.print(cmd);
  }
  else
  {
    Serial.println("AT+CIPCLOSE");
    esp8266.println("AT+CIPCLOSE");
    //Resend...
    error=1;
  }
}

boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1");
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(5000);
  if(esp8266.find("OK"))
  {
    return true;
  }
  else
  {
    return false;
  }
}
