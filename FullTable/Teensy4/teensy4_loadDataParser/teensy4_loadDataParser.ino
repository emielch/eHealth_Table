#include "QuickStats.h"
#include <EEPROM.h>

#define NUMSAMPLES 10
#define CELLSPERPORT 67
#define NUMCELLS CELLSPERPORT*3
#define NUMACTUALCELLS 199
QuickStats stats; //initialize an instance of this class

int loadLUT[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,22,14,15,16,17,18,19,20,21,34,47,33,32,31,30,29,28,27,26,25,24,23,35,36,37,38,39,40,41,42,43,44,45,46,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,135,134,161,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198 };

double dataAvg[NUMACTUALCELLS];
float loadData[NUMACTUALCELLS];

unsigned long data[NUMSAMPLES][NUMCELLS];
int currSample = 0;
bool newdata1;
bool newdata2;
bool newdata3;

bool continuousCalibration = true;
bool streamData = false;

HardwareSerial* loadSer1 = &Serial1;
HardwareSerial* loadSer2 = &Serial3;
HardwareSerial* loadSer3 = &Serial4;
//HardwareSerial* ledSer = &Serial6;


void setup() {
	loadSer1->begin(2000000);
	loadSer2->begin(2000000);
	loadSer3->begin(2000000);
	//ledSer->begin(2000000);

	loadAvg();
}


int counter = 0;
unsigned long measureTime = 0;

void loop() {
	checkSerial();
	checkUSBSerial();
}

void checkUSBSerial() {
	while (Serial.available()) {
		char inChar = Serial.read();

		if (inChar == '?') {
			// when the video application asks, give it all our info
			// for easy and automatic configuration
			Serial.print("LOAD");
			Serial.write(',');
			Serial.print(NUMACTUALCELLS);
			Serial.println();
		}
		else if (inChar == 's') {
			saveAvg();
		}
		else if (inChar == 'l') {
			loadAvg();
		}
		else if (inChar == 'c') {
			continuousCalibration = true;
		}
		else if (inChar == 'x') {
			continuousCalibration = false;
		}
		else if (inChar == 'd') {
			streamData = true;
		}
		else if (inChar == 'f') {
			streamData = false;
		}

	}
}


void checkSerial() {
	while (loadSer1->available()) {
		int startChar = loadSer1->read();
		if (startChar == '%') {
			// receive the data
			int count = loadSer1->readBytes((char *)data[currSample], sizeof(unsigned long)*CELLSPERPORT);
			if (count == sizeof(unsigned long)*CELLSPERPORT) {
				newdata1 = true;
			}
		}
	}

	while (loadSer2->available()) {
		int startChar = loadSer2->read();
		if (startChar == '%') {
			// receive the data
			int count = loadSer2->readBytes((char *)data[currSample] + sizeof(unsigned long)*CELLSPERPORT, sizeof(unsigned long)*CELLSPERPORT);
			if (count == sizeof(unsigned long)*CELLSPERPORT) {
				newdata2 = true;
			}
		}
	}

	while (loadSer3->available()) {
		int startChar = loadSer3->read();
		if (startChar == '%') {
			// receive the data
			int count = loadSer3->readBytes((char *)data[currSample] + sizeof(unsigned long)*CELLSPERPORT * 2, sizeof(unsigned long)*CELLSPERPORT);
			if (count == sizeof(unsigned long)*CELLSPERPORT) {
				newdata3 = true;
			}
		}
	}


	if (newdata1 && newdata2 && newdata3) {
		newdata1 = false;
		newdata2 = false;
		newdata3 = false;
		currSample++;
		if (currSample >= NUMSAMPLES) currSample = 0;
		newData();
	}

}



void newData() {
	for (int k = 0; k < NUMACTUALCELLS; k++) {
		int j = loadLUT[k];
		float samples[NUMSAMPLES];
		for (int i = 0; i < NUMSAMPLES; i++) {
			samples[i] = data[i][j] * 0.01;
		}

		stats.bubbleSort(samples, NUMSAMPLES);

		float iqr = samples[int(NUMSAMPLES*0.666)] - samples[int(NUMSAMPLES*0.333)];

		int s = 0;
		float val = 0;;
		for (int i = 0; i < NUMSAMPLES; i++) {
			if (samples[i] < samples[int(NUMSAMPLES*0.666)] + 1.2*iqr && samples[i] > samples[int(NUMSAMPLES*0.333)] - 1.2*iqr) {
				s++;
				val += samples[i];
			}
		}

		val /= s;


		if (!isnan(val)) {
			if (continuousCalibration && iqr < 4) {
				dataAvg[k] = dataAvg[k] * 0.999 + val * 0.001;
			}
			loadData[k] = (val - dataAvg[k])*0.005;
			/*Serial.print(loadData[k]);
			Serial.print('\t');*/
		}
		/*else {
			Serial.print(0);
			Serial.print('\t');
		}*/
	}
	//Serial.println();

	if (streamData) {
		Serial.print('%');
		Serial.write((char*)loadData, sizeof(loadData));
		Serial.print('#');
	}
}


void saveAvg() {
	float tempVal;
	for (int i = 0; i < NUMACTUALCELLS; i++) {
		tempVal = dataAvg[i];
		EEPROM.put(i * sizeof(float), tempVal);
	}
}

void loadAvg() {
	float tempVal;
	for (int i = 0; i < NUMACTUALCELLS; i++) {
		EEPROM.get(i * sizeof(float), tempVal);
		dataAvg[i] = tempVal;
	}
}
