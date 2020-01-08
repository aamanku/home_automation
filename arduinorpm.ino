
volatile byte counter;
unsigned int rpm;
unsigned long timehold;
unsigned long lastcounttime;
void setup()
{
  Serial.begin(9600);
  pinMode(2,INPUT);
  counter=1;
  rpm=0;
  timehold=0;
  lastcounttime=0;
  attachInterrupt(2,isr,RISING);
  }
void loop()
{
  if((millis()-timehold)>=1000){
  detachInterrupt(2);
  rpm=20*counter;
  Serial.println(rpm);
  timehold=millis();
  counter=0;
  attachInterrupt(2,isr,RISING);
  }}
void isr()
{
  if((millis()-lastcounttime)>10){
  counter++;
  lastcounttime=millis();
  }
  }
