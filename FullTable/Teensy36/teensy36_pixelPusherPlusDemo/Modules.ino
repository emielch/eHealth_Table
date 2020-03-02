Module modules[NUMMCELLS];
int neighborSpacing[6][2] = { { 4, -6 }, { 7, 0 }, { 4, 6 }, { -3, 6 }, { -7, 0 }, { -3, -6 } };
#define MODULE_LED_AM 42


#define NUMPRESSGROUPS 10
Module* pressGroupStartElems[NUMPRESSGROUPS];

void setup_modules() {
	for (int i = 0; i < NUMPRESSGROUPS; i++) {
		pressGroupStartElems[i] = NULL;
	}

	for (int i = 0; i < NUMMCELLS; i++) {
		modules[i].xPos = modMap[i][0];
		modules[i].yPos = modMap[i][1];

		for (int j = 0; j < 6; j++) {
			float searchPos[2] = { modMap[i][0] + neighborSpacing[j][0], modMap[i][1] + neighborSpacing[j][1] };
			modules[i].neighbors[j] = NULL;
			for (int k = 0; k < NUMMCELLS; k++) {
				if ((modMap[k][0] == searchPos[0] || modMap[k][0] == searchPos[0] - 1) && modMap[k][1] == searchPos[1]) {
					modules[i].neighbors[j] = &modules[k];
					break;
				}
			}
		}
	}

}

int emitDelay = 200;

void loop_modules(bool allowNewOrbs) {
	for (int i = 0; i < NUMMCELLS; i++) {
		if (loadData[i] > 2) {
			checkSerial();
			checkUSBSerial();

			if (!modules[i].pressed) {
				bool foundC = false;

				for (int j = 0; j < 6; j++) {
					if (modules[i].neighbors[j] != NULL && modules[i].neighbors[j]->pressed) {
						modules[i].c = modules[i].neighbors[j]->c;
						foundC = true;
						break;
					}
				}
				if (!foundC) modules[i].c = Color(random(0, 360), 100, 100, HSB_MODE);
			}

			COL_RGB modCol;
			modCol = { min(loadData[i] * modules[i].c.red()*0.1, 255), min(loadData[i] * modules[i].c.green()*0.1, 255), min(loadData[i] * modules[i].c.blue()*0.1, 255) };

			for (int j = 0; j < MODULE_LED_AM; j++) {
				leds.setPixel(i * 42 + j, modCol);
			}

			if (allowNewOrbs) {
				if (millis() > modules[i].lastOrbEmit + emitDelay) {
					if (spawnOrb(i, loadData[i], modules[i].c.hue())) modules[i].lastOrbEmit = millis();
				}
			}

			modules[i].pressed = true;
		}
		else modules[i].pressed = false;
	}
}
