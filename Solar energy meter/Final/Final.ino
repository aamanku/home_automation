#include <SPI.h>  //for rtc
#include <DS3231.h>  //for rtc
#include <SdFat.h>   //improved stock sd lib
#include <LiquidCrystal.h>
DS3231 rtc(SDA,SCL);  //pointing to pins on uno
SdFat sd; // defining SdFat obj
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //LiquidCrystal lcd(rs, en, d4, d5, d6, d7)  const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
File myFile;
//--------------------------------------------------------------------------------------------
short int x=1;
  short int y=0;
  short int z=0;
  char buff;
  String num="";
  float energy=0;
  long int rawcursum;
  float current=0;
  long int rawvolsum;
  float voltage=0;
  volatile long int currenttime=0;
  volatile long int lasttime=0;
  #define rawini 506
  
//-----------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  rtc.begin();  //initialize real time clock
  lcd.begin(16, 2); //initializing lcd
  while(!Serial){}  //Wait for serial
  Serial.println("Initializing SD card...");
  if (!sd.begin(10)) {
    Serial.println("Initialization failed!");
    return;
  }
  Serial.println("Initialization done.");
  //-------------------------------------------------------------------------------------------------------------------------------
  myFile = sd.open("data12.txt",O_RDONLY|O_AT_END);
  
  
  if(1){
    myFile.seekCur(-40);
    while (myFile.available()){
      buff=myFile.read();
      Serial.println(x);
      Serial.println(buff);
      if (buff=='J'){
        y=x;
        x++;
      }else if(buff==','){
        z=x;
        x++;
      }else{
        x++;
      }
    }
    Serial.println(y);
    Serial.println(z);
    myFile.seekEnd();
    x=0;
    myFile.seekCur(-40+z);
    while (myFile.available()) {
      buff=myFile.read();
        if(buff!='J'){
          num+=buff;
        }else{
          Serial.println("break");
          break;
        }
        
      }
      Serial.println("Last energy= "+num);
      
      energy=num.toFloat();
  }else{
    Serial.println("File Doesnt exist");
    }
  myFile.close();
  Serial.println(energy,5);
  Serial.println("Starting Loop");
  lcd.print("Starting Loop");
  delay(1000);
  //millis();
  lasttime=rtc.getUnixTime(rtc.getTime());
}

void loop() {

  rawcursum=0;
  rawvolsum=0;
  for (int i=0;i<=199;i++)
  {
    rawcursum += analogRead(A1);
    rawvolsum += analogRead(A2);
    delay(0.2);
    }
  current=0.22786*((rawcursum/200)-rawini);
  voltage=0.02421*(rawvolsum/200);
  
  myFile = sd.open("data12.txt", O_CREAT|O_WRONLY |O_APPEND| O_AT_END|O_SYNC);
  currenttime=rtc.getUnixTime(rtc.getTime());
  energy+=voltage*current*(currenttime-lasttime);
  Serial.println((currenttime-lasttime));
  lasttime=rtc.getUnixTime(rtc.getTime());
  myFile.print(voltage);
  myFile.print(";");
  myFile.print(current);
  myFile.print(",");
  myFile.print(energy);
  myFile.println("J");
  Serial.println(voltage);
  Serial.println(current);
  Serial.println(energy);
  myFile.close();
  delay(3000);

}
