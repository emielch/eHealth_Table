// 
// 
// 

#include "Orb.h"
#define WIDTH 21
#define HEIGHT 20

Orb::Orb()
{
	xSpd = 0;
	ySpd = 0;
}

void Orb::init(float _size, float _spd, float _hue, int _xBound, int _yBound)
{
	xBound = _xBound;
	yBound = _yBound;

	xPos = random(0, xBound);
	yPos = random(0, yBound);
	xSpd = random(-_spd, _spd);
	ySpd = random(-_spd, _spd);;
	size = _size;
	hue = _hue;
	bri = 100;

}

float bouncePow = 50;

void Orb::move(float dt)
{
	hue += 5 * dt;
	if (hue >= 360) hue -= 360;
	c.setHSB(hue, 100, bri);


	//xPos += xSpd * dt;
	//yPos += ySpd * dt;

	if (xPos > xBound) {
		xSpd -= bouncePow * dt;
	}
	else if (xPos < 0) {
		xSpd += bouncePow * dt;
	}

	if (yPos > yBound) {
		ySpd -= bouncePow * dt;
	}
	else if (yPos < 0) {
		ySpd += bouncePow * dt;
	}
}

bool doOverlap(int xs1, int xe1, int ys1, int ye1, int xs2, int xe2, int ys2, int ye2)
{
	// If one rectangle is on left side of other 
	if (xs1 > xe2 || xs2 > xe1)
		return false;

	// If one rectangle is above other 
	if (ys1 > ye2 || ys2 > ye1)
		return false;

	return true;
}

void Orb::render()
{
	if (bri == 0) return;
	int x1 = xPos - size / 2;
	int x2 = xPos + size / 2;
	int y1 = yPos - size / 2;
	int y2 = yPos + size / 2;


	for (int x = x1; x < x2; x++) {
		for (int y = y1; y < y2; y++) {

			if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) continue;

			float Dx = x - xPos;
			float Dy = y - yPos;
			float dist = sqrt(Dx * Dx + Dy * Dy);

			float bri = (size / 2) - dist;
			bri = bri / (size / 2);
			if (bri < 0) bri = 0;

			int ledID = ledMap[x][y];
			Color pixCol = get_ledLib(ledID);
			pixCol.add(c, bri);

			set_ledLib(ledID, pixCol.red(), pixCol.green(), pixCol.blue());
		}
	}
}
