#include "Orb.h"
#include <Colore.h>
#include <OctoWS2811_Dither.h>

#define WIDTH 21
#define HEIGHT 20

extern double dataAvg[];

int ledMap[WIDTH][HEIGHT] = {
{ -1,-1,-1,-1,-1,-1,-1,-1,175,182,189,196,-1,-1,-1,-1,-1,-1,-1,-1},
{ -1,-1,-1,-1,-1,-1,-1,170,176,183,190,197,203,-1,-1,-1,-1,-1,-1,-1},
{ -1,-1,-1,-1,-1,-1,168,171,177,184,191,198,204,208,-1,-1,-1,-1,-1,-1},
{ -1,-1,259,266,273,280,169,172,178,185,192,199,205,209,217,224,231,238,-1,-1},
{ -1,254,260,267,274,281,287,173,179,186,193,200,206,212,218,225,232,239,245,-1},
{ -1,255,261,268,275,282,288,174,180,187,194,201,207,213,219,226,233,240,246,-1},
{ 252,256,262,269,276,283,289,292,181,188,195,202,210,214,220,227,234,241,247,250},
{ 253,257,263,270,277,284,290,293,7,14,21,28,211,215,221,228,235,242,248,251},
{ -1,258,264,271,278,285,291,2,8,15,22,29,35,216,222,229,236,243,249,-1},
{ -1,-1,265,272,279,286,0,3,9,16,23,30,36,40,223,230,237,244,-1,-1},
{ -1,-1,133,140,147,154,1,4,10,17,24,31,37,41,49,56,63,70,-1,-1},
{ -1,128,134,141,148,155,161,5,11,18,25,32,38,44,50,57,64,71,77,-1},
{ -1,129,135,142,149,156,162,6,12,19,26,33,39,45,51,58,65,72,78,-1},
{ 126,130,136,143,150,157,163,166,13,20,27,34,42,46,52,59,66,73,79,82},
{ 127,131,137,144,151,158,164,167,91,98,105,112,43,47,53,60,67,74,80,83},
{ -1,132,138,145,152,159,165,86,92,99,106,113,119,48,54,61,68,75,81,-1},
{ -1,-1,139,146,153,160,84,87,93,100,107,114,120,124,55,62,69,76,-1,-1},
{ -1,-1,-1,-1,-1,-1,85,88,94,101,108,115,121,125,-1,-1,-1,-1,-1,-1},
{ -1,-1,-1,-1,-1,-1,-1,89,95,102,109,116,122,-1,-1,-1,-1,-1,-1,-1},
{ -1,-1,-1,-1,-1,-1,-1,90,96,103,110,117,123,-1,-1,-1,-1,-1,-1,-1},
{ -1,-1,-1,-1,-1,-1,-1,-1,97,104,111,118,-1,-1,-1,-1,-1,-1,-1,-1},
};


#define NUMMCELLS 7

float loadData[NUMMCELLS+1];

int modLUT[NUMMCELLS] = { 2,3,4,5,0,1,6 };

#define BEAM_AM 10
Beam beams[BEAM_AM];

const int ledsPerStrip = 84;
DMAMEM int displayMemory[ledsPerStrip * 6];
COL_RGB copyMemory[ledsPerStrip * 8];
COL_RGB drawingMemory[ledsPerStrip * 8];

const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811_Dither leds(ledsPerStrip, displayMemory, copyMemory, drawingMemory, config);


Orb orbs[NUMMCELLS];

void set_ledLib(int pixel, byte r, byte g, byte b) {
	leds.setPixel(pixel, r, g, b);
}

uint32_t bufsize = ledsPerStrip * 24;
void reset_ledLib() {
	memset(drawingMemory, 0, bufsize);
}

void show_ledLib() {
	leds.show();
}


Color get_ledLib(int pixel) {
	Color pixelCol(drawingMemory[pixel].r, drawingMemory[pixel].g, drawingMemory[pixel].b, RGB_MODE);
	return pixelCol;
}


Segment seg[] = {
   Segment(0,41),  // 0
   Segment(42,83),  // 0
   Segment(84,125),  // 0
   Segment(126,167),  // 0
   Segment(168,209),  // 0
   Segment(210,251),  // 0
   Segment(252,293),  // 0
};

const int LED_AM = ledsPerStrip * 8;
byte segAm = sizeof(seg) / sizeof(Segment);
Colore colore(LED_AM, seg, segAm, beams, BEAM_AM, &set_ledLib, &get_ledLib, &show_ledLib, &reset_ledLib);


void setup() {
	Serial1.begin(2000000);
	leds.begin();

	for (int i = 0; i < NUMMCELLS; i++) {
		orbs[i].init(7, 10, i*137.508, WIDTH, HEIGHT);
	}

	orbs[0].setPos(6, 3.5);
	orbs[1].setPos(13, 3.5);
	orbs[2].setPos(3, 9.5);
	orbs[3].setPos(10, 9.5);
	orbs[4].setPos(17, 9.5);
	orbs[5].setPos(6, 15.5);
	orbs[6].setPos(13, 15.5);


	dataAvg[0] = 87868.52;
	dataAvg[1] = 85843.13;
	dataAvg[2] = 85452.88;
	dataAvg[3] = 82954.31;
	dataAvg[4] = 82930.22;
	dataAvg[5] = 83996.37;
	dataAvg[6] = 85785.13;
	//for (int i = 0; i < NUMMCELLS; i++) {
	//	dataAvg[i] = 42500;
	//}
}

elapsedMillis sincePulse = 0;
int nextPulse = 500;
int modID = 0;

float hue = 0;

float dt;

void loop() {
	checkSerial();

	for (int i = 0; i < NUMMCELLS; i++) {
		Color c((360 / NUMMCELLS)*i, 100, min(45, loadData[i]), HSB_MODE);
		orbs[modLUT[i]].size = min(500, loadData[i]*100)*0.2;
	}

	reset_ledLib();

	//Color c(hue, 100, 50, HSB_MODE);
	//for (int x = 0; x < 21; x++) {
	//	for (int y = 0; y < 20; y++) {


	//		leds.setPixel(ledMap[x][y], c.red(), c.green(), c.blue());
	//		leds.show();
	//		//delay(10);
	//	}
	//}
	//hue += 137.507764050;
	//if (hue >= 360) hue -= 360;

	//if (millis() > 15000) {
		for (int i = 0; i < NUMMCELLS; i++) {
			orbs[i].render();
		}
	//}

	leds.show();

	for (int i = 0; i < NUMMCELLS; i++) {
		orbs[i].move(dt);
	}

	calcDt();


	

	//colore.update(true);
	////printFramerate();
}


void printFramerate() {
	Serial.print("FrameRate: ");
	Serial.println(colore.getFPS()); // print framerate
}


uint32_t lastCalc; // variable to keep track of the loops per second

void calcDt() {
	uint32_t currTime = micros();
	dt = (currTime - lastCalc) / 1000000.;  // assume one frame per second
	lastCalc = currTime;
}