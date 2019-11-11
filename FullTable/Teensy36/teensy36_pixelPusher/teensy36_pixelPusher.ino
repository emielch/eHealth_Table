#include <MultiWS2811.h>

const int ledsPerStrip = 84;

DMAMEM int displayMemory[ledsPerStrip * 96];
COL_RGB copyMemory[ledsPerStrip * 128];
COL_RGB drawingMemory[ledsPerStrip * 128];

const int config = WS2811_GRB | WS2811_800kHz;

MultiWS2811 leds(ledsPerStrip, displayMemory, copyMemory, drawingMemory, config);

unsigned int screenSaverDelay = 2000;
elapsedMillis sinceNewFrame = screenSaverDelay+1;



void setup()
{
	Serial.setTimeout(50);
	leds.begin();
}


void loop()
{
	if (sinceNewFrame > screenSaverDelay) {
		reset_ledLib();
		leds.show();
	}

	int startChar = Serial.read();

	if (startChar == '%') {
		// receive a frame
		int count = Serial.readBytes((char *)drawingMemory, sizeof(drawingMemory));
		if (count == sizeof(drawingMemory)) {
			leds.show();
			sinceNewFrame = 0;
		}

	}
	else if (startChar == '?') {
		// when the video application asks, give it all our info
		// for easy and automatic configuration
		Serial.print("LEDS");
		Serial.write(',');
		Serial.print(ledsPerStrip);
		Serial.println();
	}
	else if (startChar >= 0) {
		// discard unknown characters
	}
}


uint32_t bufsize = ledsPerStrip * 384;
void reset_ledLib() {
	memset(drawingMemory, 0, bufsize);
}