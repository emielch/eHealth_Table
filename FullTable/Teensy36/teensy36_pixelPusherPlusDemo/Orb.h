// Orb.h

#ifndef _ORB_h
#define _ORB_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define WIDTH 105
#define HEIGHT 104

#define NUMMCELLS 199

#include <Colore.h>
#include "Vector.h"
#include <MultiWS2811.h>


extern int ledMap[105][104];
extern float modMap[199][2];
extern float* loadData;
extern MultiWS2811 leds;
extern byte currMode;


class Orb {

public:
	Vector pos, spd;
	float size, sizeInit;
	boolean active;
	boolean inflating;
	boolean stayAlive;
	boolean stayPos;

	Color c;
	
	Orb();
	void init(float x, float y, float _spd, float _size, Color _c, bool _stayAlive, bool _stayPos);
	void move(float dt);
	void render();
	void setPos(float x, float y) {
		pos.x = x;
		pos.y = y;
	}

};

#endif

