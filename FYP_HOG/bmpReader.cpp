#include "bmpReader.h"

using namespace std;
unsigned char *pBmpBuf;//����ͼ�����ݵ�ָ��

int bmpWidth;//ͼ��Ŀ�
int bmpHeight;//ͼ��ĸ�
RGBQUAD *pColorTable;//��ɫ��ָ��

int biBitCount;//ͼ�����ͣ�ÿ����λ��



			   //-----------------------------------------------------------------------------------------
			   //����һ��ͼ��λͼ���ݡ����ߡ���ɫ��ָ�뼰ÿ������ռ��λ������Ϣ,����д��ָ���ļ���
bool readBmp(char *bmpName)
{
	FILE *fp;
	fopen_s(&fp, bmpName, "rb");//�����ƶ���ʽ��ָ����ͼ���ļ�

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

	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�

	pBmpBuf = new unsigned char[lineByte * bmpHeight];

	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);

	fclose(fp);//�ر��ļ�

	return 1;//��ȡ�ļ��ɹ�
}

bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable)
{

	//���λͼ����ָ��Ϊ0����û�����ݴ��룬��������

	if (!imgBuf)
		return 0;

	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0

	int colorTablesize = 0;

	if (biBitCount == 8)
		colorTablesize = 1024;

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���

	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	//�Զ�����д�ķ�ʽ���ļ�

	FILE *fp;
	fopen_s(&fp, bmpName, "wb");

	if (fp == 0)
		return 0;

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ

	BITMAPFILEHEADER fileHead;

	fileHead.bfType = 0x4D42;//bmp����

							 //bfSize��ͼ���ļ�4����ɲ���֮��

	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*height;

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

	head.biHeight = height;

	head.biPlanes = 1;

	head.biSize = 40;

	head.biSizeImage = lineByte*height;

	head.biWidth = width;

	head.biXPelsPerMeter = 0;

	head.biYPelsPerMeter = 0;

	//дλͼ��Ϣͷ���ڴ�

	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//����Ҷ�ͼ������ɫ��д���ļ� 

	if (biBitCount == 8)
		fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

	//дλͼ���ݽ��ļ�

	fwrite(imgBuf, height*lineByte, 1, fp);

	//�ر��ļ�

	fclose(fp);

	return 1;

}
void doIt()
{
	char readPath[] = "test.bmp";
	readBmp(readPath);
	// �������ͼ����Ϣ
	cout << "width=" << bmpWidth << "height=" << bmpHeight << "biBitCount=" << biBitCount << endl;
	// ͼ����ֽ���
	int linebyte1 = (bmpWidth*biBitCount / 8 + 3) / 4 * 4;
	int n = 0, m = 0, count_xiang_su = 0;

	// ofstream outfile("ͼ������.txt",ios::in|ios::trunc);���������ݴ���TXT�ļ���
	//��ʼ��ԭʼ���ص����顣

	if (biBitCount == 8)
	{
		for (int i = 0; i<bmpHeight / 2; i++)
		{
			for (int j = 0; j<bmpWidth / 2; i++)
				*(pBmpBuf + i*linebyte1 + j) = 0;
		}
	}

	if (biBitCount == 24)
	{
		for (int i = 0; i<bmpHeight; i++)
		{
			for (int j = 0; j<bmpWidth; j++)
			{
				for (int k = 0; k<3; k++)//ÿ����RGB���������ֱ���0�ű�ɺ�ɫ
				{
					m = *(pBmpBuf + i*linebyte1 + j * 3 + k);

					count_xiang_su++;

				}
				n++;
			}


		}
		cout << "�ܵ����ظ���Ϊ:" << n << endl;
		cout << "----------------------------------------------------" << endl;
	}

	// jiang tuxiang shuju cunpan .
	char writePath[] = "necpy.BMP";
	saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);

	//�����������pBmpBuf��pColorTable��ȫ�ֱ��������ļ�����ʱ����Ŀռ�

	delete[]pBmpBuf;
	if (biBitCount == 8)
		delete[]pColorTable;

}
