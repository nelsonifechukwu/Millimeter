
/*#include <SoftwareSerial.h>
 * 
This would have been used alongside defining an instance of
SofwareSerial Serial1(10,11) but due to usage of that port
another rx, tx has to be found. You can't just
communicate with any if found using the above instance
you have to change it to the original named instance
of the rx and tx pin to be used for communication
e.g on arduino mega, pin 18 and 19 serves as rx1 and tx1 
respectively with their origianl instance as Serial1.
So therfore to communicate with through that port
initiate Serial1.begin(baudrate) where Serial1 replaces
Serial1;
*/
#include <SPI.h>
#include <SD.h>
#include<EEPROM.h>

//Used for GSM module

//Using Arduino Mega, search for rx pins that supports change interrupts
//SoftwareSerial Serial1(14, 15);
unsigned int temp=0,i=0,x=0,k=0;
char str[200],flag1=0,flag2=0;
String bal="";
String initSms ="AT+CMGS=\"+**\"";  //the number to send SMS
String BalanceSms = "This is an SMS from Smart Energy Meter & your balance is ";

//Serial1 NETWORK INFO IMPORTED
//watch when replacing Serial1 with serial1
#define AccessPointName "airtelgprs.com"
// Set the address of the server where the measurements should be reported.
#define HTTPserver "http://neilz.000webhostapp.com"
// Set the URL of the program that receives the measurements.
#define HTTPurl "/newdata.php?"

//DECLARATION FOR THEFT DETECTION
//int irled = 6;
//int irledport = 3;


//FLAGS
int theft_flag = 0;
int cashbalance = 1; /*
controls the load based on availability of balance
*/
 /* transfers data after successful 
connection of GSM/Serial1 module
*/

//Used for LCD Screen
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//instance for SDcard file
File myFile;
//For current Sensor
/*#include "ACS712.h"
float error= 0.05;    // initial error of ACS712 sensor
 ACS712 ac1(A0, error); */

 //IMPORTED SOME CORRECTIONS
float current=0.00f;
float power=0.00f;
double load=0L;
float balance=0.00f;
float unit =0.00f;
float unitPrice =5.00f;
long lastPrint = millis();
long lastDtl = millis();

///////////////////////////for relay
int relay = 8;
bool alert=true;

//////////////////////////////
//Timer variables

unsigned long previoustimesd= 0;
unsigned long previoustimegsm = 0;
unsigned long previoustimethingspeak = 0;




  

void setup(){
  
   
  Serial.begin(9600);

  
  pinMode(relay, OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(3, INPUT);
  digitalWrite(6,HIGH);
  
  digitalWrite(relay, LOW);
  //Initialization of Sdcard
    Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    Serial.println("CONNECT SDCARD TO INITIATE METER");
    /*DONT BEGIN METER IF SD CARD IS'NT IN PLACE
    LOOP IS ONLY RUN ONCE, same technique for 
    not rinnu
    */
    return;
  }
  Serial.println("initialization done.");
  
  myFile = SD.open("SMeter.txt", FILE_WRITE);
 //Write the headings to the SDcard
  if(myFile){
  myFile.println("Voltage,  Current,  Power,  Balance,  Unit");
  }
  
  myFile.close();

  //End of Writing to SDCARD
  
  
  Serial1.begin(9600);
  
  lcd.begin();
  
  
  
  attachInterrupt(1, theft, FALLING);
  reStoreInfo();   //restore data from arduino's Static memory after reboot/ power failure
  printInfo();
  gsm_init();
  Serial1.println("AT+CNMI=2,2,0,0,0"); 
  gsm_connect_Serial1();
 

}


void loop(){
  
  //Stops flow of electricity once theft is ddetected
  /*this return scheme stops all execution once
  the below condition is true check arduino.cc for more
   if (condition){
   do a particular thing then
   return;
   The above doesnt execute the other blocks of code below
   it when placed first in loop  
   }*/ 


//Once theft is detected, trigger this
  if(theft_flag) 
 {
  //Serial.println("THEFT!!");
  lcd.clear();
 
  lcd.setCursor(5,0);
  lcd.print("THEFT!!");
  
  lcd.setCursor(2, 1);
  lcd.print("CASE TAMPERED");
  digitalWrite(relay, LOW);
  delay(1000);
  
 // upload data to thingspeak
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
  delay(4000); // large time is required to communicate
  //remotely and send data to server
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=IG4Y682D64JBRP3F&field3=" + String(1);
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
  return;
  
  } 



//if(cashbalance){
if(balance > 1) 
{ 
  digitalWrite(relay,HIGH);
  currentCalculation();
  storeInfo();
  printDetails();
  printBalance();
  checkLowBalance();
  checkSMS();
  //digitalWrite(relay,HIGH);
  send_alldata();
  
  

  }
  else{
    
      digitalWrite(relay,LOW);
      lcd.clear();
      lcd.print("Low Balance");
      lcd.setCursor(0,1);
      lcd.print("Please Recharge");
      delay(500);
    }
    
checkLowBalance();
checkSMS();

Serial.flush();
Serial1.flush();


}
void send_alldata(){
  
  unsigned long currenttime = millis();
  if(currenttime - previoustimesd > 20000){
   storein_sdcard();
  previoustimesd = currenttime;
  }

   
  if(currenttime - previoustimegsm > 50000){
   char buf[20];
    float bv = 150.00;
  // Read battery voltage and convert to string "voltage=nn.n"
  dtostrf(bv, 3, 1, buf);
  String message = String("voltage=");
  message +=buf;                    
  message.toCharArray(buf, 20); 
  
//IMPORTED TECHNIQUE TO CONVERT TO CHAR
float cu = current; 
char bua[20];
    dtostrf(cu, 3, 1, bua);
  String cur = String("current=");
  cur +=bua;                    
  cur.toCharArray(bua, 20); 

 float po = power; 
char bub[20];
    dtostrf(po, 3, 1, bub);
  String powe = String("power=");
  powe +=bub;                    
  powe.toCharArray(bub, 20); 

float ba = balance;  
char buc[20];
    dtostrf(ba, 3, 1, buc);
  String bala = String("balance=");
  bala +=buc;                    
  bala.toCharArray(buc, 20); 


float un = unit; 
char bud[20];
    dtostrf(un, 3, 1, bud);
  String uni = String("unit=");
  uni +=bud;                    
  uni.toCharArray(bud, 20); 
  
    
  gsm_send_data(buf, bua, bub, buc, bud);    
  previoustimegsm = currenttime;
  

  }

    if(currenttime - previoustimethingspeak >60000){
  thingspeak_send(); 
  previoustimethingspeak = currenttime;
  }

  }
void gsm_connect_Serial1(){
Serial1.print("AT+CGATT=1\r\n");
  delay(1000);
  Serial1.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
  delay(1000);
  Serial1.print("AT+SAPBR=3,1,\"APN\",\"");
  Serial1.print(AccessPointName);
  Serial1.print("\"\r\n");
  delay(1000);
  Serial1.print("AT+SAPBR=1,1\r\n");
  delay(2000);
}

//Serial1 SEND FUNCTION IMPORTED
void gsm_send_data(char * value, char * current1, char * power1, char * balance1, char * unit1)
{   //decrease time for speedy upload
    
    Serial.println("Sending data.");    
    Serial1.print("AT+HTTPINIT\r\n");  // Initialize HTTP
    //Serial.print("AT+HTTPINIT\r\n");
    delay(2000);
    Serial1.print("AT+HTTPPARA=\"URL\",\"");
    Serial1.print(HTTPserver);
    Serial1.print(HTTPurl);

    delay(50);
    Serial1.print("voltage="); 
    delay(50);
    Serial1.print(value); 
    delay(50);
    Serial1.print("&current1="); 
    delay(50);
    Serial1.print(current1); 
     delay(50);
    Serial1.print("&power1=");
    delay(50);
    Serial1.print(power1); 
     delay(50);
    Serial1.print("&balance1=");
    delay(50);
    Serial1.print(balance1); 
     delay(50);
    Serial1.print("&unit1="); 
    delay(50);
    Serial1.print(unit1); 
    delay(50); 
    Serial1.write("\"\r\n");   // close url
    delay(1500);
    Serial1.print("AT+HTTPPARA=\"CID\",1\r\n");    // End the PARA
    delay(2000);
    Serial1.print("AT+HTTPACTION=0\r\n");
    //Serial.print("AT+HTTPACTION=0\r\n");
    delay(2000);    
    Serial1.print("AT+HTTPTERM\r\n");
    //Serial.print("AT+HTTPTERM\r\n");
    //Serial.println();
    delay(2000);    
    Serial.print("data sent complete : ");
    delay(2000);
}



 void storein_sdcard(){ 
  
   myFile = SD.open("SMeter.txt", FILE_WRITE);
 //Write the headings to the SDcard
  if(myFile){
 //  myFile.println("Voltage, Current, Power, Balance, Unit");
    currentCalculation();
    
    Serial.print("Writing to SMeter.txt...");
    //Print Info too textfile after every 5secs
    myFile.print("150");
    myFile.print("/");
    myFile.print(current);
    myFile.print("/");
    myFile.print(power);
    myFile.print("/");
    myFile.print(balance);
    myFile.print("/");
    myFile.print(unit);
    myFile.println("/"); 
   
    myFile.close(); //closes file
  }
  
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
    
  }
void storeInfo()
{
//  EEPROM[100]=unit;
//  EEPROM[0]=balance;
    
  EEPROM.put(0,unit);
  EEPROM.put(100,balance);
  
}

void reStoreInfo()
{
//  unit= EEPROM[100];
//  balance= EEPROM[0];
    
  EEPROM.get(0,unit);
  EEPROM.get(100,balance);
  
}

void checkLowBalance()
{
  if(balance < 20 && alert)     //notify customer about low balance
  {
      sendSms("Your Balance is Below 20NGN, Please recharge to avoid disconnection");
      alert = false;
     // cashbalance = 0;
  }
}

void printBalance()
{
  if((millis() - lastPrint)>5000) //print balance and usage every 5 seconds without using delay()
    {
      printInfo();
      delay(1000);
      lastPrint = millis();
    }
  }

void currentCalculation()
{
  //IMPORTED SOME CORRECTIONS
   current =0.66 ; //power calculations
   power=2.00*current;
   load =(power/(10.0*36.0)); // instantanious load
   unit+=load;                      //Cumulated load or power consumption 
   balance -=load*unitPrice;        // balance adjustment  
  }

//printing...
void printInfo(){

  lcd.clear();
  lcd.print("Balance:");
  lcd.print(balance,2);
  lcd.print("NGN");
  lcd.setCursor(0,1); 
  lcd.print("Use:");
  lcd.print(unit,4);
  lcd.print(" KWtt");
  delay(100);
}
void printDetails()
{

  lcd.clear();
  lcd.print("Current:");
  lcd.print(current,3);
  lcd.print("Amps");
  lcd.setCursor(0,1); 
  lcd.print("Power:");
  lcd.print(power);
  lcd.print(" Watt");
  delay(1000);
  
  }







/////////////////////////////////////////////////
//All the methods for GSM and SMS System
/////////////////////////////////////////////////
void gsm_init()
{
  lcd.clear();
  lcd.print("Finding GSM/GPRS");
  Serial.println("Finding GSM/GPRS..");
  boolean at_flag=1;
  while(at_flag)
  {
    Serial1.println("AT");
    while(Serial1.available()>0)
    {
      if(Serial1.find("OK"))
      at_flag=0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Module Connected..");
  Serial.println("Module Connected..");
  delay(1000);
  lcd.clear();
  lcd.print("Disabling ECHO");
  Serial.print("Disabling ECHO");
  boolean echo_flag=1;
  while(echo_flag)
  {
    Serial1.println("ATE0");
    while(Serial1.available()>0)
    {
      if(Serial1.find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Echo OFF");
  Serial.print("\tEcho OFF");
  delay(1000);
  lcd.clear();
  lcd.print("Finding Network..");
  Serial.println("\t Finding Network..");
  boolean net_flag=1;
  while(net_flag)
  {
    Serial1.println("AT+CPIN?");
    while(Serial1.available()>0)
    {
      if(Serial1.find("+CPIN: READY"))
      net_flag=0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Network Found..");
  Serial.println("Network Found..");
  delay(1000);
  lcd.clear();

}
void sendSms(String text){

  init_sms();
  Serial.println("Sending:");
  send_data(text);
  send_sms();
}

void init_sms()
{
   Serial1.println("AT+CMGF=1");
   delay(200);
   Serial1.println(initSms);
   
   delay(200);
}

void send_sms()
{
  Serial1.write(26);
  message_sent();
}

void send_data(String message)
{
  Serial1.println(message);
  Serial.println(message);
  delay(200);
}

void message_sent()
{
  lcd.clear();
  lcd.print("Message Sent.");
  Serial.println("Message Sent.");
  delay(1000);
  //Serial1.flush();
}

void checkSMS(){
    GPRSEvent();
    balanceCheck();
      if(temp==1)
      {
       decode_message();
       send_confirmation_sms();
      }
  }

void GPRSEvent()
{
  while(Serial1.available())
  {
    char ch=(char)Serial1.read();
      str[i++]=ch;
    
    if(ch == '*')
    {
      temp=1;
      lcd.clear();
      lcd.print("Message Received");
      Serial.print("Message Received:\t");
//      Serial.println(str);
      delay(500);
      break;
    }
  }
}

void decode_message()
{
    x=0,k=0,temp=0;
    while(str[x]!='#')
    {
      x++;
     }
      x++;
      bal="";
      while(str[x]!='*')
      {
        bal+=str[x];
        x++;
        }
        Serial.println(bal.toInt());
      
}

void send_confirmation_sms()
{
    int recharge_amount=bal.toInt();
    balance+=recharge_amount;
    lcd.clear();
    lcd.print("Energy Meter ");
    lcd.setCursor(0,1);
    lcd.print("Recharged:");

    //FLAGS BEWARE
    alert = true; // resets the flag for the system to
    //check for balance again
    //cashbalance = 1;// resets flag for running of the 
    //whole major system in the loop
    lcd.print(recharge_amount);
    delay(2000);
    String text1="Energy meter recharged NGN:";
    text1+=recharge_amount;
    String text2= text1+". Total Balance "+(String)balance;
    sendSms(text2);
    Serial.print(    "           SENT:"  );
    Serial.print(text1);
    Serial.println(text2);
    temp=0;
    i=0;
    x=0;
    k=0;
    delay(1000);
}

void balanceCheck(){
    
    x=0,k=0;
    String Stringcheck = "";
    while(x<i)
    {
      Stringcheck+=str[x];
      x++;
      }
//      Serial.println("Checking");
//      Serial.println(Stringcheck);
      if(Stringcheck.indexOf("Balance?")>0)
      {
        sendSms("Total Balance "+(String)balance);
        for(int c=0;c<i;c++)
        {
            str[c]='a';
          }
        }
  }
//
  
  void thingspeak_send(){
   
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
  delay(4000); // large time is required to communicate
  //remotely and send data to server
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=IG4Y682D64JBRP3F&field1=" + String(balance) +"&field2="+String(unit);
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
  delay(3000); 
  
} 

 void theft(){
  digitalWrite(relay, LOW);
  theft_flag = 1; 
  
  } 
