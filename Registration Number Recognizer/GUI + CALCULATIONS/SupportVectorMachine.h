#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>
#include "dirent.h"
#include <iostream>
#include "Extension.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace cv;
using namespace std;
using namespace cv::ml;
using namespace Extension;

const int number_of_feature = 32;

namespace SupportVectorMachine {
	
	//vector<string> getFilesPaths(string folder_path);
	//vector<string> getFoldersPaths(string path);
	vector<string> getContentPaths(string path);
	//static int countPixels(Mat img, bool black_pixel = true);
	static int countPixels(Mat img);
	static vector<float> calculateFeature(Mat src);
	bool training(string trainImgpath);
	char characterRecognition(Mat imgCharacter);
	bool recognize(Mat src, System::Windows::Forms::TextBox^  textBox, Matrix2Picture mat2bmp, System::Windows::Forms::PictureBox^  pictureBox1, System::Windows::Forms::PictureBox^  pictureBox2);
}