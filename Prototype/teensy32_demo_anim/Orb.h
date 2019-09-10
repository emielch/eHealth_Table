// Orb.h

#ifndef _ORB_h
#define _ORB_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Colore.h>


extern int ledMap[21][20];
extern const int ledsPerStrip;
extern void set_ledLib(int pixel, byte r, byte g, byte b);
extern Color get_ledLib(int pixel);


class Orb {

public:
	float xPos, yPos, size, xSpd, ySpd;
	Orb();
	void init(float _size, float _spd, float _hue, int _xBound, int _yBound);
	void move(float dt);
	void render();
	void setBri(float _bri) { bri = _bri; }
	void setPos(float x, float y) {
		xPos = x;
		yPos = y;
	}

	

private:

	Color c;
	float hue, bri;

	int xBound, yBound;


};

#endif

