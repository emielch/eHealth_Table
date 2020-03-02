#include "Orb.h"


Orb::Orb()
{
	spd = { 0,0 };
	active = false;
}

void Orb::init(float x, float y, float _spd, float _size, Color _c, bool _stayAlive, bool _stayPos)
{
	active = true;
	inflating = true;
	pos = { x,y };
	spd = { random(-_spd, _spd), random(-_spd, _spd) };
	size = 0;
	sizeInit = _size;
	c = _c;
	stayAlive = _stayAlive;
	stayPos = _stayPos;
}

float bouncePow = 50;

void Orb::move(float dt)
{
	if (!active) return;

	if (inflating) {
		size += (sizeInit - size) * 5 * dt;
		if (size > sizeInit*0.95) {
			inflating = false;
		}
	}
	else if (!(stayAlive && currMode == 1)) size -= 5 * dt;
	if (size <= 0 && !(stayAlive && currMode == 1)) {
		active = false;
		return;
	}

	float hue = c.hue() + 20 * dt;
	if (hue >= 360) hue -= 360;
	c.setHSB(hue, c.saturation(), c.brightness());


	if (currMode == 1 && !stayPos) {
		// gravity pull towards pressure
		float  val, mag;
		Vector dist;

		for (int i = 0; i < NUMMCELLS; i++) {
			if (loadData[i] < 2) continue;
			dist.x = modMap[i][0] - pos.x;
			dist.y = modMap[i][1] - pos.y;

			mag = dist.mag();
			if (mag == 0) continue;
			val = min(1 / mag,2);
			val *= max(0, loadData[i]);

			dist.normalize();
			spd.add(dist*val*0.4);
		}

		// bounce from borders
		Vector posFromCenter{ pos.x - WIDTH / 2., pos.y - HEIGHT / 2. };
		if (posFromCenter.mag() > WIDTH / 2 - size / 2) {
			Vector towardsCenter = posFromCenter * -1;
			spd.add(towardsCenter*dt);
		}

		spd = spd * 0.99;
	}

	if(!stayPos) pos.add(spd*dt);

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


void addColorToLedID(Color col, int ledID, float fadeFac) {
	if (fadeFac != 0.) {
		COL_RGB pixelCol = leds.getPixelRGB(ledID);

		pixelCol.r = constrain(pixelCol.r + col.red()*fadeFac, 0, 255);
		pixelCol.g = constrain(pixelCol.g + col.green()*fadeFac, 0, 255);
		pixelCol.b = constrain(pixelCol.b + col.blue()*fadeFac, 0, 255);

		leds.setPixel(ledID, pixelCol.r, pixelCol.g, pixelCol.b);
	}
}



void Orb::render()
{
	if (!active) return;
	if (pos.x > WIDTH + size / 2 || pos.x < 0 - size / 2 || pos.y > HEIGHT + size / 2 || pos.y < 0 - size / 2) {
		return;
	}

	float rSize = size < 2 ? 2 : size;

	int x1 = pos.x - rSize / 2;
	int x2 = pos.x + rSize / 2;
	int y1 = pos.y - rSize / 2;
	int y2 = pos.y + rSize / 2;


	for (int x = x1; x <= x2; x++) {
		for (int y = y1; y <= y2; y++) {

			if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) continue;
			int ledID = ledMap[x][y];
			if (ledID == -1) continue;

			float Dx = x - pos.x;
			float Dy = y - pos.y;
			float dist = sqrt(Dx * Dx + Dy * Dy);

			float b = (rSize / 2) - dist;
			b = b / (rSize / 2);
			if (b < 0) b = 0;
			if (size < 1) b *= size / 2;

			leds.getPixelPointer(ledID)->add(COL_RGB{ c.red(),c.green(),c.blue() }, b);
			//addColorToLedID(c, ledID, b);
		}
	}
}

