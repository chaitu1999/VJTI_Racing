/*#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,6,5,4,3);*/
float value=0;
float rev=0;
int rpm;
int oldtime=0;
int time;

void isr() //interrupt service routine
{
rev++;
}

void setup()
{
//lcd.begin(16,2);                //initialize LCD
attachInterrupt(digitalPinToInterrupt(3),isr,RISING);  //attaching the interrupt
}

void loop()
{
delay(1000);
Serial.begin(9600);
detachInterrupt(3);           //detaches the interrupt
time=millis()-oldtime;        //finds the time 
rpm=(rev/time)*60000;         //calculates rpm
oldtime=millis();             //saves the current time
rev=0;
/*lcd.clear();
lcd.setCursor(0,0);
lcd.print("___TACHOMETER___");
lcd.setCursor(0,1);
lcd.print(     rpm);
lcd.print(" RPM");
lcd.print("   ");*/
Serial.println(rpm);
attachInterrupt(digitalPinToInterrupt(3),isr,RISING);
}
