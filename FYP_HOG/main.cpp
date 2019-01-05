#include  "BmpImage.h"
using namespace std;

void clearScreen();

void prepareHogDatForImagePath(string imageName, bool hasLabel,  bool isTrain);

void prepareDat(bool isTrain, bool hasLabel);


int hogCounter = 0;
ofstream svmDatFile;
COORD w10 = { 0, 1 };
COORD w00 = { 0, 0 };
COORD w20 = { 0, 2 };

int main() {

	for (int i = 0; i < 10; i++) {
		cout << random(105) << " ";
	}

	srand((int)time(0));
	prepareDat(true, true);
	prepareDat(true, false);
	prepareDat(false, true);
	prepareDat(false, false);

	system("pause");
	return 0;
}


void prepareDat(bool isTrain, bool hasLabel) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), w00);
	cout << "prepare data for" << (isTrain ? " train " : " test ") << "dataset, " <<( hasLabel ? "pos " : "neg " )<< "group" ;

	string inPath;
	//目标文件夹路径
	svmDatFile.open(isTrain ? "train.dat" : "test.dat", hasLabel ? ios::trunc : ios::app);

	 inPath = string(isTrain? TRAIN_PATH: TEST_PATH).append(hasLabel? "pos\\*.bmp": "neg\\*.bmp");//遍历文件夹下的所有.bmp文件

	 vector <string> *res = new vector<string>;
	findFile(inPath, res);
	int count = 0;
	hogCounter = 0;

	for (vector<string>::iterator it = res->begin(); it != res->end(); it++) {
		string imageName = *it;
		count++;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), w10);
		cout << "image number: " << count << " image name: " << imageName << endl;
		prepareHogDatForImagePath(imageName, hasLabel, isTrain);
	}
	clearScreen();

	svmDatFile.close();
}

void prepareHogDatForImagePath(string imageName, bool hasLabel, bool isTrain) {
	
	string imagePath = string(isTrain? TRAIN_PATH: TEST_PATH).append(hasLabel? "pos\\": "neg\\").append(imageName);
	BmpImage *image = new BmpImage(imagePath.c_str());

	if (hasLabel) {
		//读取Label 文件
		ifstream infile;
		string labelPath = string(isTrain ? TRAIN_PATH : TEST_PATH).append("labels\\").append(imageName);
		stringReplace(labelPath, ".bmp", ".txt");
		infile.open(labelPath);

		while (infile.good() && !infile.eof()) {
			string line;
			getline(infile, line);

			vector<string> v;
			SplitString(line, v, " ");
			if (line.length()) {
				Window *gtWindow = new Window(stof(v[1]), stof(v[2]), stof(v[3]), stof(v[4]), image->bmpWidth, image->bmpHeight);
				image->gtWindows.push_back(gtWindow);
			}
		}
		// 根据label 创建hog 并比对
		for (vector<Window *>::iterator iter = image->gtWindows.begin(); iter != image->gtWindows.end(); iter++) {
			Window *window = *iter;
			//cout << "x, y, w, h is: " << window->x << " " << window->y << " " << window->width << " " << window->height << " " << endl;
			int lx = window->x - NORM_WIDTH / 2 - 5 + random(10);
			lx = lx <= 0 ? 0 : (lx > image->bmpWidth - 1 - NORM_WIDTH ? image->bmpWidth - 1 - NORM_WIDTH : lx);
			int ly = window->y - NORM_WIDTH / 2 - 5 + random(10);
			ly = ly <= 0 ? 0 : (ly > image->bmpHeight - 1 - NORM_HEIGHT ? image->bmpHeight - 1 - NORM_HEIGHT : ly);
			if (lx < 0 || ly < 0) {
				continue;
			}

			float * hog = image->getHOGForWindowStartFrom(lx, ly);

			svmDatFile << "+1" << " ";
			for (int i = 0; i < ARRAY_ALL; i++) {
				svmDatFile << i+1 << ":" << hog[i] << " ";
			}
			svmDatFile << endl;

			hogCounter++;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), w20);
			cout << "the number of hog is " << hogCounter << endl;
			delete[] hog;
		}
		infile.close();
	} else {
		//对neg image 进行随机取hog
		if (image->bmpWidth < NORM_WIDTH + 20 || image->bmpHeight < NORM_HEIGHT + 20) return;

		int lx = random((int)(image->bmpWidth - 21 - NORM_WIDTH)) + 10;
		int ly = random((int)(image->bmpHeight - 21 - NORM_HEIGHT)) + 10;
		cout << image->bmpHeight - 21 - NORM_HEIGHT << endl;
		float * hog = image->getHOGForWindowStartFrom(lx, ly);
		
		hogCounter++;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), w20);
		cout << "the number of hog is " << hogCounter << endl;

		svmDatFile << "-1" << " ";
		for (int i = 0; i < ARRAY_ALL; i++) {
			svmDatFile << i+1 << ":" << hog[i] << " ";
		}
		svmDatFile << endl;
	}

	delete[] image->magBuf;
	delete[] image->angBuf;
}

void clearScreen() {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), w00);
	cout << "                                                                                                                                                                                                                                      ";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), w10);
	cout << "                                                                                                                                                                                                                                      ";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), w20);
	cout << "                                                                                                                                                                                                                                      ";
}