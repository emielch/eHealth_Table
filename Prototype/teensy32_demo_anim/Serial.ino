#include "QuickStats.h"

#define NUMSAMPLES 5
QuickStats stats; //initialize an instance of this class



double dataAvg[NUMMCELLS+1];

unsigned long data[NUMMCELLS+1][NUMSAMPLES];
int currSample = 0;



String inputString = "";
int dataSerialPos = 0;


void checkSerial() {
	while (Serial1.available()) {
		char inChar = (char)Serial1.read();
		//Serial.print(inChar);
		
		if (inChar == '\t') {
			data[dataSerialPos][currSample] = inputString.toInt();
			dataSerialPos++;
			inputString = "";
		}
		else if (inChar == '\n' || inChar == '\r') {
			inputString = "";
			if (dataSerialPos != 0) {
				newData();
			}
			if (dataSerialPos != 0) {
				dataSerialPos = 0;
				currSample++;
				if (currSample >= NUMSAMPLES) currSample = 0;
			}
		}
		else inputString += inChar;
	}
}


void newData() {
	for (int j = 0; j < NUMMCELLS; j++) {
		float samples[NUMSAMPLES];
		for (int i = 0; i < NUMSAMPLES; i++) {
			samples[i] = data[j][i] * 0.01;
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
			Serial.print(val);
			Serial.print('\t');
		}
	}
	Serial.println();

}