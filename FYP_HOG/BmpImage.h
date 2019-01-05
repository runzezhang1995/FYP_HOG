#pragma once
#include "common.h"
#include "Window.h"

class BmpImage {

public:
	unsigned char *pBmpBuf;//读入图像数据的指针
	double *magBuf;   // 梯度大小 
	unsigned char *angBuf; // 梯度方向分组 （0-8） 分9组

	int bmpWidth;//图像的宽
	int bmpHeight;//图像的高
	RGBQUAD *pColorTable;//颜色表指针
	
	int biBitCount;//图像类型，每像素位数
	int linebyte;
	vector<Window *> gtWindows;


	BmpImage(int height, int width, int bitCount, unsigned char * pBmpBuf, RGBQUAD* pColorTable);
	BmpImage(const char *fileName);
	BmpImage();

	bool readBmpFromFile(const char *fileName);
	bool saveBmpToFile(const char *fileName);
	unsigned char getPixelValue(int height, int width, int channel = -1);
	float* getCellHog(int x, int y, int width, int channel = -1);
	float* getHOGForWindowStartFrom(int x, int y, int channel = -1);
	bool isWindowContainPerson(Window* window);

};

