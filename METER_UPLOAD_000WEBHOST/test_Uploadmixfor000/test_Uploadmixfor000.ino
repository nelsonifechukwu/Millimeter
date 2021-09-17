  
#include <SoftwareSerial.h>
 
//SIM808 TX is connected to Arduino D8
#define SIM808_TX_PIN 10
 #define BATTERY A3
//SIM808 RX is connected to Arduino D7
#define SIM808_RX_PIN 11

 // Set the GPRS Access Point Name (APN) to suit your SIM card.
#define AccessPointName "airtelgprs.com"
// Set the address of the server where the measurements should be reported.
#define HTTPserver "http://meterproject.000webhostapp.com"
// Set the URL of the program that receives the measurements.
#define HTTPurl "/newdata.php?"
//Create software serial object to communicate with SIM808
SoftwareSerial GPRS(SIM808_TX_PIN,SIM808_RX_PIN);


void gsm_disConnect_gprs(){
  GPRS.write("AT+CGATT=0\r\n"); // Attach to GPRS
  delay(2000);
  Serial.println("GPRS off");
}
  
void gsm_connect_gprs(){
GPRS.print("AT+CGATT=1\r\n");
  delay(1000);
  GPRS.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
  delay(1000);
  GPRS.print("AT+SAPBR=3,1,\"APN\",\"");
  GPRS.print(AccessPointName);
  GPRS.print("\"\r\n");
  delay(1000);
  GPRS.print("AT+SAPBR=1,1\r\n");
  delay(3000);
}


// Function that sends temperature and humidity to my server.
void gsm_send_data(char * value)
{
    Serial.println("Sending data.");    
    GPRS.print("AT+HTTPINIT\r\n");  // Initialize HTTP
    //Serial.print("AT+HTTPINIT\r\n");
    delay(1000);
    GPRS.print("AT+HTTPPARA=\"URL\",\"");
    GPRS.print(HTTPserver);
    GPRS.print(HTTPurl);
    
    delay(50);
    GPRS.print(value); 
    //Serial.print("&value=");
    delay(50);
    GPRS.write("\"\r\n");   // close url
   
    //Serial.print("\"\r\n");
    delay(2000);
    GPRS.print("AT+HTTPPARA=\"CID\",1\r\n");    // End the PARA
    //Serial.print("AT+HTTPPARA=\"CID\",1\r\n");
    delay(2000);
    GPRS.print("AT+HTTPACTION=0\r\n");
    //Serial.print("AT+HTTPACTION=0\r\n");
    delay(3000);    
    GPRS.print("AT+HTTPTERM\r\n");
    //Serial.print("AT+HTTPTERM\r\n");
    //Serial.println();
    delay(3000);    
    Serial.print("data sent complete : ");
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
   
  //Being serial communication witj Arduino and SIM808
  GPRS.begin(9600);
  delay(1000);
   
  Serial.println("Setup Complete!");

  GPRS.write("AT+CREG?\r\n");
  delay(150);
 gsm_connect_gprs();

}

 float battery_voltage() // Read the battery voltage
{
  float voltage = 0;
  for (int i=0; i<30; i++)
  {
    int batteryValue = analogRead(BATTERY);
    voltage += batteryValue * (16.128 / 1023.0); // This formula must be adjusted to suit the Vcc voltage
    delay(2000);                                 // and the avlues of the resistors used for measurement
  }                                              // Should be 16.0/1023.0 for Vcc=4v and perfectly equal resistors.
  return voltage/30; // To smooth out any noise, 30 samples are taken and the average is returned.
}

void loop() {
 char buf[20];

  float bv = battery_voltage();            // Read battery voltage and convert to string "voltage=nn.n"
  dtostrf(bv, 3, 1, buf);
  String message = String("voltage=");
  message +=buf;                    
  message.toCharArray(buf, 20);     
  gsm_send_data(buf);              
delay(1000); 
}

