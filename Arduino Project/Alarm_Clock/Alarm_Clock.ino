//************libraries**************//
#include <SoftwareSerial.h>
#include <Wire.h>
#include <DS3231.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//************************************//
Adafruit_SSD1306 display(4);
DS3231 clock;
RTCDateTime dt;
SoftwareSerial bluetooth(3, 2); //rx hc06 -> pin 2, tx hc06 -> pin 3 


//************Button*****************//
int P1=8; // Button SET MENU'
int P2=9; // Button +
int P3=10; // Button -
int P4=11; // SWITCH Alarm

//**************Alarm***************//
#define LED 13
#define buzzer 6

//************Variables**************//
int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg;
int menu =0;
int setAll =0;

String incomingByte; //to receive string 
String tipo,ore,minuti;

uint8_t alarmHours = 0, alarmMinutes = 0;  // Holds the current alarm time

void setup()
{
  clock.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  bluetooth.begin(9600); //set baud rate
  pinMode(P1,INPUT_PULLUP); // https://www.arduino.cc/en/Tutorial/InputPullupSerial
  pinMode(P2,INPUT_PULLUP);
  pinMode(P3,INPUT_PULLUP);
  pinMode(P4,INPUT_PULLUP);
  pinMode(LED,OUTPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer as an output
  printAllOff();
  Serial.begin(9600);
  Wire.begin();
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(24,5);
  display.setTextColor(WHITE);
  display.print("HOLO"); 
  display.setCursor(18,33);
  display.print("ALARM"); 
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(2);

  int menu=0;

}
 
void loop()
{ 
    //bluetooth
    if (bluetooth.available() > 0) {
    // read the incoming byte:
    incomingByte = bluetooth.readString(); //to read string
    tipo = incomingByte.substring(0,4);
    int ind1 = incomingByte.indexOf(":",0);
    int ind2 = incomingByte.indexOf(":",ind1+1);
	//tipo = incomingByte.substring(0,ind1-1);
    ore = incomingByte.substring(ind1+1,ind2);
    minuti = incomingByte.substring(ind2+1);
    Serial.print(minuti);
    if(tipo=="time"){ //se la prima sottostringa (delimitata da ":") del comando ricevuto è "time" allora:
      if(ore.length() == 1)
        ore = "0"+ore;
      if(minuti.length() == 1)
        minuti = "0"+minuti;
      clock.setDateTime(2014, 4, 13, ore.toInt(), minuti.toInt(), 00);     // Set the time to 12:00:00 (24hr format)
      }
      }
// check if you press the SET button and increase the menu index
  if(digitalRead(P1)== LOW) 
  {
   menu=menu+1;
  }
  if((digitalRead(P2)== LOW)&&(digitalRead(P3)== LOW))                                                                                                                                                                     
  {
    
    DisplaySetHourAll();
    display.clearDisplay();
    DisplaySetMinuteAll();
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.print("SAVING IN PROGRESS");
    display.display();
    display.clearDisplay();
   //QUIQUIQUI
   
    delay(1000);
    display.clearDisplay();
  }
// in which subroutine should we go?
  if (menu==0)
    {
     DisplayDateTime(); // void DisplayDateTime
     Alarm(); // Alarm control
          }
  if (menu==1)
    {
    DisplaySetHour();
    }
  if (menu==2)
    {
    DisplaySetMinute();
    }
  
  if (menu==6)
    {
    StoreAgg(); 
    delay(500);
    menu=0;
    }
    delay(2);  
}

void DisplayDateTime ()
{
// We show the current date and time
  dt = clock.getDateTime();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 10);
  display.setTextSize(4);
  //display.print("Hour:");
  
  if (dt.hour<=9)
  {
    display.print("0");
  }
  display.print(dt.hour, DEC);
  hourupg=dt.hour;
  display.print(":");
  if (dt.minute<=9)
  {
    display.print("0");
  }
  display.print(dt.minute, DEC);
  minupg=dt.minute;
  
  
  display.display();
}

void DisplaySetHour()
{
// time setting
  display.clearDisplay();
  dt = clock.getDateTime();
  if(digitalRead(P2)==LOW)
  {
    if(hourupg==23)
    {
      hourupg=0;
    }
    else
    {
      hourupg=hourupg+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if(hourupg==0)
    {
      hourupg=23;
    }
    else
    {
      hourupg=hourupg-1;
    }
  }
  display.setCursor(0,0);
  display.print("Set hour:");
  display.setCursor(0,18);
  display.print(hourupg,DEC);
  display.display();
  delay(200);
}

void DisplaySetMinute()
{
// Setting the minutes
  display.clearDisplay();
  if(digitalRead(P2)==LOW)
  {
    if (minupg==59)
    {
      minupg=0;
    }
    else
    {
      minupg=minupg+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if (minupg==0)
    {
      minupg=59;
    }
    else
    {
      minupg=minupg-1;
    }
  }
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Set Minutes:");
  display.setCursor(0,18);
  display.print(minupg,DEC);
  display.display();
  delay(200);
}
  

void StoreAgg()
{
// Variable saving
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("SAVING IN PROGRESS");
  display.display();
  delay(1000);
  display.clearDisplay();
  clock.setDateTime(yearupg,monthupg,dayupg,hourupg,minupg,00);
  
  delay(200);
}
void DisplaySetHourAll()// Setting the alarm minutes
{
  while(digitalRead(P1)==HIGH){

  display.clearDisplay();

  if(digitalRead(P2)==LOW)
  {
    if(alarmHours==23)
    {
      alarmHours=0;
    }
    else
    {
      alarmHours=alarmHours+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if(alarmHours==0)
    {
      alarmHours=23;
    }
    else
    {
      alarmHours=alarmHours-1;
    }
  }
    display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print("Hour Alarm");
  display.setCursor(50,25);
  display.setTextSize(3);
  display.print(alarmHours,DEC);
  delay(200);
  display.display();
 }
 delay(200);
}

void DisplaySetMinuteAll()// Setting the alarm minutes
 {
  while(digitalRead(P1)==HIGH){ 

  display.clearDisplay();
  if(digitalRead(P2)==LOW)
  {
    if (alarmMinutes==59)
    {
      alarmMinutes=0;
    }
    else
    {
      alarmMinutes=alarmMinutes+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if (alarmMinutes==0)
    {
      alarmMinutes=59;
    }
    else
    {
      alarmMinutes=alarmMinutes-1;
    }
  }
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print("Min Alarm");
  display.setCursor(50,25);
  display.setTextSize(3);
  display.print(alarmMinutes,DEC);
  display.display();
  delay(200);
 }
 delay(200);
}
void printAllOn(){
   display.setCursor(0,50);
   display.setTextSize(1);
   display.print("Alarm: ");
  
  
  
  if (alarmHours <= 9)
  {
    display.print("0");
  }
  display.print(alarmHours, DEC);
  
  display.print(":");
  if (alarmMinutes <= 9)
  {
    display.print("0");
  }
  display.print(alarmMinutes, DEC); 
  display.display();
}
void printAllOff() {
  display.setCursor(0, 50);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.print("Alarm: Off  ");  
  display.display();
}
void Alarm(){
  int i;
   if(digitalRead(P4)== LOW)
  {
   setAll=setAll+1;
  }
  if (setAll==0)
    {
     printAllOff();
     digitalWrite(LED,LOW);
     }
  if (setAll==1)
    {

     printAllOn();    
  
      dt = clock.getDateTime();
     if ( dt.hour == alarmHours && dt.minute == alarmMinutes )
        {
           dt = clock.getDateTime();
         digitalWrite(LED,HIGH);
         for(i=0;i<20;i++)
          {
          digitalWrite(buzzer,HIGH);
          delay(2);//wait for 1ms
          digitalWrite(buzzer,LOW);
          delay(2);//wait for 1ms
          }
        }
    else{
        
         digitalWrite(LED,LOW);
        }
    
    } 
  if (setAll==2)
    {
     setAll=0;
    }
    delay(200);
}
