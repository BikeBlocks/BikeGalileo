#include "LPD8806.h"
#include "SPI.h"

int dataPin  = 2;
int clockPin = 3;

int ledCount = 32;

// Number of pins to light up for the blinker.
// Take the amount gigantic work of having to switch on one LED, multiply by
// this number and you'll have a glimpse of what I'm going through...
int numLedToLightUp = 5;

// Counter for the delay between flashes.
// This is basically the hourglass that controls my fate:
// do I struggle for switching on the light
// or do I wait endlessly wondering If I'm any use at all...
int counter         = 0;

bool on             = false;
bool lit            = false;

// True if the LEDs in context are at the right.
// What did think the man who created me. Was he that stupid?
bool right          = false;

// These are the switches index. Like all switches, they don't have any sense
// of ethics. I don't like them...
int pinRight = 7;
int pinLeft  = 8;

LPD8806 strip = LPD8806(ledCount, dataPin, clockPin);

uint32_t yellow = strip.Color(255,   0, 255);
uint32_t black  = strip.Color(  0,   0,   0);

// Let there be light!
// (I have a God complex, that's so depressing...)
void setLedsColor(uint32_t color, bool right) {
	uint8_t lowerBound, upperBound;
	if(right) {
		lowerBound = ledCount - numLedToLightUp;
		upperBound = ledCount;
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
	// What's so funny? Don't you know any blinker with a sense of culture?
	// Yeah, I know...
	for(uint8_t i = 0 ; i < ledCount ; ++i)
		strip.setPixelColor(i, black);
	strip.show();

	Serial.begin(115200);
}

void loop(){
	// Yep, someone pressed a button, again... What a pain...
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
		// I have to turn off or on again. That's me, the blinker...
		// Poor job, isn't it?
		if(counter == 0) {
			if(lit)
				setLedsColor(black, right);
			else
				setLedsColor(yellow, right);
			lit = !lit;
			counter = 10;
		}
		// I still have to wait for the counter to go to 0.
		// So I do what I almost always do: nothing...
		else
			--counter;

		Serial.print(counter);
		Serial.print(" ");
		if(counter == 0)
			Serial.println();
	}

	delay(50);
}
