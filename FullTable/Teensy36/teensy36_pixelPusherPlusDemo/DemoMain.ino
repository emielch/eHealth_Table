#define NUMMCELLS 199
float serialData1[NUMMCELLS];
float serialData2[NUMMCELLS];
float* loadData = serialData1;
float* serialBuffer = serialData2;

#define MODEAM 3
byte currMode = 0;
elapsedMillis sinceModeStart = 0;
unsigned long modeTimes[MODEAM] = { 35,25,30 };  // how long each mode will be playing in seconds


void demoSetup() {
	Serial1.begin(2000000);  // increase SERIAL1_RX_BUFFER_SIZE to 1024
	setup_modules();
	for (int i = 0; i < MODEAM; i++) {
		modeTimes[i] *= 1000;
	}
}



void demoLoop() {
	modeCheck();
	checkSerial();
	pressCombiCheck();
	reset_ledLib();

	switch (currMode) {
	case 0:
		loop_modules(true);
		break;
	case 1:
		// particle gravity
		loopScalingOrbs();
		break;
	case 2:
		loop_WarpGrid();
		break;
	default:
		break;
	}

	loopPS();

	if(sinceDemoStart>1000) leds.show();

	printFramerate();
}

void resetDemo() {
	resetOrbs();
	currMode = 0;
	sinceModeStart = 0;
}


void pressCombiCheck() {
	if (loadData[194] > 40 && loadData[195] < 10 && loadData[196] > 40 && loadData[197] < 10 && loadData[198] > 40) {
		switchMode();
	}
}


void switchMode() {
	if (sinceModeStart < 1000) return;

	currMode++;
	if (currMode >= MODEAM) currMode = 0;

	else if (currMode == 1) {
		setupScalingOrbs();
		for (int i = 0; i < 200; i++) {
			spawnGravityOrb();
		}
	}

	sinceModeStart = 0;
}


void modeCheck() {
	if (sinceModeStart > modeTimes[currMode]) {
		switchMode();
	}
}


void setPixel(int x, int y, COL_RGB col) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
	int ledID = ledMap[x][y];
	if (ledID == -1) return;
	leds.setPixel(ledID, col);
}

void setPixel(int x, int y, COL_RGB col, float fadeFac) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
	if (fadeFac == 0.) return;
	int ledID = ledMap[x][y];
	if (ledID == -1) return;

	leds.getPixelPointer(ledID)->add(col, fadeFac);
}

void setPixel(float _x, float _y, COL_RGB col) {
	for (int x = _x; x <= (int)_x + 1; x++) {
		for (int y = _y; y <= (int)_y + 1; y++) {
			float xDist = x - _x;
			float yDist = y - _y;
			float val = 1 - sqrt(sq(xDist) + sq(yDist))*1.41 / 2;
			setPixel(x, y, col, val);
		}
	}
}





float dt;
uint32_t lastCalc; // variable to keep track of the loops per second

void calcDt() {
	uint32_t currTime = micros();
	dt = (currTime - lastCalc) / 1000000.;  // assume one frame per second
	lastCalc = currTime;
}

unsigned long frameCount;
elapsedMicros sinceLastFPSPrint = 0;
unsigned long printDelay = 1000000;
void printFramerate() {
	frameCount++;
	if (sinceLastFPSPrint > printDelay) {
		Serial.print("FPS: ");
		Serial.println(frameCount / (sinceLastFPSPrint / 1000000.));
		sinceLastFPSPrint = 0;
		frameCount = 0;
	}
}

