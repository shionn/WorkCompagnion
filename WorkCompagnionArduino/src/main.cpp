#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
	#include <avr/power.h>
#endif

#define PIN 6

#define NONE 0
#define GREEN_FLASH 1
#define YELLO_FLASH 2
#define RED_FLASH 3
#define GREEN_RING 4
#define YELLOW_RING 5
#define BLUE_RING 6
#define RED_RING 7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
	Serial.begin(9600);
	strip.begin();
	strip.show();
}

int mode = GREEN_FLASH;

uint32_t ring = 0;
int flash = 0;

void doFlash(uint32_t r, uint32_t g, uint32_t b) {
	uint32_t color = strip.Color(r>>abs(8-flash), g>>abs(8-flash), b>>abs(8-flash));
	for(uint32_t i=0; i<strip.numPixels(); i++) {
		strip.setPixelColor(i, color);
		strip.show();
	}
	flash = (flash + 1) % 16;
}

void doRing(uint32_t r, uint32_t g, uint32_t b) {
	for (uint32_t i=0; i<4; i++) {
		uint32_t color = strip.Color(r >> (6-i*2), g >> (6-i*2), b >> (6-i*2));
		strip.setPixelColor((ring+i)%16, color);
		strip.show();
		strip.setPixelColor((ring+i+8)%16, color);
		strip.show();
		strip.setPixelColor((ring+i+4)%16, 0,0,0);
		strip.show();
		strip.setPixelColor((ring+i+12)%16, 0,0,0);
		strip.show();
	}
	ring = (ring +1) % 8;
}

void doNone() {
	strip.clear();
	strip.show();
}

void loop() {
	switch (mode) {
		case NONE : doNone(); break;
		case GREEN_FLASH : doFlash(0,255,0); break;
		case YELLO_FLASH : doFlash(255,255,0); break;
		case RED_FLASH : doFlash(255,0,0); break;
		case GREEN_RING : doRing(0,255,0); break;
		case YELLOW_RING : doRing(255,255,0); break;
		case BLUE_RING : doRing(0,0,255); break;
		case RED_RING : doRing(255,0,0); break;
	}

	delay(100);

	if (Serial.available()) {
		String message = Serial.readString();
		message.trim();
		// Serial.println(message);
		Serial.flush();
		if (message.equalsIgnoreCase("redring")) {
			mode = RED_RING;
		} else if (message.equalsIgnoreCase("yellowring")) {
			mode = YELLOW_RING;
		} else if (message.equalsIgnoreCase("greenring")) {
			mode = GREEN_RING;
		} else if (message.equalsIgnoreCase("bluering")) {
			mode = BLUE_RING;
		} else if (message.equalsIgnoreCase("redflash")) {
			mode = RED_FLASH;
		} else if (message.equalsIgnoreCase("yellowflash")) {
			mode = YELLO_FLASH;
		} else if (message.equalsIgnoreCase("greenflash")) {
			mode = GREEN_FLASH;
		} else {
			mode = NONE;
		}
	}

}
