#include <Firmata.h>
#include <Boards.h>

/**************************************************************************
 *                                                                     	*  	 
 *   NeoPixel Ring Clock                                               	*
 *   Copyright (C) 2014  Andy Doro (mail@andydoro.com)                 	*
 *                                                                     	*
 ***************************************************************************
 *                                                                     	*
 * This program is free software; you can redistribute it and/or modify	*
 * it under the terms of the GNU General Public License as published by	*
 * the Free Software Foundation; either version 2 of the License, or   	*
 * (at your option) any later version.                                 	*
 *                                                                     	*
 * This program is distributed in the hope that it will be useful,     	*
 * but WITHOUT ANY WARRANTY; without even the implied warranty of      	*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       	*
 * GNU General Public License for more details.                        	*
 *                                                                     	*
 * You should have received a copy of the GNU General Public License   	*
 * along with this program; if not, write to the Free Software         	*
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,               	*
 * MA  02111-1307  USA                                                 	*
 *                                                                     	*
 ***************************************************************************
 *
 *
 * Revision History
 *
 * Date 	 By    What
 * 20140320 	AFD 	First draft
 */

// include the library code:
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>

// define pins
#define HOURPIN 6
#define MINUTEPIN 5
#define SECONDPIN 4
#define BRIGHTNESS 8 // set max brightness

RTC_DS1307 RTC; // Establish clock object
DateTime Clock; // Holds current clock time

Adafruit_NeoPixel hour_circle = Adafruit_NeoPixel(12, HOURPIN, NEO_GRB + NEO_KHZ800); // hour_circle object
Adafruit_NeoPixel minute_quarter = Adafruit_NeoPixel(15, MINUTEPIN, NEO_GRB + NEO_KHZ800);// quarter circle
Adafruit_NeoPixel second_quarter = Adafruit_NeoPixel(15, SECONDPIN, NEO_GRB + NEO_KHZ800);// quarter circle

byte hourval, minuteval, secondval; // holds the time
// byte pixelColorRed, pixelColorGreen, pixelColorBlue; // holds color values
int pixelColorGreen = 0;
int pixelColorBlue = 0;
int pixelColorRed = 0;


void setup () {
  Wire.begin();        	// Begin I2C
  RTC.begin();   // begin clock

  // Serial.begin(9600);
  // set pinmodes
  pinMode(HOURPIN, OUTPUT);
  pinMode(MINUTEPIN, OUTPUT);
  pinMode(SECONDPIN, OUTPUT);

  if (! RTC.isrunning()) {
	// Serial.println("RTC is NOT running!");
	// following line sets the RTC to the date & time this sketch was compiled
	RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  hour_circle.begin();
  second_quarter.begin();
  minute_quarter.begin();
  //hour_circle.show(); // Initialize all pixels to 'off'

  hour_circle.setBrightness(BRIGHTNESS); // set brightness
  minute_quarter.setBrightness(BRIGHTNESS);
  second_quarter.setBrightness(BRIGHTNESS);

  // startup sequence
  delay(500);
  colorWipe(hour_circle.Color(255, 0, 0), 10); // Redhour_circle
  colorWipe(hour_circle.Color(0, 255, 0), 10); // Green
  colorWipe(hour_circle.Color(0, 0, 255), 10); // Blue
  delay(500);

}

void loop () {


  char* colon = ":"; // static characters save a bit
  char* slash = "/"; // of memory
  int minute_pix;
  int second_pix;

  // get time
  Clock = RTC.now(); // get the RTC time

  secondval = Clock.second();  // get seconds
  minuteval = Clock.minute();  // get minutes
  hourval = Clock.hour();  	// get hours

  if(hourval > 11) hourval -= 12; // This clock is 12 hour, if 13-23, convert to 0-11

  minute_pix = (minuteval % 15) - 1;
  second_pix = (secondval % 15) - 1;
  
  for(uint8_t i=0; i<hour_circle.numPixels(); i++) {
    pixelColorGreen = 0;
    pixelColorBlue = 0;
    pixelColorRed = 0;
    if (i < hourval || hourval == 0) {
      pixelColorRed = 255;   
    }
    hour_circle.setPixelColor(i, hour_circle.Color(pixelColorRed, pixelColorGreen, pixelColorBlue));
  }
  

  for(uint8_t i=0; i<minute_quarter.numPixels(); i++) {
    pixelColorGreen = 0;
    pixelColorBlue = 0;
    pixelColorRed = 0;
    if (minuteval >= 15) {
      if (i == 4) {
        pixelColorRed = 100;
        pixelColorBlue = 100;
      }
    }
    if (minuteval >= 30) {
      if (i == 9) {
        pixelColorRed = 100;
        pixelColorBlue = 100;
      }
    }
    if (minuteval >= 45) {
      if (i == 14) {
        pixelColorRed = 100;
        pixelColorBlue = 100;
      }
    }

    if (i == minute_pix) {
      pixelColorBlue = 255;   
    }
    minute_quarter.setPixelColor(i, minute_quarter.Color(pixelColorRed, pixelColorGreen, pixelColorBlue));
  }

  for(uint8_t i=0; i<second_quarter.numPixels(); i++) {
    pixelColorGreen = 0;
    pixelColorBlue = 0;
    pixelColorRed = 0;
    if (secondval >= 15) {
      if (i == 4) {
        pixelColorRed = 100;
        pixelColorBlue = 100;
      }
    }
    if (secondval >= 30) {
      if (i == 9) {
        pixelColorRed = 100;
        pixelColorBlue = 100;
      }
    }
    if (secondval >= 45) {
      if (i == 14) {
        pixelColorRed = 100;
        pixelColorBlue = 100;
      }
    }
    if (i == second_pix) {
      pixelColorGreen = 255;
    }
    second_quarter.setPixelColor(i, second_quarter.Color(pixelColorRed, pixelColorGreen, pixelColorBlue));
  }

/*
  // for serial debugging
   Serial.print(hourval, DEC);
   Serial.print(':');
   Serial.print(minuteval, DEC);
   Serial.print(':');
   Serial.println(secondval, DEC); 
*/
  //display
  hour_circle.show();
  minute_quarter.show();
  second_quarter.show();
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<hour_circle.numPixels(); i++) {
	hour_circle.setPixelColor(i, c);
	hour_circle.show();
	delay(wait);
  }
  for(uint16_t i=0; i<minute_quarter.numPixels(); i++) {
	minute_quarter.setPixelColor(i, c);
	minute_quarter.show();
	delay(wait);
  }
  for(uint16_t i=0; i<second_quarter.numPixels(); i++) {
	second_quarter.setPixelColor(i, c);
	second_quarter.show();
	delay(wait);
  }

}
