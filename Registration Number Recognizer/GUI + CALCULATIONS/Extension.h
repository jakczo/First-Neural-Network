#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <sstream>
#include <string>
#include <iostream>

using namespace System::Runtime::InteropServices;

namespace Extension {
	class Matrix2Picture {
	public:
		Matrix2Picture();
		//Bitmap -> mat i pic
		System::Drawing::Bitmap^ Matrix2Bitmap(cv::Mat& colorImage);
	};

	class String2Character {
	public:
		String2Character();
		//char* ConvertString2Character(String str)
		char* ConvertString2Character(System::String^ str) { //Marshall method
			char* strs = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
			return strs;
		}
	};

	class Number2String {
	public:
		std::string intiger2String(int number) {
			std::stringstream ss;
			ss << number;
			return ss.str();
		}
	};

	//class Character2String {

	//};

	//class Number2Row {

	//};
}