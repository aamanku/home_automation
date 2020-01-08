#include <SPI.h>  //for rtc
#include <DS3231.h>  //for rtc

#include <LiquidCrystal.h>
DS3231 rtc(SDA,SCL);  //pointing to pins on uno

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //LiquidCrystal lcd(rs, en, d4, d5, d6, d7)  const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

//--------------------------------------------------------------------------------------------

  float energy=0;
  float power=0;
  long int rawcursum;
  float current=0;
  long int rawvolsum;
  float voltage=0;
  volatile long int currenttime=0;
  volatile long int lasttime=0;
  int nsample=0;
//  char line[8];
//  String fl_v[7];
//  String fl_i[7];
//  String fl_p[7];
//  String fl_e[7];
  
  #define rawini 506
  
//-----------------------------------------------------------------------------------------------

void setup() {
//  Serial.begin(9600);
  rtc.begin();  //initialize real time clock
  lcd.begin(16, 2); //initializing lcd
  lcd.setCursor(6,0);
  lcd.print("AMK");
  //-------------------------------------------------------------------------------------------------------------------------------
  lcd.setCursor(2,1);
  lcd.print("Energy Meter");


  lasttime=rtc.getUnixTime(rtc.getTime());
}

void loop() {

//---------------------------------------------------------------------------------
  rawcursum=0;
  rawvolsum=0;
  nsample=0;
  while((rtc.getUnixTime(rtc.getTime())-lasttime)<=10)
  {
    rawcursum += analogRead(A2);
    rawvolsum += analogRead(A1);
    nsample++;
    }
  current=abs(0.22786*((rawcursum/nsample)-rawini));
  voltage=0.020699*(rawvolsum/nsample);
  currenttime=rtc.getUnixTime(rtc.getTime());
  power=voltage*current;
  energy+=voltage*current*(currenttime-lasttime)*2.7777*pow(10,-7);
  
//  Serial.println((currenttime-lasttime));
  lasttime=rtc.getUnixTime(rtc.getTime());
// ---------------------------------------------------------------------------------
//  Serial.println(voltage);
//  Serial.println(current);
//  Serial.println(energy);
//  Serial.println(power);
  lcd.clear();
//  dtostrf(voltage,4,1,fl_v);
//  dtostrf(current,4,1,fl_i);
//  dtostrf(power,4,1,fl_p);
//  dtostrf(energy,4,3,fl_e);
lcd.setCursor(0,0);
lcd.print("V=");
lcd.print(voltage,1);

lcd.setCursor(8,0);

lcd.print("I=");
lcd.print(current,1);

lcd.setCursor(0,1);

lcd.print("P=");
lcd.print(power,1);

lcd.setCursor(8,1);

lcd.print("E=");
lcd.print(energy,3);




}
