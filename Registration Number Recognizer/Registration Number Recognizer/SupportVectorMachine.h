#pragma once
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/ml.hpp>
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

//const int classes = 30; //marks count
const int classes = 36; //marks count

const int samples = 10; //samples count //bylo 10
const int numberOfFeature = 64; //features count bylo 32

namespace SupportVectorMachine {
	vector<string> getContentPaths(string path);
	static int countPixels(Mat img);
	static vector<float> calculateFeature(Mat src);
	int countSamples(string folderPath);
	vector<vector<cv::Point>> findContours(Mat binaryImage);
	Mat convertToBinary(Mat normalImage);
	bool training(string trainImgpath);
	char characterRecognition(Mat imgCharacter);
	bool recognize(Mat src, System::Windows::Forms::TextBox^  textBox, Matrix2Picture mat2bmp, System::Windows::Forms::PictureBox^  pictureBox1, System::Windows::Forms::PictureBox^  pictureBox2);
}