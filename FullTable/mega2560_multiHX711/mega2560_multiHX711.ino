
#define NUMMCELLS 67

int sck_pin = 2;
int dStart_pin = 3;

byte GAIN = 1;

byte ports[11][24];

unsigned long data[NUMMCELLS];

byte portMapping[70][2] = {
	{4,0},
	{4,1},
	{4,4},
	{4,5},
	{6,5},
	{4,3},
	{7,3},
	{7,4},
	{7,5},
	{7,6},
	{1,4},
	{1,5},
	{1,6},
	{1,7},
	{8,1},
	{8,0},
	{7,1},
	{7,0},
	{3,3},
	{3,2},
	{3,1},
	{3,0},
	{0,0},
	{0,1},
	{0,2},
	{0,3},
	{0,4},
	{0,5},
	{0,6},
	{0,7},
	{2,7},
	{2,6},
	{2,5},
	{2,4},
	{2,3},
	{2,2},
	{2,1},
	{2,0},
	{3,7},
	{6,2},
	{6,1},
	{6,0},
	{10,7},
	{10,6},
	{10,5},
	{10,4},
	{10,3},
	{10,2},
	{10,1},
	{10,0},
	{1,3},
	{1,2},
	{1,1},
	{1,0},
	{5,0},
	{5,1},
	{5,2},
	{5,3},
	{5,4},
	{5,5},
	{5,6},
	{5,7},
	{9,0},
	{9,1},
	{9,2},
	{9,3},
	{9,4},
	{9,5},
	{9,6},
	{9,7}
};


void setup() {
	Serial.begin(2000000);
	pinMode(sck_pin, OUTPUT);

	for (int i = dStart_pin; i < dStart_pin + NUMMCELLS; i++) {
		pinMode(i, INPUT);
	}

}

int counter = 0;
unsigned long measureTime = 0;


void loop() {
	/*if (millis() > measureTime + 1000) {
		measureTime = millis();

		Serial.print("SPS: ");
		Serial.println(counter);
		
		counter = 0;
	}
	counter++;*/

	for (int j = 0; j < NUMMCELLS; j++) {
		data[j] = 0;
	}

	unsigned long waitStart = millis();
	for (int j = 0; j < NUMMCELLS; j++) {  // wait for data being available on all ADC's
		while (digitalRead(dStart_pin + j) && millis()<waitStart);
	}

	for (uint8_t i = 0; i < (24 + GAIN); i++) { // read 24 bit data + set gain and start next conversion
		delayMicroseconds(1); // required for faster mcu's?
		digitalWrite(sck_pin, 1);
		delayMicroseconds(1);

		if (i < 24) {
			ports[0][i] = PINA;
			ports[1][i] = PINB;
			ports[2][i] = PINC;
			ports[3][i] = PIND;
			ports[4][i] = PINE;
			ports[5][i] = PINF;
			ports[6][i] = PING;
			ports[7][i] = PINH;
			ports[8][i] = PINJ;
			ports[9][i] = PINK;
			ports[10][i] = PINL;
		}

		digitalWrite(sck_pin, 0);
	}

	boolean dout;

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
	}

	Serial.print('%');
	Serial.write((char*)data, sizeof(data));
}


boolean digitalReadFastPort(byte pin, byte time) {
	byte port = portMapping[pin][0];
	byte bit = portMapping[pin][1];

	byte mask = (1 << bit);

	return ports[port][time] & mask;
}