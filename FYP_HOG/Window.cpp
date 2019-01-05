#include "Window.h"

Window::Window(){
	x = -1;
	y = -1; 
	width = -1;
	height = -1;
}

Window::Window(float fx, float fy, float fw, float fh, int imageWidth, int imageHeight) {
	x = (int)((float)imageWidth * fx);
	y = (int)((float)imageHeight * fy);
	width = (int)((float)imageWidth * fw);
	height = (int)((float)imageHeight * fh);
}

Window::Window(int nx, int ny, int fw, int fh) {
	x = nx;
	y = ny;
	width = fw;
	height = fh;
}

bool Window::isOverLapWithWindow(Window * window)
{
	int endx = max(x + width / 2, window->x + window->width / 2);
	int startx = min(x - width / 2, window->x - window->width / 2);
	int endy = max(y + height / 2, window->y + window->height / 2);
	int starty = min(y - height / 2, window->y - window->height / 2);
	int overLapWidth = width + window->width - (endx - startx);
	int overLapHeight = height + window->height - (endy - starty);

	if (overLapHeight <= 0 || overLapWidth <= 0) {
		return false;
	}
	else {
		int area = overLapHeight * overLapWidth;
		int area1 = width * height;
		int area2 = window->width * window->height;
		float ratio = (float)area / (float)(area1 + area2 - area);
		if (ratio > 0.5) {
			return true;
		}
		else {
			return false;
		}
	}
}

