#include "Extension.h"

using namespace Extension;

String2Character::String2Character() {

}

Matrix2Picture::Matrix2Picture() {

}

System::Drawing::Bitmap^ Matrix2Picture::Matrix2Bitmap(cv::Mat& colorImage) {
	//System::Drawing::Graphics^ graphics = control->CreateGraphics();
	System::IntPtr ptr(colorImage.ptr());
	System::Drawing::Bitmap^ bitmap;
	switch (colorImage.type()) {
		case CV_8UC3: { //non-grayscale images are correctly displayed here
			bitmap = gcnew System::Drawing::Bitmap(colorImage.cols, colorImage.rows, colorImage.step,
				System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
			break;
		}
		case CV_8UC1: {	//grayscale images are incorrectly displayed here 
			bitmap = gcnew System::Drawing::Bitmap(colorImage.cols, colorImage.rows, colorImage.step,
				System::Drawing::Imaging::PixelFormat::Format8bppIndexed, ptr);
			break;
		}
		default: {
			//error message
			break;
		}
	}
	return bitmap;
}