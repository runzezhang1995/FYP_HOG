#include "BmpImage.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846

#endif // !1

BmpImage::BmpImage() {
	this->bmpHeight = 0;
	this->bmpWidth = 0;
	this->biBitCount = 0;
	this->pBmpBuf = nullptr;
	this->pColorTable = nullptr;
}



BmpImage::BmpImage(int height = 0, int width = 0, int bitCount = 0, unsigned char * pBmpBuf = nullptr, RGBQUAD* pColorTable = nullptr) {
	this->bmpHeight = height;
	this->bmpWidth = width;
	this->biBitCount = bitCount;
	this->pBmpBuf = pBmpBuf;
	this->pColorTable = pColorTable;
	linebyte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
}


BmpImage::BmpImage(const char *fileName) {
	readBmpFromFile(fileName);
	//float maxMag = 0;
	//float minMag = 360;
	for (short h = 0; h < bmpHeight; h++) {
		for (short w = 0; w < bmpWidth; w++) {
			if (biBitCount == 24) {
				for (short c = 0; c < 3; c++) {
					unsigned char 	xFilterValue = getPixelValue(h, w + 1, c) - getPixelValue(h, w - 1, c);
					unsigned char  yFilterValue = getPixelValue(h + 1, w, c) - getPixelValue(h - 1, w, c);
					
					*(magBuf + h*linebyte + w * 3 + c) = sqrt(pow(xFilterValue, 2) + pow(yFilterValue, 2));

					double ang = atan2(yFilterValue, xFilterValue) * 180 / M_PI;
					ang += ang < 0 ? 180 : 0;
					unsigned char angClass = ((int)ang) / 20;
					*(angBuf + h*linebyte + w * 3 + c) = angClass;
				}
			}
			else if (biBitCount == 8) {
				char 	xFilterValue = getPixelValue(h, w + 1) - getPixelValue(h, w - 1);
				char yFilterValue = getPixelValue(h + 1, w) - getPixelValue(h - 1, w);
				*(magBuf + h*linebyte + w) = sqrt(pow(xFilterValue, 2) + pow(yFilterValue, 2));
				

				double ang = atan2(yFilterValue, xFilterValue) * 180 / M_PI;
				ang -= 0.0001;
				ang += ang < 0 ? 180 : 0;
				if (ang < 0) {
					ang = 0;
				}
				unsigned char angClass = ((int)ang) / BIN_SIZE;
				*(angBuf + h*linebyte + w) = angClass;
				
				//cout <<*(magBuf + h*linebyte + w) << " ";
				//if (*(magBuf + h*linebyte + w) > maxMag) maxMag = *(magBuf + h*linebyte + w);
				//if (*(magBuf + h*linebyte + w) < minMag) minMag = *(magBuf + h*linebyte + w);

			}
		}
	}
	//cout << "max ang:" << maxMag << endl;

	//cout << "min ang:" << minMag << endl;

	delete[] pBmpBuf;
}




unsigned char BmpImage::getPixelValue(int height, int width, int channel) {
	if (!pBmpBuf) {
		return NULL;
	}
	// make the margin pixel value equal to the boundary of the image 
	height = height < 0 ? 0 : (height >= bmpHeight ? bmpHeight - 1 : height);
	width = width < 0 ? 0 : (width >= bmpWidth ? bmpWidth - 1 : width);

	if (channel == -1) {
		return  *(pBmpBuf + height*linebyte + width);
	}
	else {
		return  *(pBmpBuf + height*linebyte + width * 3 + channel);
	}
}



bool BmpImage::readBmpFromFile(const char *fileName) {
	FILE *fp;
	fopen_s(&fp, fileName, "rb");//�����ƶ���ʽ��ָ����ͼ���ļ�

	if (!fp)
		return 0;

	//����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER

	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	//����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head��

	BITMAPINFOHEADER head;

	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp); //��ȡͼ����ߡ�ÿ������ռλ������Ϣ

	bmpWidth = head.biWidth;

	bmpHeight = head.biHeight;

	biBitCount = head.biBitCount;//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����

	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256

	if (biBitCount == 8)
	{
		//������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�
		pColorTable = new RGBQUAD[256];

		fread(pColorTable, sizeof(RGBQUAD), 256, fp);

	}
	else {
		pColorTable = new RGBQUAD[256];
		for (int i = 0; i<256; i++)
		{
			pColorTable[i].rgbBlue = i;
			pColorTable[i].rgbGreen = i;
			pColorTable[i].rgbRed = i;
			pColorTable[i].rgbReserved = 0;
		}
	}

	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�

	pBmpBuf = new unsigned char[lineByte * bmpHeight];
	
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
	
		
	// ����ɫͼת��Ϊ�Ҷ�ͼ
	if (biBitCount == 24) {
		biBitCount = 8;
		int newlineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
		unsigned char *newBuf = new unsigned char[newlineByte * bmpHeight];
		int i = 0;

		for (int y = 0; y < bmpHeight; y++) {
			for (int x = 0; x < bmpWidth; x++) {
				*(newBuf + y * newlineByte + x) = (*(pBmpBuf + y * lineByte + x * 3) * 30 + *(pBmpBuf + y * lineByte + x * 3 + 1) * 59 + *(pBmpBuf + y * lineByte + x * 3 + 2) * 11 + 50) / 100;
			}
		}

		delete[] pBmpBuf;
		pBmpBuf = newBuf;
		biBitCount = 8;
		linebyte = newlineByte;
	}

	magBuf = new double[linebyte * bmpHeight];
	angBuf = new unsigned char[linebyte * bmpHeight];

	fclose(fp);//�ر��ļ�

	return 1;//��ȡ�ļ��ɹ�
}


bool  BmpImage::saveBmpToFile(const char *fileName) {
	//���λͼ����ָ��Ϊ0����û�����ݴ��룬��������

	if (!pBmpBuf)
		return 0;

	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0

	int colorTablesize = 0;

	if (biBitCount == 8)
		colorTablesize = 1024;

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���

	//�Զ�����д�ķ�ʽ���ļ�

	FILE *fp;
	fopen_s(&fp, fileName, "wb");

	if (fp == 0)
		return 0;

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ

	BITMAPFILEHEADER fileHead;

	fileHead.bfType = 0x4D42;//bmp����

							 //bfSize��ͼ���ļ�4����ɲ���֮��

	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + linebyte*bmpHeight;

	fileHead.bfReserved1 = 0;

	fileHead.bfReserved2 = 0;

	//bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��

	fileHead.bfOffBits = 54 + colorTablesize;

	//д�ļ�ͷ���ļ�

	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ

	BITMAPINFOHEADER head;

	head.biBitCount = biBitCount;

	head.biClrImportant = 0;

	head.biClrUsed = 0;

	head.biCompression = 0;

	head.biHeight = bmpHeight;

	head.biPlanes = 1;

	head.biSize = 40;

	head.biSizeImage = linebyte*bmpHeight;

	head.biWidth = bmpWidth;

	head.biXPelsPerMeter = 0;

	head.biYPelsPerMeter = 0;

	//дλͼ��Ϣͷ���ڴ�

	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//����Ҷ�ͼ������ɫ��д���ļ� 

	if (biBitCount == 8)
		fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

	//дλͼ���ݽ��ļ�

	fwrite(this->pBmpBuf, bmpHeight*linebyte, 1, fp);

	//�ر��ļ�

	fclose(fp);

	return 1;
}

float* BmpImage::getHOGForWindowStartFrom(int x, int y, int channel) {
	float *hog = new float[ARRAY_ALL];
	int hogofCellsize = CELL_W_NVM * CELL_H_NVM * BIN_NVM;
	float * hogOfCells = new float[CELL_W_NVM * CELL_H_NVM * BIN_NVM];
	// ��ȡÿ��cell��hog
	for (int cy = 0; cy < CELL_H_NVM; cy++) {
		for (int cx = 0; cx < CELL_W_NVM; cx++) {
			float*hogOfCell = getCellHog(x + cx * CELL_SIZE, y + cy * CELL_SIZE, CELL_SIZE);
			for (int i = 0; i < BIN_NVM; i++) {
				*(hogOfCells + cy * CELL_W_NVM * BIN_NVM + cx * BIN_NVM  +i) = hogOfCell[i];
			}
			delete[] hogOfCell;
		}
	}


	// ��ȡÿ��block ��hog, �Ƚ��й�һ��
	for (int by = 0; by < BLOCK_H_NVM; by++) {
		for (int bx = 0; bx < BLOCK_W_NVM; bx++) {
			float* hogOfBlock = new float[BLOCK_SIZE * BLOCK_SIZE * BIN_NVM];
			for (int i = 0; i < BLOCK_SIZE; i++) {
				for (int j = 0; j < BLOCK_SIZE; j++) {
					for (int k = 0; k < BIN_NVM; k++) {
						int currentPosition = (by + i) * BLOCK_W_NVM * BIN_NVM + (bx + j) * BIN_NVM + k;
						hogOfBlock[i * BLOCK_SIZE * BIN_NVM + j * BIN_NVM + k] = *(hogOfCells + (by + i) * CELL_W_NVM * BIN_NVM + (bx + j) * BIN_NVM +k);
					}
				}
			}
			int size = BLOCK_SIZE * BLOCK_SIZE * BIN_NVM;
			float err = 0.000001;
			float sum = err;
			for (int i = 0; i < size; i++) {
				sum += hogOfBlock[i] * hogOfBlock[i];
			}
			sum = sqrt(sum);
			for (int i = 0; i < size; i++) {
				hogOfBlock[i] /= sum;
			}
			for (int i = 0; i < size; i++) {
				*(hog + by * BLOCK_W_NVM * size + bx * size + i) = hogOfBlock[i];
			}

			delete[] hogOfBlock;
		}
	}
	delete[] hogOfCells;
	return hog;
}

bool BmpImage::isWindowContainPerson(Window* window)
{
	for (vector<Window *>::iterator iter = gtWindows.begin(); iter != gtWindows.end(); iter++)
	{
		if ((*iter)->isOverLapWithWindow(window)) {
			return true;
		}
	}
	return false;
}

float* BmpImage::getCellHog(int x, int y, int width, int channel) {
	float *fbin = new float[BIN_NVM];
	memset(fbin, 0, sizeof(float) * BIN_NVM);

	for (int i = y; i < y + width; i++) { // ����
		for (int j = x; j < x + width; j++) { // ����
			*(fbin+(int)*(angBuf + i*linebyte + j)) += (float)*(magBuf + i*linebyte + j);
		}
	}
	return fbin;

}