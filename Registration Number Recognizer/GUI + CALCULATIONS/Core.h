//#pragma once
//#include <vector>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/ml/ml.hpp>
//#include <opencv2/highgui/highgui.hpp>
//using namespace std;
//using namespace cv;
//ref class Core
//{
//public:
//	Core(void);
//	void set_image(Mat);
//	void set_svm_model(string);
//	void do_process();
//	vector<string> get_text_recognition();
//	vector<Mat> get_plate_detection();
//	vector<Mat> get_character_isolation();
//	vector<vector<Mat> > get_character();
//	vector<double> get_process_time();
//	void clear();
//private:
//	bool done;
//	bool ready;
//	SVM svm;
//	Mat image;
//	vector<Mat> plates;
//	vector<Mat> draw_character;
//	vector<vector<Mat> > characters;
//	vector<string> text_recognition;
//	vector<double> process_time;
//	void sort_character(vector<Mat>&);
//	char character_recognition(Mat);
//	bool plate_detection();
//	bool plate_recognition();
//};
//
