#include <Adafruit_Sensor.h>



#include <DHT.h>
#include <DHT_U.h>



#include <SoftwareSerial.h>
//SoftwareSerial Serial1(10,11);

#include <String.h>


#define DHTPIN A0

DHT dht(DHTPIN, DHT11);
 
void setup()
{
  Serial1.begin(9600);               // the Serial1 baud rate   
  Serial.begin(9600);    // the Serial1 baud rate 
  dht.begin();

  delay(1000);
}
 
void loop()
{
      float h = dht.readHumidity();
      float t = dht.readTemperature(); 
      delay(100);   
         
      Serial.print("Temperature = ");
      Serial.print(t);
      Serial.println(" °C");
      Serial.print("Humidity = ");
      Serial.print(h);
      Serial.println(" %");    
      
   
  if (Serial1.available())
    Serial.write(Serial1.read());

  Serial1.println("AT");
  delay(1000);

  Serial1.println("AT+CPIN?");
  delay(1000);

  Serial1.println("AT+CREG?");
  delay(1000);

  Serial1.println("AT+CGATT?");
  delay(1000);

  Serial1.println("AT+CIPSHUT");
  delay(1000);

  Serial1.println("AT+CIPSTATUS");
  delay(2000);

  Serial1.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  Serial1.println("AT+CSTT=\"airtelgprs.com\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  Serial1.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  Serial1.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  Serial1.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  Serial1.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  Serial1.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=IG4Y682D64JBRP3F&field1=" + String(100) +"&field2="+String(100);
  Serial.println(str);
  Serial1.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();

  Serial1.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  Serial1.println();
 
  ShowSerialData();
 
  Serial1.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 
void ShowSerialData()
{
  while(Serial1.available()!=0)
  Serial.write(Serial1.read());
  delay(5000); 
  
}
