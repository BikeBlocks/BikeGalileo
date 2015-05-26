/*************************************************************************
 * File Name          : ColorLoop.ino
 * Author             : Evan
 * Updated            : Ander
 * Version            : V1.0.2
 * Date               : 27/03/2014
 * Description        : Test for Makeblock Electronic modules of  MeRGBLed and
                        LedStrip.
 * License            : CC-BY-SA 3.0
 * Copyright (C) 2013 Maker Works Technology Co., Ltd. All right reserved.
 * http://www.makeblock.cc/
 **************************************************************************/
#include "LPD8806.h"
#include "SPI.h"
//#include <SoftwareSerial.h>
//#include <Wire.h>

int dataPin  = 2;
int clockPin = 3;

int ledCount        = 32;
int numLedToLightUp = 5;
int counter         = 0;
bool on             = false;
bool lit            = false;
bool right          = false;

int pinRight = 7;
int pinLeft  = 8;

LPD8806 strip = LPD8806(ledCount, dataPin, clockPin);

uint32_t yellow = strip.Color(255,   0, 255);
uint32_t black  = strip.Color(  0,   0,   0);

void setLedsColor(uint32_t color, bool right) {
	uint8_t lowerBound, upperBound;
	if(right) {
		lowerBound = ledCount - numLedToLightUp;
		upperBound = ledCount ;
	} else {
		lowerBound = 0;
		upperBound = numLedToLightUp;
	}

	for(uint8_t i = lowerBound ; i < upperBound ; ++i)
		strip.setPixelColor(i, color);
	strip.show();
}

void setup() {
	pinMode(pinRight, INPUT_PULLUP);
	pinMode(pinLeft, INPUT_PULLUP);

	strip.begin();

	// In order for the light to shine so brightly, the darkness must be present.
	//                       -- Francis Bacon
	for(uint8_t i = 0 ; i < ledCount ; ++i)
		strip.setPixelColor(i, black);
	strip.show();

	Serial.begin(115200);
}

void loop(){
	if(digitalRead(pinRight) == 0 || digitalRead(pinLeft) == 0) {
		bool newRight = (digitalRead(pinRight) == 0);
		Serial.println();
		Serial.print("On ");
		Serial.println((newRight)? "right!" : "left!");

		while(digitalRead(pinRight) == 0 || digitalRead(pinLeft) == 0);

		bool different = (newRight != right);
		right = newRight;

		if(on && !different) {
			setLedsColor(black, right);
			on = false;
		} else {
			if(different)
				setLedsColor(black, !right);

			setLedsColor(yellow, right);
			counter = 10;
			lit = true;
			on = true;
		}
	}

	if(on) {
		if(counter == 0) {
			if(lit)
				setLedsColor(black, right);
			else
				setLedsColor(yellow, right);
			lit = !lit;
			counter = 10;
		} else
			--counter;

		Serial.print(counter);
		Serial.print(" ");
		if(counter == 0)
			Serial.println();
	}

	delay(50);
}
