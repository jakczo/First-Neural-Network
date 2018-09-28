#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>
#include "dirent.h"
#include "Extension.h"
#include <iostream>
#include "SupportVectorMachine.h"

namespace GUICALCULATIONS {

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
	using namespace SupportVectorMachine;

	Mat src; //need to create a Mat type for the src image.
	String2Character stringTochar;
	Matrix2Picture mat2bmp;

	/// <summary>
	/// Summary for MainWindow
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::PictureBox^  ptbSource;
	private: System::Windows::Forms::Button^  btnProcess;
	private: System::Windows::Forms::Button^  btnLearn;
	public: System::Windows::Forms::TextBox^  textBox;
	public: System::Windows::Forms::PictureBox^  pictureBox1;
	public: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::Label^  label1;

	protected:
	protected:
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainWindow::typeid));
			this->ptbSource = (gcnew System::Windows::Forms::PictureBox());
			this->btnProcess = (gcnew System::Windows::Forms::Button());
			this->btnLearn = (gcnew System::Windows::Forms::Button());
			this->textBox = (gcnew System::Windows::Forms::TextBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ptbSource))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// ptbSource
			// 
			this->ptbSource->Location = System::Drawing::Point(12, 89);
			this->ptbSource->Name = L"ptbSource";
			this->ptbSource->Size = System::Drawing::Size(606, 319);
			this->ptbSource->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->ptbSource->TabIndex = 2;
			this->ptbSource->TabStop = false;
			// 
			// btnProcess
			// 
			this->btnProcess->Location = System::Drawing::Point(73, 15);
			this->btnProcess->Name = L"btnProcess";
			this->btnProcess->Size = System::Drawing::Size(55, 22);
			this->btnProcess->TabIndex = 5;
			this->btnProcess->Text = L"Process";
			this->btnProcess->UseVisualStyleBackColor = true;
			this->btnProcess->Click += gcnew System::EventHandler(this, &MainWindow::btnProcess_Click);
			// 
			// btnLearn
			// 
			this->btnLearn->Location = System::Drawing::Point(12, 15);
			this->btnLearn->Name = L"btnLearn";
			this->btnLearn->Size = System::Drawing::Size(55, 21);
			this->btnLearn->TabIndex = 6;
			this->btnLearn->Text = L"Learn";
			this->btnLearn->UseVisualStyleBackColor = true;
			this->btnLearn->Click += gcnew System::EventHandler(this, &MainWindow::btnLearn_Click);
			// 
			// textBox
			// 
			this->textBox->Location = System::Drawing::Point(12, 52);
			this->textBox->Name = L"textBox";
			this->textBox->Size = System::Drawing::Size(116, 20);
			this->textBox->TabIndex = 7;
			this->textBox->TextChanged += gcnew System::EventHandler(this, &MainWindow::textBox_TextChanged);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(226, 8);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(193, 75);
			this->pictureBox1->TabIndex = 9;
			this->pictureBox1->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(425, 8);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(193, 75);
			this->pictureBox2->TabIndex = 10;
			this->pictureBox2->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(153, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(0, 13);
			this->label1->TabIndex = 11;
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(626, 420);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->textBox);
			this->Controls->Add(this->btnLearn);
			this->Controls->Add(this->btnProcess);
			this->Controls->Add(this->ptbSource);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainWindow";
			this->Text = L"MainWindow";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ptbSource))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		private: char* ConvertString2Char(System::String^ str) { //function converting  string to array of characters
			char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
			return str2;
		}

		private: System::Void btnProcess_Click(System::Object^  sender, System::EventArgs^  e) { //Process button action
																						 
			//Open File Dialog (select a path)
			OpenFileDialog^ dgOpen = gcnew OpenFileDialog();
			dgOpen->Filter = "Image(*.bmp; *.jpg)|*.bmp;*.jpg|All files (*.*)|*.*||";
			if (dgOpen->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
				return;
			}

			//Showing image in picture box
			Bitmap^ bmpSrc1 = gcnew Bitmap(dgOpen->FileName); //converting to bitmap
			ptbSource->Image = bmpSrc1;
			ptbSource->Refresh();
			src = imread(ConvertString2Char(dgOpen->FileName));
			int dbSize_H = src.rows;
			int dbSize_W = src.cols;
			System::String^ h = gcnew System::String(to_string(dbSize_H).c_str());
			System::String^ w = gcnew System::String(to_string(dbSize_W).c_str());
			label1->Text = w + " x " + h;
			textBox->Text = "";

			if (recognize(src, textBox, mat2bmp, pictureBox1, pictureBox2)) {
				//Mat plate;
				//Mat markedCharacters; //mat to img
				//imshow("Plate", src); //new window
				//imshow("Marked characters", src); //new window
			} else {
				MessageBox::Show("Some error occured during recognition");
			}
		}

		private: System::Void btnLearn_Click(System::Object^  sender, System::EventArgs^  e) {
			//choose from where program should get data
			FolderBrowserDialog^ folder = gcnew FolderBrowserDialog();
			if (folder->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
				return;
			}
			System::String^ folderPath;
			folderPath = folder->SelectedPath;
			char buffer[100] = { 0 };
			if (folderPath->Length < sizeof(buffer)) {
				sprintf(buffer, "%s", folderPath);
			}
			std::string folderPathString(buffer); //path of the data content

			////choose where to save SVM file
			//SaveFileDialog^ saveFile = gcnew SaveFileDialog();
			//if (saveFile->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
			//	return;
			//}
			//System::String^ saveFileName = saveFile->FileName; //FileName with path
			//char buffer2[100] = { 0 };
			//if (saveFileName->Length < sizeof(buffer2)) {
			//	sprintf(buffer2, "%s", saveFileName);
			//}
			//std::string saveSVM(buffer2); //path (including name and extension) of the generated data (SVM)
		

			if (training(folderPathString)) {
				MessageBox::Show("\t   Training completed.\nSVM file has been saved in project directory.");
			} else {
				MessageBox::Show("Some error occured during training");
			}
		}

		private: System::Void textBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		}
	};
}