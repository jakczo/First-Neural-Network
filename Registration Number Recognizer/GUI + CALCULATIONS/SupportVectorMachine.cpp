#include "SupportVectorMachine.h"

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

static int SupportVectorMachine::countPixels(Mat img) {
	int black = 0;
	for (int i = 0; i < img.rows; ++i) {
		for (int j = 0; j < img.cols; ++j) {
			if (img.at<uchar>(i, j) == 0) {
				++black;
			} 
		}
	}
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
	resize(img, img, cv::Size(40, 40));
	int cellHeight = img.rows / 8;
	int cellWidth = img.cols / 8;
	int sumOfBlackPixelsOfImage = countPixels(img);

	for (int i = 0; i < img.rows; i += cellHeight) {
		for (int j = 0; j < img.cols; j += cellWidth) {
			Mat cell = img(Rect(i, j, cellHeight, cellWidth));
			int sumOfBlackPixelsOfCell = countPixels(cell);
			float singleFeature = (float)sumOfBlackPixelsOfCell / sumOfBlackPixelsOfImage;
			vectorOfFeatures.push_back(singleFeature);
		}
	}

	//for (int i = 0; i < img.rows; i += cellHeight) {
	//	for (int j = 0; j < img.cols; j += cellWidth) {
	//		Mat cell = img(Rect(i, j, cellHeight, cellWidth));
	//		int sumOfBlackPixelsOfCell = countPixels(cell);
	//		float singleFeature = (float)sumOfBlackPixelsOfCell / sumOfBlackPixelsOfImage;
	//		vectorOfFeatures.push_back(singleFeature);
	//	}
	//}

	//for (int i = 0; i < 16; i += 4) {
	//	float singleFeature = vectorOfFeatures[i] + vectorOfFeatures[i + 1] + vectorOfFeatures[i + 2] + vectorOfFeatures[i + 3];
	//	vectorOfFeatures.push_back(singleFeature);
	//}

	//for (int i = 0; i < 4; ++i) {
	//	float singleFeature = vectorOfFeatures[i] + vectorOfFeatures[i + 4] + vectorOfFeatures[i + 8] + vectorOfFeatures[i + 12];
	//	vectorOfFeatures.push_back(singleFeature);
	//}

	//vectorOfFeatures.push_back(vectorOfFeatures[0] + vectorOfFeatures[5] + vectorOfFeatures[10] + vectorOfFeatures[15]);
	//vectorOfFeatures.push_back(vectorOfFeatures[3] + vectorOfFeatures[6] + vectorOfFeatures[9] + vectorOfFeatures[12]);
	//vectorOfFeatures.push_back(vectorOfFeatures[0] + vectorOfFeatures[1] + vectorOfFeatures[4] + vectorOfFeatures[5]);
	//vectorOfFeatures.push_back(vectorOfFeatures[2] + vectorOfFeatures[3] + vectorOfFeatures[6] + vectorOfFeatures[7]);
	//vectorOfFeatures.push_back(vectorOfFeatures[8] + vectorOfFeatures[9] + vectorOfFeatures[12] + vectorOfFeatures[13]);
	//vectorOfFeatures.push_back(vectorOfFeatures[10] + vectorOfFeatures[11] + vectorOfFeatures[14] + vectorOfFeatures[15]);
	//vectorOfFeatures.push_back(vectorOfFeatures[5] + vectorOfFeatures[6] + vectorOfFeatures[9] + vectorOfFeatures[10]);

	//vectorOfFeatures.push_back(
	//	vectorOfFeatures[0] + vectorOfFeatures[1] + vectorOfFeatures[2] + vectorOfFeatures[3] + 
	//	vectorOfFeatures[4] + vectorOfFeatures[7] + vectorOfFeatures[8] + vectorOfFeatures[11] +
	//	vectorOfFeatures[12] + vectorOfFeatures[13] + vectorOfFeatures[14] + vectorOfFeatures[15]
	//);
	return vectorOfFeatures; //32 features
}

int SupportVectorMachine::countSamples(string folderPath) { //function that counts features in whole data file
	int counter = 0;
	Mat sampleImage;
	vector<string> folders = getContentPaths(folderPath);
	if (folders.size() <= 0) {
		MessageBox::Show("Chosen data folder is empty!");
		return -1;
	}
	if (classes != folders.size() || samples <= 0 || classes <= 0) {
		MessageBox::Show("There are not enough classes or samples in data folder!");
		return -1;
	}

	for (size_t i = 0; i < folders.size(); ++i) { //folder with classes loop
		vector<string> files = getContentPaths(folders.at(i));
		if (files.size() <= 0) {
			MessageBox::Show("There is no samples in one of the given folders!");
			return -1;
		}
		for (size_t j = 0; j < files.size(); ++j) { //folder with samples loop
			sampleImage = imread(files.at(j));
			if (sampleImage.empty()) {
				MessageBox::Show("One of the samples is wrong!");
				return -1;
			}
			++counter;
		}
	}
	return counter;
}

bool SupportVectorMachine::training(string folderPath) {
	std::string savePath = "SVM/svm.txt";
	Mat sampleImage;
	//int numberOfSamples = countSamples(folderPath);
	//if (numberOfSamples <= 0) {
	//	MessageBox::Show("Data file is corrupted");
	//	return false;
	//}
	vector<string> folders = getContentPaths(folderPath);
	Mat data = Mat(360, numberOfFeature, CV_32FC1); //matrix of samples' features;
	Mat responses = Mat(360, 1, CV_32SC1); //matrix of responses
	int index = 0;

	//Train SVM OpenCV 3.1
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC); //0-infinity
	svm->setKernel(SVM::RBF); //radial base function
	svm->setGamma(0.5);
	svm->setC(16);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

	for (size_t i = 0; i < folders.size(); ++i) { //folder with classes loop
		vector<string> files = getContentPaths(folders.at(i));
		for (size_t j = 0; j < 10; ++j) { //files.size(); ++j) { //folder with samples loop
			sampleImage = imread(files.at(j));
			vector<float> feature = calculateFeature(sampleImage);
			for (size_t t = 0; t < feature.size(); ++t) {
				data.at<float>(index, t) = feature.at(t); //i = 5, j = 9
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
	Ptr<SVM> svmNew = Algorithm::load<SVM>("SVM/svm.txt");
	char character = '*';
	vector<float> feature = calculateFeature(imgCharacter);
	Mat mat = Mat(1, numberOfFeature, CV_32FC1);
	for (size_t i = 0; i < feature.size(); ++i) {
		float temp = feature[i];
		mat.at<float>(0, i) = temp;
	}

	int id= int(svmNew->predict(mat));

	if (id >= 0 && id <= 9)
		character = (char)(48 + id); //0-9
	if (id >= 10 && id <= 35)
		character = (char)(55 + id); //A-Z (ri: (10 -> 26) + 55 = Acsii: (65 -> 90))
	return character;
}

vector<vector<cv::Point>> SupportVectorMachine::findContours(Mat binaryImage) {
	//In this part program modyfies picture to find interesting shapes - contours
	//Morphological Transformations - erode and dilate - to eliminate the noises
	Mat binaryClone = binaryImage.clone();
	vector<vector<cv::Point>> contours;
	Mat element = getStructuringElement(MORPH_RECT, cv::Size(3, 3));

	erode(binaryClone, binaryClone, element); //make objects in black bigger
	imshow("1.Erode", binaryClone);
	dilate(binaryClone, binaryClone, element); //making objects in white bigger
	imshow("2.Dilate", binaryClone);
	findContours(binaryClone, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));	//Finding contours on binary picture
	return contours;
}

Mat SupportVectorMachine::convertToBinary(Mat normalImage) {
	Mat gray, binary;
	cvtColor(normalImage, gray, CV_BGR2GRAY); //converting to shades of grey
	adaptiveThreshold(gray, binary, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 55, 5); //converting to binary
	//threshold(gray, binary, 40, 255, CV_THRESH_BINARY);
	return binary;
}

bool SupportVectorMachine::recognize(Mat mainImage, System::Windows::Forms::TextBox^ textBox, Matrix2Picture mat2bmp, System::Windows::Forms::PictureBox^  pictureBox1, System::Windows::Forms::PictureBox^  pictureBox2) {
	vector<Mat> plates;
	vector<Mat> drawCharacter;
	vector<vector<Mat>> characters;
	vector<string> textRecognition;
	Mat image = mainImage.clone();
	Mat binary;
	bool flagPlateFound = false;
	bool flagCharacterFound = false;
	vector<vector<cv::Point>> contours;
	
	if (image.size().height > cv::Size(1280, 720).height && image.size().width > cv::Size(1280, 720).width) {
		resize(image, image, cv::Size(604, 806));
	}
	
	binary = convertToBinary(image);
	contours = findContours(binary);
	if (contours.size() <= 0) { //if there are no contours...
		return false; //...exit function
	}

	for (size_t i = 0; i < contours.size(); ++i) { //going through the rectangle contours
		Rect actualContour = boundingRect(contours.at(i)); //pass i-th contour
		rectangle(image, actualContour, Scalar(0, 0, 255), 2, 8, 0); //color every passing contour
		imshow("before", image);
		if (actualContour.width > image.cols / 2
			|| actualContour.height > image.cols / 2
			|| actualContour.width < 120
			|| actualContour.height < 40
			|| (double)actualContour.width / actualContour.height > 4.5
			|| (double)actualContour.width / actualContour.height < 3.5) {
			continue;
		}
		imshow("after", image);

		//we found our plate
		//Cut binary in r-place and save it to subBinary
		Mat subBinary = binary(actualContour); //fragment of the photo in the place of the contour
		
		
		Mat plateMatrix = subBinary.clone();

		vector<vector<cv::Point>> subContours = findContours(subBinary);
		if (subContours.size() < 8) {
			continue;
		}

		int count = 0;
		vector<Mat> c;
		Mat subImage = image(actualContour);
		imshow("9.subimage", subImage);
		vector<Rect> rCharacters;
		for (size_t j = 0; j < subContours.size(); ++j) {
			Rect sub_r = boundingRect(subContours.at(j));

			if (sub_r.height > actualContour.height / 2 && sub_r.width < actualContour.width / 8 && sub_r.width > 5 && actualContour.width > 15 && sub_r.x > 5) {
				Mat additionalMatrix = plateMatrix(sub_r);
				double ratio = (double)countPixels(additionalMatrix) / (additionalMatrix.cols*additionalMatrix.rows);

				if (ratio > 0.2 && ratio < 0.7) {
					rCharacters.push_back(sub_r);
					rectangle(subImage, sub_r, Scalar(255, 0, 0), 2, 8, 0);
				}
			}
		}
		if (rCharacters.size() >= 7) {
			for (int i = 0; i < rCharacters.size() - 1; ++i) {
				for (int j = i + 1; j < rCharacters.size(); ++j) {
					Rect temp;
					if (rCharacters.at(j).x < rCharacters.at(i).x) {
						temp = rCharacters.at(j);
						rCharacters.at(j) = rCharacters.at(i);
						rCharacters.at(i) = temp;
					}
				}
			}
			for (int i = 0; i < rCharacters.size(); ++i) {
				Mat additionalMatrix = plateMatrix(rCharacters.at(i));
				c.push_back(additionalMatrix);
			}
			characters.push_back(c);
			subBinary = binary(actualContour);
			plates.push_back(plateMatrix);
			drawCharacter.push_back(subImage);
			flagCharacterFound = true;
			flagPlateFound = true;
		}
		rectangle(image, actualContour, Scalar(0, 255, 0), 2, 8, 0);
	}

	if (flagPlateFound && flagCharacterFound) {
		Mat plateImg, chaImg;
		cv::convertScaleAbs(plates[0], plateImg);
		cv::convertScaleAbs(drawCharacter[0], chaImg);
		cv::resize(plateImg, plateImg, cv::Size(200, 50));
		cv::resize(chaImg, chaImg, cv::Size(200, 50));

		pictureBox1->Image = mat2bmp.Matrix2Bitmap(plateImg);
		pictureBox2->Image = mat2bmp.Matrix2Bitmap(chaImg);
		pictureBox1->Refresh();
		pictureBox2->Refresh();

		// Plate recognition
		for (size_t i = 0; i < characters.size(); i++) {
			string result;
			for (size_t j = 0; j < characters.at(i).size(); ++j) {

				char cs = characterRecognition(characters.at(i).at(j));
				result.push_back(cs);
			}
			textRecognition.push_back(result);
			System::String^ str = gcnew System::String(result.c_str()); // Convert std string to System String
			textBox->Text += str;
		}
		return true;
	}
	return false;
}