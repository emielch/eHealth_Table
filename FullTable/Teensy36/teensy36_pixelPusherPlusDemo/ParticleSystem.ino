
#define NUMORBS 400
#define NUMMCELLS 199
Orb orbs[NUMORBS];

elapsedMillis sinceGravityOrbSpawn = 0;


void loopPS() {
	for (int i = 0; i < NUMORBS; i++) {
		orbs[i].move(dt);
	}

	for (int i = 0; i < NUMORBS; i++) {
		orbs[i].render();
		checkSerial();
		checkUSBSerial();
	}

	if (currMode == 1 && sinceGravityOrbSpawn >500) {
		orbs[random(NUMMCELLS, NUMORBS)].stayAlive = false;
		spawnGravityOrb();
	}
}

void resetOrbs() {
	for (int i = 0; i < NUMORBS; i++) {
		orbs[i].active = false;
	}
}

bool spawnOrb(int modID, float pressure, int hue) {
	Orb* newOrb = getInactiveOrb();
	if (newOrb == NULL) return false;

	newOrb->init(modMap[modID][0], modMap[modID][1], constrain(pressure, 10, 25), constrain(pressure, 5, 20), Color(hue + random(-10, 10), min(random(60, 120), 100), 100, HSB_MODE), false, false);
	return true;
}

bool spawnGravityOrb() {
	Orb* newOrb = getInactiveOrb();
	if (newOrb == NULL) return false;

	newOrb->init(random(25, 75), random(25, 75), random(5, 10), random(2, 5), Color(random(0, 360), min(random(60, 120), 100), 100, HSB_MODE), true, false);
	sinceGravityOrbSpawn = 0;
	return true;
}


void setupScalingOrbs() {
	for (int i = 0; i < NUMMCELLS; i++) {
		orbs[i].init(modMap[i][0], modMap[i][1], 0, 0, Color(modMap[i][0]*2.5-40, 100, 100, HSB_MODE), true, true);
	}
}

void loopScalingOrbs() {
	for (int i = 0; i < NUMMCELLS; i++) {
		orbs[i].size = max(0, min(20, loadData[i]));
	}
}



Orb* getInactiveOrb() {
	for (int i = 0; i < NUMORBS; i++) {
		if (!orbs[i].active) return &orbs[i];
	}
	return NULL;
}
