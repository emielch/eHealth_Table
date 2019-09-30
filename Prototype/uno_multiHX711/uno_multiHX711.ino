
#define NUMMCELLS 7

int sck_pin = 2;
int dStart_pin = 3;

byte GAIN = 1;

byte ports[2][24];

unsigned long data[NUMMCELLS];



byte portMapping[14][2] = {
{1,0},
{1,1},
{1,2},
{1,3},
{1,4},
{1,5},
{1,6},
{1,7},
{0,0},
{0,1},
{0,2},
{0,3},
{0,4},
{0,5}
};


void setup() {
	Serial.begin(2000000);
	//Serial1.begin(2000000);
	pinMode(sck_pin, OUTPUT);

	for (int i = dStart_pin; i < dStart_pin + NUMMCELLS; i++) {
		pinMode(i, INPUT);
	}

}

int counter = 0;
unsigned long measureTime = 0;

unsigned long readingPorts;
unsigned long waiting;
unsigned long startLoop;
unsigned long processPorts;

void loop() {
	if (millis() > measureTime + 1000) {
		measureTime = millis();

		//Serial.print("SPS: ");
		//Serial.println(counter);
		//Serial.print(waiting - startLoop);
		//Serial.print('\t');
		//Serial.print(readingPorts - waiting);
		//Serial.print('\t');
		//Serial.println(processPorts-readingPorts);
		

		counter = 0;
	}

	startLoop = micros();

	for (int j = 0; j < NUMMCELLS; j++) {
		data[j] = 0;
	}

	for (int j = 0; j < NUMMCELLS; j++) {  // wait for data being available on all ADC's
		while (digitalRead(dStart_pin + j));
	}

	waiting = micros();

	boolean dout;

	for (uint8_t i = 0; i < (24 + GAIN); i++) { // read 24 bit data + set gain and start next conversion
		delayMicroseconds(1); // required for faster mcu's?
		digitalWrite(sck_pin, 1);
		delayMicroseconds(1);

		if (i < 24) {
			ports[0][i] = PINB;
			ports[1][i] = PIND;
		}



		digitalWrite(sck_pin, 0);
	}

	readingPorts = micros();

	for (uint8_t i = 0; i < 24; i++) {
		for (int j = 0; j < NUMMCELLS; j++) {

			dout = digitalReadFastPort(dStart_pin + j,i);
			data[j] = data[j] << 1;
			if (dout) {
				data[j]++;
			}
		}
	}

	for (int j = 0; j < NUMMCELLS; j++) {
		data[j] = data[j] ^ 0x800000; // if out of range (min), change to 0

		//if (j > 1)continue;
		Serial.print(data[j]);
		Serial.print('\t');
	}
	Serial.println();

	processPorts = micros();

	counter++;
}


boolean digitalReadFastPort(byte pin, byte time) {
	byte port = portMapping[pin][0];
	byte bit = portMapping[pin][1];

	byte mask = (1 << bit);

	return ports[port][time] & mask;
}