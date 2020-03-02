// Module.h

#ifndef _MODULE_h
#define _MODULE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Colore.h"


class Module {

public:
	Module() {};
	int id;
	float xPos, yPos;
	Module* neighbors[6];
	bool pressed = false;
	Color c;

	unsigned long lastOrbEmit = 0;

};


#endif

