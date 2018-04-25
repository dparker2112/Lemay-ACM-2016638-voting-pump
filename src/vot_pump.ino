#include <Adafruit_NeoPixel.h>
#include "Neosegment.h"
#include <EEPROM.h>
#include "EEPROMAnything.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

#define NEO_PIXEL_COUNT 72
#define NEO_PIXEL_DIGITS 4
#define LED_BRIGHTNESS 255   // Brightness is a value from 0 to 255
#define AFTER_BUTTON_PUSH_DELAY 400

#define COLOR_WIPE_WAIT 10

#define BUTTON_PIN_POLL_1 2
#define BUTTON_PIN_POLL_2 3
#define BUTTON_PIN_POLL_3 4
#define BUTTON_PIN_POLL_4 5

#define BUTTON_PIN_RESET 22

#define NEOPIXEL_PIN_POLL_1 6
#define NEOPIXEL_PIN_POLL_2 7
#define NEOPIXEL_PIN_POLL_3 8
#define NEOPIXEL_PIN_POLL_4 9

#define NEOSEGMENT_PIN_POLL_1 10
#define NEOSEGMENT_PIN_POLL_2 11
#define NEOSEGMENT_PIN_POLL_3 12
#define NEOSEGMENT_PIN_POLL_4 13

#define EEPROM_ADDRESS_POLL_1 0
#define EEPROM_ADDRESS_POLL_2 2
#define EEPROM_ADDRESS_POLL_3 4
#define EEPROM_ADDRESS_POLL_4 6

bool firstTime = 1;

int button_state_reset = 0;

int button_state_poll_1 = 0;
int button_state_poll_2 = 0;
int button_state_poll_3 = 0;
int button_state_poll_4 = 0;

int color_poll_1 = 0;
int color_poll_2 = 0;
int color_poll_3 = 0;
int color_poll_4 = 0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip_poll_1 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEOPIXEL_PIN_POLL_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_poll_2 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEOPIXEL_PIN_POLL_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_poll_3 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEOPIXEL_PIN_POLL_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_poll_4 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEOPIXEL_PIN_POLL_4, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

Neosegment neosegment_poll_1(NEO_PIXEL_DIGITS, NEOSEGMENT_PIN_POLL_1, LED_BRIGHTNESS);
Neosegment neosegment_poll_2(NEO_PIXEL_DIGITS, NEOSEGMENT_PIN_POLL_2, LED_BRIGHTNESS);
Neosegment neosegment_poll_3(NEO_PIXEL_DIGITS, NEOSEGMENT_PIN_POLL_3, LED_BRIGHTNESS);
Neosegment neosegment_poll_4(NEO_PIXEL_DIGITS, NEOSEGMENT_PIN_POLL_4, LED_BRIGHTNESS);

int count_poll_1;
int count_poll_2;
int count_poll_3;
int count_poll_4;

void setup() {

  // initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  randomSeed(analogRead(0));

  // Get the persistant poll values from the EEPROM

  EEPROM_readAnything(EEPROM_ADDRESS_POLL_1, count_poll_1);
  EEPROM_readAnything(EEPROM_ADDRESS_POLL_2, count_poll_2);
  EEPROM_readAnything(EEPROM_ADDRESS_POLL_3, count_poll_3);
  EEPROM_readAnything(EEPROM_ADDRESS_POLL_4, count_poll_4);

  // may not need this
  delay(200);

  // BUTTON RESET
  pinMode(BUTTON_PIN_RESET, INPUT);
  digitalWrite(BUTTON_PIN_RESET, HIGH);

  // BUTTON 1

  pinMode(BUTTON_PIN_POLL_1, INPUT);
  digitalWrite(BUTTON_PIN_POLL_1, HIGH);

  String printString = "Poll 1: ";
  printString = printString + count_poll_1;

  Serial.print(printString);
  Serial.println();

  // BUTTON 2

  color_poll_2 = random(768);
  displayNumber(neosegment_poll_2, count_poll_2, color_poll_2);

  pinMode(BUTTON_PIN_POLL_2, INPUT);
  digitalWrite(BUTTON_PIN_POLL_2, HIGH);

  printString = "Poll 2: ";
  printString = printString + count_poll_2;

  Serial.print(printString);
  Serial.println();

  // BUTTON 3

  color_poll_3 = random(768);
  displayNumber(neosegment_poll_3, count_poll_3, color_poll_3);

  pinMode(BUTTON_PIN_POLL_3, INPUT);
  digitalWrite(BUTTON_PIN_POLL_3, HIGH);

  printString = "Poll 3: ";
  printString = printString + count_poll_3;

  Serial.print(printString);
  Serial.println();

  // BUTTON 4

  color_poll_4 = random(768);
  displayNumber(neosegment_poll_4, count_poll_4, color_poll_4);

  pinMode(BUTTON_PIN_POLL_4, INPUT);
  digitalWrite(BUTTON_PIN_POLL_4, HIGH);

  printString = "Poll 4: ";
  printString = printString + count_poll_4;

  Serial.print(printString);
  Serial.println();

  // NEO PIXEL STRIP 1

  strip_poll_1.begin();
  strip_poll_1.show();

  // NEO PIXEL STRIP 2

  strip_poll_2.begin();
  strip_poll_2.show();

  // NEO PIXEL STRIP 3

  strip_poll_3.begin();
  strip_poll_3.show();

  // NEO PIXEL STRIP 4

  strip_poll_4.begin();
  strip_poll_4.show();

  // NEO SEGMENT 1

  neosegment_poll_1.begin();
  neosegment_poll_1.clearAll();

  // NEO SEGMENT 2

  neosegment_poll_2.begin();
  neosegment_poll_2.clearAll();

  // NEO SEGMENT 3

  neosegment_poll_3.begin();
  neosegment_poll_3.clearAll();

  // NEO SEGMENT 4

  neosegment_poll_4.begin();
  neosegment_poll_4.clearAll();

}

void loop() {

  if (firstTime == 1) {

    firstTime = 0;

    color_poll_1 = random(768);
    colorWipe(color_poll_1, strip_poll_1);
    displayNumber(neosegment_poll_1, count_poll_1, color_poll_1);

    color_poll_2 = random(768);
    colorWipe(color_poll_2, strip_poll_2);
    displayNumber(neosegment_poll_2, count_poll_2, color_poll_2);

    color_poll_3 = random(768);
    colorWipe(color_poll_3, strip_poll_3);
    displayNumber(neosegment_poll_3, count_poll_3, color_poll_3);

    color_poll_4 = random(768);
    colorWipe(color_poll_4, strip_poll_4);
    displayNumber(neosegment_poll_4, count_poll_4, color_poll_4);
  }

  button_state_reset = digitalRead(BUTTON_PIN_RESET);

  if (button_state_reset == LOW) {

    // reset pressed

    count_poll_1 = 0;
    count_poll_2 = 0;
    count_poll_3 = 0;
    count_poll_4 = 0;

    color_poll_1 = random(768);
    color_poll_2 = random(768);
    color_poll_3 = random(768);
    color_poll_4 = random(768);

    EEPROM_writeAnything(EEPROM_ADDRESS_POLL_1, count_poll_1);
    EEPROM_writeAnything(EEPROM_ADDRESS_POLL_2, count_poll_2);
    EEPROM_writeAnything(EEPROM_ADDRESS_POLL_3, count_poll_3);
    EEPROM_writeAnything(EEPROM_ADDRESS_POLL_4, count_poll_4);

    colorWipe(color_poll_1, strip_poll_1);
    displayNumber(neosegment_poll_1, count_poll_1, color_poll_1);

    colorWipe(color_poll_2, strip_poll_2);
    displayNumber(neosegment_poll_2, count_poll_2, color_poll_2);

    colorWipe(color_poll_3, strip_poll_3);
    displayNumber(neosegment_poll_3, count_poll_3, color_poll_3);

    colorWipe(color_poll_4, strip_poll_4);
    displayNumber(neosegment_poll_4, count_poll_4, color_poll_4);

    delay(AFTER_BUTTON_PUSH_DELAY);
  }

  // POLL 1

  button_state_poll_1 = digitalRead(BUTTON_PIN_POLL_1);

  if (button_state_poll_1 == LOW) {

    color_poll_1 = random(768);

    colorWipe(color_poll_1, strip_poll_1);

    EEPROM_writeAnything(EEPROM_ADDRESS_POLL_1, ++count_poll_1);

    String printString = "Poll 1: ";
    printString = printString + count_poll_1;

    Serial.print(printString);
    Serial.println();

    displayNumber(neosegment_poll_1, count_poll_1, color_poll_1);

    delay(AFTER_BUTTON_PUSH_DELAY);
  }

  // POLL 2

  button_state_poll_2 = digitalRead(BUTTON_PIN_POLL_2);

  if (button_state_poll_2 == LOW) {

    color_poll_2 = random(768);

    colorWipe(color_poll_2, strip_poll_2);

    EEPROM_writeAnything(EEPROM_ADDRESS_POLL_2, ++count_poll_2);

    String printString = "Poll 2: ";
    printString = printString + count_poll_2;

    displayNumber(neosegment_poll_2, count_poll_2, color_poll_2);

    delay(AFTER_BUTTON_PUSH_DELAY);
  }

  // POLL 3

  button_state_poll_3 = digitalRead(BUTTON_PIN_POLL_3);

  if (button_state_poll_3 == LOW) {
    color_poll_3 = random(768);

    colorWipe(color_poll_3, strip_poll_3);

    EEPROM_writeAnything(EEPROM_ADDRESS_POLL_3, ++count_poll_3);

    String printString = "Poll 3: ";
    printString = printString + count_poll_3;

    Serial.print(printString);
    Serial.println();

    displayNumber(neosegment_poll_3, count_poll_3, color_poll_3);

    delay(AFTER_BUTTON_PUSH_DELAY);
  }

  // POLL 4

  button_state_poll_4 = digitalRead(BUTTON_PIN_POLL_4);

  if (button_state_poll_4 == LOW) {

    color_poll_4 = random(768);

    colorWipe(color_poll_4, strip_poll_4);

    EEPROM_writeAnything(EEPROM_ADDRESS_POLL_4, ++count_poll_4);

    String printString = "Poll 4: ";
    printString = printString + count_poll_4;

    Serial.print(printString);
    Serial.println();

    displayNumber(neosegment_poll_4, count_poll_4, color_poll_4);

    delay(AFTER_BUTTON_PUSH_DELAY);
  }
}

// Fill the dots one after the other with a color
void colorWipe(int color, Adafruit_NeoPixel& strip) {

  for (int e = 0; e < NEO_PIXEL_COUNT; e++) {
    for (int f = 0; f < 4; f++) {
      setPixel(strip, e + f, color, 255);
    }
    strip.show();
    delay(12);
    strip.setPixelColor(e, 0, 0, 0);
  }
  strip.show();

}

void setSeg(Neosegment& segment, int segz, int valz, int colorz, int fadez) {
  while (colorz > 767) {
    colorz -= 768;
  }
  segment.setDigit(segz,  valz,
                   ((constrain(colorz, 0, 255) - constrain(colorz - 255, 0, 255))*fadez) / 255,
                   ((constrain(colorz - 255, 0, 255) - constrain(colorz - 511, 0, 255))*fadez) / 255,
                   (((255 - constrain(colorz - 0, 0, 255)) + constrain(colorz - 511, 0, 255))*fadez) / 255);
}

void setPixel(Adafruit_NeoPixel& strip, int pixelz, int colorz, int fadez) {
  while (colorz > 767) {
    colorz -= 768;
  }
  strip.setPixelColor(pixelz,
                      ((constrain(colorz, 0, 255) - constrain(colorz - 255, 0, 255))*fadez) / 255,
                      ((constrain(colorz - 255, 0, 255) - constrain(colorz - 511, 0, 255))*fadez) / 255,
                      (((255 - constrain(colorz - 0, 0, 255)) + constrain(colorz - 511, 0, 255))*fadez) / 255);
}

void displayNumber(Neosegment& segment, int number, int color) {

  int ones = (number % 10);
  int tens = ((number / 10) % 10);
  int hundreds = ((number / 100) % 10);
  int thousands = (number / 1000);

  setSeg(segment, 3, ones, color, 255);
  setSeg(segment, 2, tens, color, 255);
  setSeg(segment, 1, hundreds, color, 255);
  setSeg(segment, 0, thousands, color, 255);
}
