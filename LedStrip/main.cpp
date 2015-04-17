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
#include "Makeblock.h"
#include <SoftwareSerial.h>
#include <Wire.h>

#define yellow (uint8_t)255, (uint8_t)255, (uint8_t)0
#define black  (uint8_t)0, (uint8_t)0, (uint8_t)0

MeRGBLed led(PORT_3);
int ledCount = 15;
bool on     = false;
bool lit    = false;
bool right  = false;
int counter = 0;

void setLedsColor(uint8_t red, uint8_t green, uint8_t blue, bool right) {
	uint8_t lowerBound = (right)? ledCount - 3 : 0;
	uint8_t upperBound = (right)? ledCount     : 3;
	for(uint8_t i = lowerBound ; i < upperBound ; ++i)
		led.setColorAt(i, red, green, blue);
	led.show();
}

void setup() {
	pinMode(2, INPUT_PULLUP);
	pinMode(4, INPUT_PULLUP);

	led.setNumber(ledCount);

	// In order for the light to shine so brightly, the darkness must be present.
	//                       -- Francis Bacon
	for(uint8_t i = 0 ; i < ledCount ; ++i)
		led.setColorAt(i, 0, 0, 0);
	led.show();

	Serial.begin(115200);
}

void loop(){
	if(digitalRead(2) == 0 || digitalRead(4) == 0) {
		bool newRight = (digitalRead(2) == 0);
		Serial.println();
		Serial.print("On ");
		Serial.println((newRight)? "right!" : "left!");

		while(digitalRead(2) == 0 || digitalRead(4) == 0);

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
