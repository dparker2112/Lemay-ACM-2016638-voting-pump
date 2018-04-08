#include "Neosegment.h"
#define nDigits 4
#define PIN 9
#define PIN 10

boolean bhold;   //new

#define pix 72
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pix, 6, NEO_GRB + NEO_KHZ800);

int color;

Neosegment neosegment(nDigits, PIN, 100);

void setup() {
  pinMode(4,INPUT_PULLUP);
  neosegment.begin();
  neosegment.clearAll();
  strip.begin();
  strip.setPixelColor(0,255,255,255);
  strip.show();
}

void loop() {
  for(int a=0;a<10;a++){
    setSeg(0,a,color,255);
    for(int b=0;b<10;b++){
      setSeg(1,b,color,255);
      for(int c=0;c<10;c++){
        setSeg(2,c,color,255);
        for(int d=0;d<10;d++){
          setSeg(3,d,color,255);
          while(digitalRead(4)||bhold){  //changed
            setSeg(0,a,color,255);
            setSeg(1,b,color,255);
            setSeg(2,c,color,255);
            setSeg(3,d,color,255);
            if(!digitalRead(4)&&bhold){bhold=0;}  //new
            delay(8);
          }
          bhold=1;  //new
          color=random(768);
          for(int e=0;e<pix;e++){
            for(int f=0;f<4;f++){
              setPixel(e+f,color,255);
            }
            strip.show();
            delay(12);
            strip.setPixelColor(e,0,0,0);
          }
          strip.show();
        }
      }
    }
  }
}

void setSeg(int segz,int valz, int colorz, int fadez){
  while(colorz>767){colorz-=768;}
  neosegment.setDigit(segz,  valz,
  ((constrain(colorz,0,255)-constrain(colorz-255,0,255))*fadez)/255,
  ((constrain(colorz-255,0,255)-constrain(colorz-511,0,255))*fadez)/255,
  (((255-constrain(colorz-0,0,255))+constrain(colorz-511,0,255))*fadez)/255);
}

void setPixel(int pixelz, int colorz, int fadez){
  while(colorz>767){colorz-=768;}
  strip.setPixelColor(pixelz,
  ((constrain(colorz,0,255)-constrain(colorz-255,0,255))*fadez)/255,
  ((constrain(colorz-255,0,255)-constrain(colorz-511,0,255))*fadez)/255,
  (((255-constrain(colorz-0,0,255))+constrain(colorz-511,0,255))*fadez)/255);
}
