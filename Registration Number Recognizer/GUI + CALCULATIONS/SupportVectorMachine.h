#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>
#include "dirent.h"
#include "Extension.h"
#include <iostream>

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
	
	vector<string> list_file(string folder_path);

	vector<string> list_folder(string path);

	static int count_pixel(Mat img, bool black_pixel = true);

	static vector<float> calculate_feature(Mat src);

	bool Training(string savepath, string trainImgpath);
}