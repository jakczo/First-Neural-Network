#include "SupportVectorMachine.h"

vector<string> SupportVectorMachine::list_file(string folder_path) {
	vector<string> files;
	DIR *dir = opendir(folder_path.c_str());
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
		{
			string file_path = folder_path + "/" + string(entry->d_name);
			files.push_back(file_path);
		}
	}
	closedir(dir);
	return files;
}

vector<string> SupportVectorMachine::list_folder(string path) {
	vector<string> folders;
	DIR *dir = opendir(path.c_str());
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
		{
			string folder_path = path + "/" + string(entry->d_name);
			folders.push_back(folder_path);
		}
	}
	closedir(dir);
	return folders;
}

static int SupportVectorMachine::count_pixel(Mat img, bool black_pixel)
{
	int black = 0;
	int white = 0;
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
		{
			if (img.at<uchar>(i, j) == 0)
				black++;
			else
				white++;
		}
	if (black_pixel)
		return black;
	else
		return white;
}

static vector<float> SupportVectorMachine::calculate_feature(Mat src)
{
	Mat img;
	if (src.channels() == 3)
	{
		cvtColor(src, img, CV_BGR2GRAY);
		threshold(img, img, 100, 255, CV_THRESH_BINARY);
	}
	else
	{
		threshold(src, img, 100, 255, CV_THRESH_BINARY);
	}

	vector<float> r;
	//vector<int> cell_pixel;
	resize(img, img, cv::Size(40, 40));
	int h = img.rows / 4;
	int w = img.cols / 4;
	int S = count_pixel(img);
	int T = img.cols * img.rows;
	for (int i = 0; i < img.rows; i += h)
	{
		for (int j = 0; j < img.cols; j += w)
		{
			Mat cell = img(Rect(i, j, h, w));
			int s = count_pixel(cell);
			float f = (float)s / S;
			r.push_back(f);
		}
	}

	for (int i = 0; i < 16; i += 4)
	{
		float f = r[i] + r[i + 1] + r[i + 2] + r[i + 3];
		r.push_back(f);
	}

	for (int i = 0; i < 4; ++i)
	{
		float f = r[i] + r[i + 4] + r[i + 8] + r[i + 12];
		r.push_back(f);
	}

	r.push_back(r[0] + r[5] + r[10] + r[15]);
	r.push_back(r[3] + r[6] + r[9] + r[12]);
	r.push_back(r[0] + r[1] + r[4] + r[5]);
	r.push_back(r[2] + r[3] + r[6] + r[7]);
	r.push_back(r[8] + r[9] + r[12] + r[13]);
	r.push_back(r[10] + r[11] + r[14] + r[15]);
	r.push_back(r[5] + r[6] + r[9] + r[10]);
	r.push_back(r[0] + r[1] + r[2] + r[3] + r[4] + r[7] + r[8] + r[11] + r[12] + r[13] + r[14] + r[15]);

	return r; //32 feature
}

bool SupportVectorMachine::Training(string savepath, string trainImgpath) {
	const int number_of_class = 30;
	const int number_of_sample = 10;
	const int number_of_feature = 32;

	//Train SVM OpenCV 3.1
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::RBF);
	svm->setGamma(0.5);
	svm->setC(16);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

	vector<string> folders = list_folder(trainImgpath);
	if (folders.size() <= 0)
	{
		//do something
		return false;
	}
	if (number_of_class != folders.size() || number_of_sample <= 0 || number_of_class <= 0)
	{
		//do something
		return false;
	}
	Mat src;
	Mat data = Mat(number_of_sample * number_of_class, number_of_feature, CV_32FC1);
	Mat label = Mat(number_of_sample * number_of_class, 1, CV_32SC1);
	int index = 0;
	for (size_t i = 0; i < folders.size(); ++i)
	{
		vector<string> files = list_file(folders.at(i));
		if (files.size() <= 0 || files.size() != number_of_sample)
		{
			return false;
		}
		string folder_path = folders.at(i);
		string label_folder = folder_path.substr(folder_path.length() - 1);
		for (size_t j = 0; j < files.size(); ++j)
		{
			src = imread(files.at(j));
			if (src.empty())
			{
				return false;
			}
			vector<float> feature = calculate_feature(src);
			for (size_t t = 0; t < feature.size(); ++t)
				data.at<float>(index, t) = feature.at(t);
			label.at<int>(index, 0) = i;
			index++;
		}
	}
	// SVM Train OpenCV 3.1
	svm->trainAuto(ml::TrainData::create(data, ml::ROW_SAMPLE, label));
	svm->save(savepath);
	return true;
}