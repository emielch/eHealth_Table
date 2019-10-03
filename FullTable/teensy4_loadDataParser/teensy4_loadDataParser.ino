#include "QuickStats.h"

#define NUMSAMPLES 10
#define NUMMCELLS 67
QuickStats stats; //initialize an instance of this class

double dataAvg[NUMMCELLS + 1];
float loadData[NUMMCELLS + 1];

unsigned long data[NUMSAMPLES][NUMMCELLS + 1];
int currSample = 0;



String inputString = "";
int dataSerialPos = 0;


void setup() {
	Serial1.begin(2000000);
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
			int count = Serial1.readBytes((char *)data[currSample], sizeof(unsigned long)*NUMMCELLS);
			if (count == sizeof(unsigned long)*NUMMCELLS) {
				currSample++;
				if (currSample >= NUMSAMPLES) currSample = 0;
				counter++;
				newData();
			}
		}
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
			loadData[j] = (val - dataAvg[j])*0.0005;
			Serial.print(loadData[j]*5000+5000);
			Serial.print('\t');
		}
	}
	Serial.println();

}