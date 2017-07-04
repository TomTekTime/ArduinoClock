/***************************************************************************** 
  "Simple Clock" by TomTekTime2 05-29-17

HARDWARE: 
  Arduino Uno https://www.amazon.com/Elegoo-ATmega328P-ATMEGA16U2-Compatible-Arduino/dp/B01EWOE0UU/ref=sr_1_4?ie=UTF8&qid=1499188869&sr=8-4&keywords=arduino+uno
  DS1307 Timer Uno Shield https://www.amazon.com/gp/product/B00PI6TQWO/ref=oh_aui_detailpage_o04_s00?ie=UTF8&psc=1
  AdaFruit 2.8 TFT Uno Shield http://www.adafruit.com/products/1651

 LIBRARIES: 
  Uses DS1307 Library (RTCLib.h) https://github.com/adafruit/RTClib/archive/master.zip
  Uses ADAfruit Graphics Library (Adafruit_GFX.h) https://github.com/adafruit/Adafruit-GFX-Library
  Uses Adafruit TFT Library (Adafruit_ILI9341.h) https://github.com/adafruit/TFTLCD-Library
  

*****************************************************************************/

// TFT Display Libraries
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_STMPE610.h"
#include "Fonts/FreeSerif18pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"

// Timer Libraries
#include <Wire.h>
#include "RTClib.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define STMPE_CS 8

// Constants
const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char monthOfTheYear[12][12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

// Global Variables
RTC_DS1307 rtc;
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
boolean screen1 = true;
String displayTime;

void setup() {
 Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
     //This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     //rtc.adjust(DateTime(2017, 06, 16, 20, 0, 0));
  }
 
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
 
   if (!ts.begin()) { 
    Serial.println("Unable to start touchscreen.");
  } 
  else { 
    Serial.println("Touchscreen started."); 
  }
}


void loop(void) {
   DateTime time = rtc.now();
      
   if (!ts.bufferEmpty()){            //toggle screen displayed when touch event received
       while(!ts.bufferEmpty()){
          TS_Point p = ts.getPoint(); 
       }
      if(screen1){
        tft.fillScreen(ILI9341_BLACK);
        drawScreen2(time);
         screen1=!screen1;
      }
      else{
         tft.fillScreen(ILI9341_BLACK);
         drawScreen1(time);
          screen1=!screen1;
      }
    }
    else{                           // otherwise refresh the screen
      if(screen1){
        refreshScreen1(time);
      }
      else{
        refreshScreen2(time);
      }
    }
    delay(100);
 }


void drawScreen1(DateTime time) {
  
  //Time
  tft.setCursor(40, 120);
  tft.setFont(&FreeSerif18pt7b);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.print(getTimeStr(time));
  displayTime = getTimeStr(time);
    
  //Day of the Week
  tft.setFont(&FreeSerif12pt7b);
  tft.setTextSize(1);
  tft.setCursor(05, 220);
  tft.print(daysOfTheWeek[time.dayOfTheWeek()]);

   //Day of the Month
  tft.setTextSize(2);
  tft.setCursor(135, 220);
  tft.print(time.day());
  
  //Month (Position based on size of month String)
  tft.setTextSize(1);
  String monthStr = monthOfTheYear[time.month()-1];
  if(monthStr.length() == 3){
     tft.setCursor(270, 220);
  }
  else if(monthStr.length() == 4){
     tft.setCursor(260, 220);
  }
  else if(monthStr.length() == 5){
     tft.setCursor(250, 220);
  }
  else if(monthStr.length() == 6){
     tft.setCursor(240, 220);
  }
  else if(monthStr.length() == 7){
     tft.setCursor(230, 220);
  }
  else if(monthStr.length() == 8){
     tft.setCursor(215, 220);
  }
  else if(monthStr.length() == 9){
     tft.setCursor(210, 220);
  }
  
 tft.print(monthOfTheYear[time.month()-1]);
}


void refreshScreen1(DateTime time) {
  //Only refresh the digit that changed
  
  if(getTimeStr(time).charAt(4) != displayTime.charAt(4)){
     tft.fillRect(222, 48, 60, 80,  ILI9341_BLACK);
   }

  if(getTimeStr(time).charAt(3) != displayTime.charAt(3)){
     tft.fillRect(172, 48, 52, 80,  ILI9341_BLACK);
   }

   if(getTimeStr(time).charAt(1) != displayTime.charAt(1)){
     tft.fillRect(91, 48, 55, 80,  ILI9341_BLACK);
   }

   if(getTimeStr(time).charAt(0) != displayTime.charAt(0)){
     tft.fillRect(40, 48, 55, 80,  ILI9341_BLACK);
   }

   if(getTimeStr(time).equals("00:00")){
     tft.fillScreen(ILI9341_BLACK);
   }
  drawScreen1(time);
}


String getTimeStr(DateTime date){
  unsigned int min = date.minute();
  String minStr = String(min);
  if (min < 10 ){
    minStr = "0" + String(min);
  }

  unsigned int hour = date.hour();
  String hourStr = String(hour);
  if(hour <10){
    hourStr = "0" + String(hour);
  }
  
  String time = hourStr+ ":" + minStr;
  return time;
}



// Globals for Clock Screen 
int clockCenterX=160;
int clockCenterY=119;
int oldsec=0;

void drawScreen2(DateTime time){
  // Clear screen
  tft.fillScreen(ILI9341_BLACK);
  
  // Draw Clockface
  for (int i=0; i<5; i++){
    tft.drawCircle(clockCenterX, clockCenterY, 119-i, ILI9341_BLUE);
  }
  for (int i=0; i<5; i++)  {
    tft.drawCircle(clockCenterX, clockCenterY, i, ILI9341_BLUE);
  }
  
  tft.setCursor(clockCenterX+92,clockCenterY+5);
  tft.print("3");
  tft.setCursor(clockCenterX-8,clockCenterY+100);
  tft.print("6");
  tft.setCursor(clockCenterX-109,clockCenterY+5);
  tft.print("9");
  //tft.setCursor( clockCenterX-16,clockCenterY-109);
  tft.setCursor( clockCenterX-10,clockCenterY-90);
  tft.print("12");
  for (int i=0; i<12; i++){
    if ((i % 3)!=0)
      drawMark(i);
  }  
  drawMin(time.minute());
  drawHour(time.hour(), time.minute());
  drawSec(time.second());
  oldsec=time.second();
}


void refreshScreen2(DateTime time) {
// tft.fillScreen(ILI9341_BLACK);

  if (oldsec!=time.second())
    {
      if (time.second()==0)
      {
        drawMin(time.minute());
        drawHour(time.hour(), time.minute());
      }
      drawSec(time.second());
      oldsec=time.second();
    }
}

 
void drawMark(int h){
  float x1, y1, x2, y2;
  
  h=h*30;
  h=h+270;
  
  x1=110*cos(h*0.0175);
  y1=110*sin(h*0.0175);
  x2=100*cos(h*0.0175);
  y2=100*sin(h*0.0175);
  
  tft.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY, ILI9341_WHITE);
}

void drawSec(int s){
  float x1, y1, x2, y2;
  int ps = s-1;
  
  if (ps==-1)
    ps=59;
  ps=ps*6;
  ps=ps+270;
  
  x1=95*cos(ps*0.0175);
  y1=95*sin(ps*0.0175);
  x2=80*cos(ps*0.0175);
  y2=80*sin(ps*0.0175);
  
  tft.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY, ILI9341_BLACK);

  s=s*6;
  s=s+270;
  
  x1=95*cos(s*0.0175);
  y1=95*sin(s*0.0175);
  x2=80*cos(s*0.0175);
  y2=80*sin(s*0.0175);
  
  tft.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY, ILI9341_RED);
}

void drawMin(int m){
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int pm = m-1;
  
  if (pm==-1)
    pm=59;
  pm=pm*6;
  pm=pm+270;
  
  x1=80*cos(pm*0.0175);
  y1=80*sin(pm*0.0175);
  x2=5*cos(pm*0.0175);
  y2=5*sin(pm*0.0175);
  x3=30*cos((pm+4)*0.0175);
  y3=30*sin((pm+4)*0.0175);
  x4=30*cos((pm-4)*0.0175);
  y4=30*sin((pm-4)*0.0175);
  
  tft.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY, ILI9341_BLACK);
  tft.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY, ILI9341_BLACK);
  tft.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY, ILI9341_BLACK);
  tft.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY, ILI9341_BLACK);

  m=m*6;
  m=m+270;
  
  x1=80*cos(m*0.0175);
  y1=80*sin(m*0.0175);
  x2=5*cos(m*0.0175);
  y2=5*sin(m*0.0175);
  x3=30*cos((m+4)*0.0175);
  y3=30*sin((m+4)*0.0175);
  x4=30*cos((m-4)*0.0175);
  y4=30*sin((m-4)*0.0175);
  
  tft.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY, ILI9341_GREEN);
  tft.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY, ILI9341_GREEN);
  tft.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY,ILI9341_GREEN);
  tft.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY, ILI9341_GREEN);
}

void drawHour(int h, int m){
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int ph = h;

  if (m==0)
  {
    ph=((ph-1)*30)+((m+59)/2);
  }
  else
  {
    ph=(ph*30)+((m-1)/2);
  }
  ph=ph+270;
  
  x1=60*cos(ph*0.0175);
  y1=60*sin(ph*0.0175);
  x2=5*cos(ph*0.0175);
  y2=5*sin(ph*0.0175);
  x3=20*cos((ph+5)*0.0175);
  y3=20*sin((ph+5)*0.0175);
  x4=20*cos((ph-5)*0.0175);
  y4=20*sin((ph-5)*0.0175);
  
  tft.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY, ILI9341_BLACK);
  tft.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY, ILI9341_BLACK);
  tft.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY, ILI9341_BLACK);
  tft.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY, ILI9341_BLACK);

  h=(h*30)+(m/2);
  h=h+270;
  
  x1=60*cos(h*0.0175);
  y1=60*sin(h*0.0175);
  x2=5*cos(h*0.0175);
  y2=5*sin(h*0.0175);
  x3=20*cos((h+5)*0.0175);
  y3=20*sin((h+5)*0.0175);
  x4=20*cos((h-5)*0.0175);
  y4=20*sin((h-5)*0.0175);
  
  tft.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY, ILI9341_MAGENTA);
  tft.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY, ILI9341_MAGENTA);
  tft.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY, ILI9341_MAGENTA);
  tft.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY, ILI9341_MAGENTA);
}

