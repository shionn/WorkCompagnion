#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
	#include <avr/power.h>
#endif

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
	Serial.begin(9600);
	strip.begin();
	strip.show();
}


int pixel = 0;
void loop() {
	for(int i=0; i<strip.numPixels()/2; i++) {
		if (i == pixel) {
			strip.setPixelColor(i, strip.Color(255, 0, 0));
			strip.show();
			strip.setPixelColor(i+strip.numPixels()/2, strip.Color(255, 0, 0));
			strip.show();
		} else {
			strip.setPixelColor(i, strip.Color(0, 0, 0));
			strip.show();
			strip.setPixelColor(i+strip.numPixels()/2, strip.Color(0, 0, 0));
			strip.show();
		}
	}
	pixel = (pixel +1) % 8;
	delay(100);

	if (Serial.available()) {
		Serial.println("coucou");
		Serial.println(Serial.readString());
	}
}
