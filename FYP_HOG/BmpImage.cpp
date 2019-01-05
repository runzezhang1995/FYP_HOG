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
	fopen_s(&fp, fileName, "rb");//二进制读方式打开指定的图像文件

	if (!fp)
		return 0;

	//跳过位图文件头结构BITMAPFILEHEADER

	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中

	BITMAPINFOHEADER head;

	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp); //获取图像宽、高、每像素所占位数等信息

	bmpWidth = head.biWidth;

	bmpHeight = head.biHeight;

	biBitCount = head.biBitCount;//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）

	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//灰度图像有颜色表，且颜色表表项为256

	if (biBitCount == 8)
	{
		//申请颜色表所需要的空间，读颜色表进内存
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

	//申请位图数据所需要的空间，读位图数据进内存

	pBmpBuf = new unsigned char[lineByte * bmpHeight];
	
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
	
		
	// 将彩色图转化为灰度图
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

	fclose(fp);//关闭文件

	return 1;//读取文件成功
}


bool  BmpImage::saveBmpToFile(const char *fileName) {
	//如果位图数据指针为0，则没有数据传入，函数返回

	if (!pBmpBuf)
		return 0;

	//颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0

	int colorTablesize = 0;

	if (biBitCount == 8)
		colorTablesize = 1024;

	//待存储图像数据每行字节数为4的倍数

	//以二进制写的方式打开文件

	FILE *fp;
	fopen_s(&fp, fileName, "wb");

	if (fp == 0)
		return 0;

	//申请位图文件头结构变量，填写文件头信息

	BITMAPFILEHEADER fileHead;

	fileHead.bfType = 0x4D42;//bmp类型

							 //bfSize是图像文件4个组成部分之和

	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + linebyte*bmpHeight;

	fileHead.bfReserved1 = 0;

	fileHead.bfReserved2 = 0;

	//bfOffBits是图像文件前3个部分所需空间之和

	fileHead.bfOffBits = 54 + colorTablesize;

	//写文件头进文件

	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	//申请位图信息头结构变量，填写信息头信息

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

	//写位图信息头进内存

	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//如果灰度图像，有颜色表，写入文件 

	if (biBitCount == 8)
		fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

	//写位图数据进文件

	fwrite(this->pBmpBuf, bmpHeight*linebyte, 1, fp);

	//关闭文件

	fclose(fp);

	return 1;
}

float* BmpImage::getHOGForWindowStartFrom(int x, int y, int channel) {
	float *hog = new float[ARRAY_ALL];
	int hogofCellsize = CELL_W_NVM * CELL_H_NVM * BIN_NVM;
	float * hogOfCells = new float[CELL_W_NVM * CELL_H_NVM * BIN_NVM];
	// 获取每个cell的hog
	for (int cy = 0; cy < CELL_H_NVM; cy++) {
		for (int cx = 0; cx < CELL_W_NVM; cx++) {
			float*hogOfCell = getCellHog(x + cx * CELL_SIZE, y + cy * CELL_SIZE, CELL_SIZE);
			for (int i = 0; i < BIN_NVM; i++) {
				*(hogOfCells + cy * CELL_W_NVM * BIN_NVM + cx * BIN_NVM  +i) = hogOfCell[i];
			}
			delete[] hogOfCell;
		}
	}


	// 获取每个block 的hog, 先进行归一化
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

	for (int i = y; i < y + width; i++) { // 行数
		for (int j = x; j < x + width; j++) { // 列数
			*(fbin+(int)*(angBuf + i*linebyte + j)) += (float)*(magBuf + i*linebyte + j);
		}
	}
	return fbin;

}