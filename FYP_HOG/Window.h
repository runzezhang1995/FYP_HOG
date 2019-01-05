#pragma once
#include "common.h"


class Window {

public:
	int x;  // position of center point in x  / pixel
	int y; // position of center point in y  / pixel
	int width; // width of window  / pixel
	int height; // height of window / pixel


	Window();
	Window(float fx, float fy, float fw, float fh, int imageWidth, int imageHeight);
	Window(int x, int y, int fw, int fh);

	bool isOverLapWithWindow(Window *window);

};

