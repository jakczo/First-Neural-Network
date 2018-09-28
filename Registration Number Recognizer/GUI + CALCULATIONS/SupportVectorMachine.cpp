#include "SupportVectorMachine.h"

//vector<string> SupportVectorMachine::getFilesPaths(string currentFolderPath) {
//	vector<string> files;
//	DIR *dir = opendir(currentFolderPath.c_str());
//	struct dirent *entry;
//	while ((entry = readdir(dir)) != NULL) {
//		if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
//			string file_path = currentFolderPath + "/" + string(entry->d_name);
//			files.push_back(file_path);
//		}
//	}
//	closedir(dir);
//	return files;
//}

//vector<string> SupportVectorMachine::getFoldersPaths(string path) {
//	vector<string> folders;
//	DIR *dir = opendir(path.c_str());
//	struct dirent *entry;
//	while ((entry = readdir(dir)) != NULL) {
//		if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
//			string currentFolderPath = path + "/" + string(entry->d_name);
//			folders.push_back(currentFolderPath);
//		}
//	}
//	closedir(dir);
//	return folders;
//}

vector<string> SupportVectorMachine::getContentPaths(string mainPath) {
	vector<string> content;
	DIR *dir = opendir(mainPath.c_str());
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
			string path = mainPath + "/" + string(entry->d_name);
			content.push_back(path);
		}
	}
	closedir(dir);
	return content;
}

//static int SupportVectorMachine::countPixels(Mat img, bool blackPixel) {
static int SupportVectorMachine::countPixels(Mat img) {
	int black = 0;
	//int white = 0;
	for (int i = 0; i < img.rows; ++i) {
		for (int j = 0; j < img.cols; ++j) {
			//if (img.at<uchar>(i, j) == 0) {
			//	++black;
			//} else {
			//	++white;
			//}
			if (img.at<uchar>(i, j) == 0) {
				++black;
			} 
		}
	}
	//if (blackPixel) {
	//	return black;
	//} else {
	//	return white;
	//}
	return black;
}

static vector<float> SupportVectorMachine::calculateFeature(Mat sampleImage) {
	Mat img;
	if (sampleImage.channels() == 3) { //if RGB
		cvtColor(sampleImage, img, CV_BGR2GRAY); //change to gray
		threshold(img, img, 100, 255, CV_THRESH_BINARY);
	} else {
		threshold(sampleImage, img, 100, 255, CV_THRESH_BINARY);
	}

	vector<float> vectorOfFeatures;
	//vector<int> cell_pixel;
	resize(img, img, cv::Size(40, 40));
	int cellHeight = img.rows / 4;
	int cellWidth = img.cols / 4;
	int sumOfBlackPixelsOfImage = countPixels(img);

	for (int i = 0; i < img.rows; i += cellHeight) {
		for (int j = 0; j < img.cols; j += cellWidth) {
			Mat cell = img(Rect(i, j, cellHeight, cellWidth));
			int sumOfBlackPixelsOfCell = countPixels(cell);
			float singleFeature = (float)sumOfBlackPixelsOfCell / sumOfBlackPixelsOfImage;
			vectorOfFeatures.push_back(singleFeature);
		}
	}

	for (int i = 0; i < 16; i += 4) {
		float singleFeature = vectorOfFeatures[i] + vectorOfFeatures[i + 1] + vectorOfFeatures[i + 2] + vectorOfFeatures[i + 3];
		vectorOfFeatures.push_back(singleFeature);
	}

	for (int i = 0; i < 4; ++i) {
		float singleFeature = vectorOfFeatures[i] + vectorOfFeatures[i + 4] + vectorOfFeatures[i + 8] + vectorOfFeatures[i + 12];
		vectorOfFeatures.push_back(singleFeature);
	}

	vectorOfFeatures.push_back(vectorOfFeatures[0] + vectorOfFeatures[5] + vectorOfFeatures[10] + vectorOfFeatures[15]);
	vectorOfFeatures.push_back(vectorOfFeatures[3] + vectorOfFeatures[6] + vectorOfFeatures[9] + vectorOfFeatures[12]);
	vectorOfFeatures.push_back(vectorOfFeatures[0] + vectorOfFeatures[1] + vectorOfFeatures[4] + vectorOfFeatures[5]);
	vectorOfFeatures.push_back(vectorOfFeatures[2] + vectorOfFeatures[3] + vectorOfFeatures[6] + vectorOfFeatures[7]);
	vectorOfFeatures.push_back(vectorOfFeatures[8] + vectorOfFeatures[9] + vectorOfFeatures[12] + vectorOfFeatures[13]);
	vectorOfFeatures.push_back(vectorOfFeatures[10] + vectorOfFeatures[11] + vectorOfFeatures[14] + vectorOfFeatures[15]);
	vectorOfFeatures.push_back(vectorOfFeatures[5] + vectorOfFeatures[6] + vectorOfFeatures[9] + vectorOfFeatures[10]);

	vectorOfFeatures.push_back(
		vectorOfFeatures[0] + vectorOfFeatures[1] + vectorOfFeatures[2] + vectorOfFeatures[3] + 
		vectorOfFeatures[4] + vectorOfFeatures[7] + vectorOfFeatures[8] + vectorOfFeatures[11] +
		vectorOfFeatures[12] + vectorOfFeatures[13] + vectorOfFeatures[14] + vectorOfFeatures[15]
	);

	return vectorOfFeatures; //32 features
}

bool SupportVectorMachine::training(string folderPath) {
	std::string savePath = "SVM/svm.txt";
	const int classes = 30; //marks count
	const int samples = 10; //samples count
	const int features = 32; //features count
	Mat sampleImage;
	Mat data = Mat(samples * classes, features, CV_32FC1); //matrix of samples
	Mat responses = Mat(samples * classes, 1, CV_32SC1); //matrix of responses
	int index = 0;

	//Train SVM OpenCV 3.1
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC); //0-infinity
	svm->setKernel(SVM::RBF); //radial base function
	svm->setGamma(0.5);
	svm->setC(16);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

	vector<string> folders = getContentPaths(folderPath);
	if (folders.size() <= 0) {
		MessageBox::Show("Chosen data folder is empty!");
		return false;
	}
	if (classes != folders.size() || samples <= 0 || classes <= 0) {
		MessageBox::Show("There are not enough classes or samples in data folder!");
		return false;
	}

	for (size_t i = 0; i < folders.size(); ++i) {
		vector<string> files = getContentPaths(folders.at(i));
		if (files.size() <= 0 || files.size() != samples) {
			MessageBox::Show("There is no samples in one of the given folders!");
			return false;
		}
		for (size_t j = 0; j < files.size(); ++j) {
			sampleImage = imread(files.at(j));
			if (sampleImage.empty()) {
				MessageBox::Show("One of the samples is wrong!");
				return false;
			}
			vector<float> feature = calculateFeature(sampleImage);
			for (size_t t = 0; t < feature.size(); ++t) {
				data.at<float>(index, t) = feature.at(t);
			}
			responses.at<int>(index, 0) = i;
			++index;
		}
	}
	// SVM Train OpenCV 3.1
	svm->trainAuto(ml::TrainData::create(data, ml::ROW_SAMPLE, responses));
	svm->save(savePath);
	return true;
}

char SupportVectorMachine::characterRecognition(Mat imgCharacter) {
	//Load SVM training file OpenCV 3.1
	//Ptr<SVM> svmNew = SVM::create();
	//svmNew = SVM::load<SVM>("svm.txt"); //nie dziala
	Ptr<SVM> svmNew = Algorithm::load<SVM>("SVM/svm.txt");

	//SVM svmNew;
	//svmNew.load("D:/svm.txt");

	char c = '*';

	vector<float> feature = calculateFeature(imgCharacter);
	// Open CV3.1
	Mat m = Mat(1, number_of_feature, CV_32FC1);
	for (size_t i = 0; i < feature.size(); ++i)
	{
		float temp = feature[i];
		m.at<float>(0, i) = temp;
	}
	//Mat m = Mat(number_of_feature,1, CV_32FC1);		// Open CV 2.4
	//for (size_t i = 0; i < feature.size(); ++i)
	//{
	//float temp = feature[i];
	//m.at<float>(i,0) = temp;
	//}
	int ri = int(svmNew->predict(m)); // Open CV 3.1
									  /*int ri = int(svmNew.predict(m));*/
	if (ri >= 0 && ri <= 9)
		c = (char)(ri + 48); //ma ascii 0 = 48
	if (ri >= 10 && ri < 18)
		c = (char)(ri + 55); //ma accii A = 5, --> tu A-H
	if (ri >= 18 && ri < 22)
		c = (char)(ri + 55 + 2); //K-N, bo I,J
	if (ri == 22) c = 'P';
	if (ri == 23) c = 'S';
	if (ri >= 24 && ri < 27)
		c = (char)(ri + 60); //T-V,  
	if (ri >= 27 && ri < 30)
		c = (char)(ri + 61); //X-Z

	return c;

}

bool SupportVectorMachine::recognize(Mat srcImg, System::Windows::Forms::TextBox^  textBox, Matrix2Picture mat2bmp, System::Windows::Forms::PictureBox^  pictureBox1, System::Windows::Forms::PictureBox^  pictureBox2) {
	vector<Mat> plates;
	vector<Mat> draw_character;
	vector<vector<Mat>> characters;
	vector<string> text_recognition;
	vector<double> process_time;
	void sort_character(vector<Mat>&);
	Mat image = srcImg;
	Mat gray, binary;
	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	double t = (double)cvGetTickCount();
	cvtColor(image, gray, CV_BGR2GRAY);
	//imshow("gray", gray);
	adaptiveThreshold(gray, binary, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 55, 5);
	//imshow("binary",binary);
	Mat or_binary = binary.clone();
	Mat element = getStructuringElement(MORPH_RECT, cv::Size(3, 3));
	erode(binary, binary, element);
	dilate(binary, binary, element);
	findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	if (contours.size() <= 0) return false;
	for (size_t i = 0; i < contours.size(); ++i)
	{
		Rect r = boundingRect(contours.at(i));
		if (r.width > image.cols / 2 || r.height > image.cols / 2 || r.width < 120 || r.height < 20
			|| (double)r.width / r.height > 4.5 || (double)r.width / r.height < 3.5)
			continue;
		Mat sub_binary = or_binary(r);
		Mat _plate = sub_binary.clone();
		vector<vector<cv::Point> > sub_contours;
		vector<Vec4i> sub_hierarchy;
		findContours(sub_binary, sub_contours, sub_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		if (sub_contours.size() < 8) continue;
		int count = 0;
		vector<Mat> c;
		Mat sub_image = image(r);
		vector<Rect> r_characters;
		for (size_t j = 0; j < sub_contours.size(); ++j)
		{
			Rect sub_r = boundingRect(sub_contours.at(j));
			if (sub_r.height > r.height / 2 && sub_r.width < r.width / 8 && sub_r.width > 5 && r.width > 15 && sub_r.x > 5)
			{
				Mat cj = _plate(sub_r);
				double ratio = (double)countPixels(cj) / (cj.cols*cj.rows);
				if (ratio > 0.2 && ratio < 0.7)
				{
					r_characters.push_back(sub_r);
					rectangle(sub_image, sub_r, Scalar(0, 0, 255), 2, 8, 0);
				}
			}
		}
		if (r_characters.size() >= 7)
		{
			// sap xep 
			for (int i = 0; i < r_characters.size() - 1; ++i)
			{
				for (int j = i + 1; j < r_characters.size(); ++j)
				{
					Rect temp;
					if (r_characters.at(j).x < r_characters.at(i).x)
					{
						temp = r_characters.at(j);
						r_characters.at(j) = r_characters.at(i);
						r_characters.at(i) = temp;
					}
				}
			}
			for (int i = 0; i < r_characters.size(); ++i)
			{
				Mat cj = _plate(r_characters.at(i));
				c.push_back(cj);
			}
			characters.push_back(c);
			sub_binary = or_binary(r);
			plates.push_back(_plate);
			draw_character.push_back(sub_image);
		}
		rectangle(image, r, Scalar(0, 255, 0), 2, 8, 0);
	}

	//imshow("plate", image);
	//imshow("char", draw_character[0]);
	Mat plateImg, chaImg;
	convertScaleAbs(plates[0], plateImg);
	convertScaleAbs(draw_character[0], chaImg);
	resize(plateImg, plateImg, cv::Size(200, 50));
	resize(chaImg, chaImg, cv::Size(200, 50));
	pictureBox1->Image = mat2bmp.Matrix2Bitmap(plateImg);
	pictureBox2->Image = mat2bmp.Matrix2Bitmap(chaImg);

	//imshow("Plate", mat2bmp.Matrix2Bitmap(plateImg)); //new window
	//imshow("Marked characters", mat2bmp.Matrix2Bitmap(chaImg)); //new window

	pictureBox1->Refresh();
	pictureBox2->Refresh();

	// Plate recoginatinon
	for (size_t i = 0; i < characters.size(); i++)
	{
		string result;
		for (size_t j = 0; j < characters.at(i).size(); ++j)
		{

			char cs = characterRecognition(characters.at(i).at(j));
			result.push_back(cs);

		}
		text_recognition.push_back(result);
		System::String^ str = gcnew System::String(result.c_str()); // Convert std string to System String
		textBox->Text += str;
	}
	return true;
}