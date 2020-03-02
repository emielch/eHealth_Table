#include "Vector.h"

int gridLineOffset = 20;
COL_RGB gridCol = { 200,200,80 };


void loop_WarpGrid() {

	for (int y = 0; y < HEIGHT; y += gridLineOffset) {
		for (float x = 0; x < WIDTH; x+=1) {
			drawWarpPixel(x, y);
		}
		checkSerial();
		checkUSBSerial();
	}
	for (int x = 0; x < WIDTH; x += gridLineOffset) {
		for (float y = 0; y < HEIGHT; y+=1) {
			drawWarpPixel(x, y);
		}
		checkSerial();
		checkUSBSerial();
	}
	//drawWarpPixel(WIDTH/2, HEIGHT/2);
}




void drawWarpPixel(float x, float y) {
	float xDist, yDist, xVect, yVect, val, dist;
	float newX = x;
	float newY = y;
	gridCol = { (int)x * 2,220 - (int)x * 2,(int)y*2 };

	for (int i = 0; i < NUMMCELLS; i++) {
		if (loadData[i] < 5) continue;
		xDist = modMap[i][0] - x;
		yDist = modMap[i][1] - y;
		val = max(0, loadData[i]-5)*0.05;
		dist = sqrt(sq(xDist) + sq(yDist));

		if (abs(xDist) > abs(yDist)) {
			yVect = yDist / abs(xDist);
			if (xDist > 0) xVect = 1;
			else xVect = -1;
		}
		else {
			xVect = xDist / abs(yDist);
			if (yDist > 0) yVect = 1;
			else yVect = -1;
		}

		if(dist!=0) dist = (1 / dist)*100;
		xVect *= dist * val;
		yVect *= dist * val;

		if (abs(xVect) > abs(xDist)) xVect = xDist;
		if (abs(yVect) > abs(yDist)) yVect = yDist;


		newX += xVect;
		newY += yVect;
	}

	setPixel(newX, newY, gridCol);

}