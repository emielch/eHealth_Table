

void checkSerial() {
	while (Serial1.available()) {
		char startChar = Serial1.read();
		if (startChar == '%') {
			// receive the data
			int count = Serial1.readBytes((char *)serialBuffer, sizeof(float) * NUMMCELLS);
			char endChar = Serial1.read();

			if (endChar == '#') {  // if the message is received properly, switch the pointers so that loadData points to the just received data
				float* tempPointer = loadData;
				loadData = serialBuffer;
				serialBuffer = tempPointer;

				/*for (int i = 0; i < NUMMCELLS; i++) {
					Serial.print(loadData[i]);
					Serial.print('\t');
				}
				Serial.println();*/

			}

		}
	}
}
