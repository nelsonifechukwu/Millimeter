


//DEFINE YOUR RELAY, CURRENT SENSOR DATA PINS, LOW BALANCE, RETRIEVE DATA, THEFT



#include <WiFi.h>
#include <HTTPClient.h>



//#include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
// define the number of bytes you want to access
#define EEPROM_SIZE 2
//Libraries for SD card
#include <SPI.h>
#include "FS.h"
#include "SD.h"

#define SD_CS 5
String dataMessage;

//WIFI parameters
const char* ssid = "***";
const char* password = "***";

//For current Sensor
#include "ACS712.h"
float error = 0.05;   // initial error of ACS712 sensor
ACS712 ac1(A0, error);
double current = 0L;
double power = 0L;
double load = 0L;
float balance = 0.00f;
float unit = 0.00f;
float unitPrice = 5.00f;
long lastPrint = millis();
long lastDtl = millis();

//LCD initialization
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName1 = "***/sensor_post.php";
const char* serverName2 = "***/sensor_post2.php?id=1";

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key
String apiKeyValue = "tPmAT5Ab3j7F9"; 
String outputsState;
///////////////////////////for relay
int relay = 8;
bool alert = true;
bool relaystate = 1;
//////////////////////////////

void setup() {
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.begin(115200);
  lcd.begin();
  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  //restore data from arduino's Static memory after reboot/ power failure
  reStoreInfo();
  printInfo();

  //SDCARD initializations
  SD.begin(SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }

  File file = SD.open("/data.txt");
  if (!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Balance, Unit, Current, Power \r\n");
  }
  else {
    Serial.println("File already exists");
  }
  file.close();

  //WIFI initializations

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //check for client connection
 /* WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(serverName1, httpPort))
  {
    Serial.println("connection failedjjjjj");
    digitalWrite(relay, LOW);
    return;
  }
*/
  if (balance > 1)
  {
    digitalWrite(relay, relaystate);
    currentCalculation();
    storeInfo();
    printDetails();
    printBalance();
    checkLowBalance();
    send_alldata();
    delay(3000);
    outputsState = httpGETRequest(serverName2);
    parse_payload();
    logSDCard();

  }
  else {

    digitalWrite(relay, LOW);
    lcd.clear();
    lcd.print("Low Balance");
    lcd.setCursor(0, 1);
    lcd.print("Please Recharge");
    delay(500);
  }

  checkLowBalance();
  Serial.flush();

}





String httpGETRequest(const char* serverName) {

  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void parse_payload() {
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(outputsState);
  int id = root["id"]; // 1
  const char* state = root["status"];
  Serial.print("ID:");
  Serial.println(id);
  Serial.print("Status:");
  Serial.println(state);
  String led = root["status"];

  if (led == "ON" ) {
    relaystate = 1;
    digitalWrite(relay, relaystate);
  }
  else if (led == "OFF" ) {
   relaystate = 0;
   digitalWrite(relay, relaystate);
   
  }
}



void send_alldata() {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName1);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&balance=" + String(balance)
                             + "&unit=" + String(unit) + "&current=" + String(current) + "&power=" + String(power) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);



    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }


}


void checkLowBalance()
{
  if (balance < 20 && alert)    //notify customer about low balance
  {
    //Take an action


    alert = false;
    // cashbalance = 0;
  }
}

//PrintBalance
void printBalance()
{
  if ((millis() - lastPrint) > 5000) //print balance and usage every 5 seconds without using delay()
  {
    printInfo();
    delay(1000);
    lastPrint = millis();
  }
}

//Calculate Current and other parameters
void currentCalculation()
{
  //IMPORTED SOME CORRECTIONS
  current = 0.66 ; //power calculations
  power = 2.00 * current;
  load = (power / (10.0 * 36.0)); // instantanious load
  unit += load;                    //Cumulated load or power consumption
  balance -= load * unitPrice;     // balance adjustment
}

//printing...
void printInfo() {

  lcd.clear();
  lcd.print("Balance:");
  lcd.print(balance, 2);
  lcd.print("NGN");
  lcd.setCursor(0, 1);
  lcd.print("Use:");
  lcd.print(unit, 4);
  lcd.print(" KWtt");
  delay(100);
}
void printDetails()
{

  lcd.clear();
  lcd.print("Current:");
  lcd.print(current, 3);
  lcd.print("Amps");
  lcd.setCursor(0, 1);
  lcd.print("Power:");
  lcd.print(power);
  lcd.print(" Watt");
  delay(1000);

}

//WRITE TO EEPROM
void storeInfo()
{
  //  EEPROM[100]=unit;
  //  EEPROM[0]=balance;

  EEPROM.write(0, unit);
  EEPROM.write(1, balance);
  EEPROM.commit();

}

//READ FROM EEPROM

void reStoreInfo()
{
  //  unit= EEPROM[100];
  //  balance= EEPROM[0];

  unit =  EEPROM.read(0);
  balance = EEPROM.read(1);

}

//Write to SDCARD
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

//Write INFO to SDCARD
void logSDCard() {
  dataMessage = String(balance) + "," + String(unit) + "," + String(current) + "," +
                String(power) + "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str());
}

//Append to SDCARD
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
