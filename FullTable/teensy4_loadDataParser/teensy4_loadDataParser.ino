#include "QuickStats.h"

#define NUMSAMPLES 10
#define CELLSPERPORT 67
#define NUMMCELLS CELLSPERPORT*3
QuickStats stats; //initialize an instance of this class

double dataAvg[NUMMCELLS];
float loadData[NUMMCELLS];

unsigned long data[NUMSAMPLES][NUMMCELLS];
int currSample = 0;
bool newdata1;
bool newdata2;
bool newdata3;


void setup() {
	Serial1.begin(2000000);
	Serial2.begin(2000000);
	Serial3.begin(2000000);
}


int counter = 0;
unsigned long measureTime = 0;

void loop() {
	/*if (millis() > measureTime + 1000) {
		measureTime = millis();

		Serial.print("SPS: ");
		Serial.println(counter);
		counter = 0;
	}*/
	checkSerial();
}

void checkSerial() {
	while (Serial1.available()) {
		int startChar = Serial1.read();
		if (startChar == '%') {
			// receive the data
			int count = Serial1.readBytes((char *)data[currSample], sizeof(unsigned long)*CELLSPERPORT);
			if (count == sizeof(unsigned long)*CELLSPERPORT) {
				newdata1 = true;
			}
		}
	}

	while (Serial2.available()) {
		int startChar = Serial2.read();
		if (startChar == '%') {
			// receive the data
			int count = Serial2.readBytes((char *)data[currSample]+ sizeof(unsigned long)*CELLSPERPORT, sizeof(unsigned long)*CELLSPERPORT);
			if (count == sizeof(unsigned long)*CELLSPERPORT) {
				newdata2 = true;
			}
		}
	}

	while (Serial3.available()) {
		int startChar = Serial3.read();
		if (startChar == '%') {
			// receive the data
			int count = Serial3.readBytes((char *)data[currSample]+ sizeof(unsigned long)*CELLSPERPORT *2, sizeof(unsigned long)*CELLSPERPORT);
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

		//for (int j = 0; j < NUMMCELLS; j++) {
		//	Serial.print(data[currSample][j]);
		//	Serial.print('\t');
		//}
		//Serial.println();
		//counter++;
		newData();
	}

}

//void printData() {
//	for (int j = 0; j < NUMMCELLS; j++) {
//		Serial.print(data[j]);
//		Serial.print('\t');
//	}
//	Serial.println();
//}


void newData() {
	for (int j = 0; j < NUMMCELLS; j++) {
		float samples[NUMSAMPLES];
		for (int i = 0; i < NUMSAMPLES; i++) {
			samples[i] = data[i][j] * 0.01;
		}

		stats.bubbleSort(samples, NUMSAMPLES);

		float iqr = samples[int(NUMSAMPLES*0.75)] - samples[int(NUMSAMPLES*0.25)];

		int s = 0;
		float val = 0;;
		for (int i = 0; i < NUMSAMPLES; i++) {
			if (samples[i] < samples[int(NUMSAMPLES*0.75)] + 1.5*iqr && samples[i] > samples[int(NUMSAMPLES*0.25)] - 1.5*iqr) {
				s++;
				val += samples[i];
			}
		}

		val /= s;


		if (!isnan(val)) {
			if (iqr < 4) {
				dataAvg[j] = dataAvg[j] * 0.997 + val * 0.003;
			}
			loadData[j] = (val - dataAvg[j])*0.005;
			Serial.print(loadData[j]);
			Serial.print('\t');
		}else {
			Serial.print(0);
			Serial.print('\t');
		}
	}
	Serial.println();

}