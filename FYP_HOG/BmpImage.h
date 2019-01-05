#pragma once
#include "common.h"
#include "Window.h"

class BmpImage {

public:
	unsigned char *pBmpBuf;//����ͼ�����ݵ�ָ��
	double *magBuf;   // �ݶȴ�С 
	unsigned char *angBuf; // �ݶȷ������ ��0-8�� ��9��

	int bmpWidth;//ͼ��Ŀ�
	int bmpHeight;//ͼ��ĸ�
	RGBQUAD *pColorTable;//��ɫ��ָ��
	
	int biBitCount;//ͼ�����ͣ�ÿ����λ��
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

