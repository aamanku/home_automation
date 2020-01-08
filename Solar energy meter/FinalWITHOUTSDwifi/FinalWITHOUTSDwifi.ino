//#include <SPI.h>  //for rtc
//#include <DS3231.h>  //for rtc

//#include <LiquidCrystal.h>
//DS3231 rtc(SDA,SCL);  //pointing to pins on uno

//LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //LiquidCrystal lcd(rs, en, d4, d5, d6, d7)  const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

//--------------------------------------------------------------------------------------------

  float energy=0;
  float power=0;
  long int rawcursum;
  float current=0;
  long int rawvolsum;
  float voltage=0;
  volatile long int currenttime=0;
  volatile long int lasttime=0;
  volatile long int lasttimewifi=0;
  int nsample=0;
//  char line[8];
//  String fl_v[7];
//  String fl_i[7];
//  String fl_p[7];
//  String fl_e[7];
  #define DEBUG true
  String mySSID = "ssid";       // WiFi SSID
  String myPWD = "password"; // WiFi Password
  String myAPI = "apikey";   // API Key
  String myHOST = "api.thingspeak.com";
  String myPORT = "80";
  String myFIELD1 = "field1"; 
  String myFIELD2 = "field2";
  String myFIELD3 = "field3";
  String myFIELD4 = "field4";
  
  #define rawini 506 //calibration
  
//-----------------------------------------------------------------------------------------------

void setup() {
  while(analogRead(A1)<=400)
  {}
  Serial.begin(115200);
  Serial1.begin(115200);
  //rtc.begin();  //initialize real time clock
 // lcd.begin(16, 2); //initializing lcd
  
  Serial.println("Initialization done.");
  //-------------------------------------------------------------------------------------------------------------------------------

  //lcd.print("Starting Loop");
  delay(1000);
  //millis();
  //lcd.clear();
  lasttime=millis();
  lasttimewifi=lasttime;
}

void loop() {

//---------------------------------------------------------------------------------
  rawcursum=0;
  rawvolsum=0;
  nsample=0;
  while(millis()-lasttime<=2000)
  {
    rawcursum += analogRead(A2);
    rawvolsum += analogRead(A1);
    nsample++;
    }
  current=abs(0.22786*((rawcursum/nsample)-rawini));
  voltage=0.020699*(rawvolsum/nsample);
  currenttime=millis();
  power=voltage*current;
  energy+=voltage*current*(currenttime-lasttime)*2.7777*pow(10,-7);
  
  Serial.println((currenttime-lasttime));
  lasttime=millis();
// ---------------------------------------------------------------------------------
  Serial.println(voltage);
  Serial.println(current);
  Serial.println(energy);
  Serial.println(power);
 // lcd.clear();
//  dtostrf(voltage,4,1,fl_v);
//  dtostrf(current,4,1,fl_i);
//  dtostrf(power,4,1,fl_p);
//  dtostrf(energy,4,3,fl_e);
//lcd.setCursor(0,0);
//lcd.print("V=");
//lcd.print(voltage,1);
//
//lcd.setCursor(8,0);
//
//lcd.print("I=");
//lcd.print(current,1);
//
//lcd.setCursor(0,1);
//
//lcd.print("P=");
//lcd.print(power,1);
//
//lcd.setCursor(8,1);
//
//lcd.print("E=");
//lcd.print(energy,3);

//while((rtc.getUnixTime(rtc.getTime())-lasttime)<=10)
  if(millis()-lasttimewifi<=15000){
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  while(!Serial1.find("OK")||!Serial1.find("ERROR")) 
  {          
      Serial.print("*");
  }
  if(Serial1.find("OK")){
  Serial.println("Connected");
  
    Serial.println("Sending");
    
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD1+"="+String(voltage)+"&"+ myFIELD2 +"="+String(current)+ myFIELD3 +"="+String(power)+ myFIELD4 +"="+String(energy);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    Serial1.find(">"); 
    Serial.println(sendData);
    //Serial.print("Value to be sent: ");
    //Serial.println(sendVal);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    espData("AT+CWQAP", 1000, DEBUG);
  }
  else{
    Serial.print("Not Connected");
    delay(1000);
    }

    lasttimewifi=millis();


    
    


}
}




String espData(String command, const int timeout, boolean debug)
{
  //Serial.print("AT Command ==> ");
  //Serial.print(command);
  //Serial.println("     ");
  
  String response = "";
  Serial1.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
