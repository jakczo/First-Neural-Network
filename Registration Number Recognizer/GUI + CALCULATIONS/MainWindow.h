#pragma once

//dodane
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>


namespace GUICALCULATIONS {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	//dodane
	using namespace System::Runtime::InteropServices;
	using namespace std;
	using namespace cv;
	Mat src; //need to create a Mat type for the src image.

	/// <summary>
	/// Summary for MainWindow
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
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
	private: System::Windows::Forms::TextBox^  textBox1;





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
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ptbSource))->BeginInit();
			this->SuspendLayout();
			// 
			// ptbSource
			// 
			this->ptbSource->Location = System::Drawing::Point(12, 53);
			this->ptbSource->Name = L"ptbSource";
			this->ptbSource->Size = System::Drawing::Size(553, 305);
			this->ptbSource->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->ptbSource->TabIndex = 2;
			this->ptbSource->TabStop = false;
			// 
			// btnProcess
			// 
			this->btnProcess->Location = System::Drawing::Point(12, 12);
			this->btnProcess->Name = L"btnProcess";
			this->btnProcess->Size = System::Drawing::Size(90, 35);
			this->btnProcess->TabIndex = 5;
			this->btnProcess->Text = L"Process";
			this->btnProcess->UseVisualStyleBackColor = true;
			this->btnProcess->Click += gcnew System::EventHandler(this, &MainWindow::btnProcess_Click);
			// 
			// btnLearn
			// 
			this->btnLearn->Location = System::Drawing::Point(108, 12);
			this->btnLearn->Name = L"btnLearn";
			this->btnLearn->Size = System::Drawing::Size(90, 35);
			this->btnLearn->TabIndex = 6;
			this->btnLearn->Text = L"Learn";
			this->btnLearn->UseVisualStyleBackColor = true;
			this->btnLearn->Click += gcnew System::EventHandler(this, &MainWindow::btnLearn_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(465, 20);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 20);
			this->textBox1->TabIndex = 7;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &MainWindow::textBox1_TextChanged);
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(577, 370);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->btnLearn);
			this->Controls->Add(this->btnProcess);
			this->Controls->Add(this->ptbSource);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainWindow";
			this->Text = L"MainWindow";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ptbSource))->EndInit();
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
	if (dgOpen->ShowDialog() == System::Windows::Forms::DialogResult::Cancel)
	{
		return;
	}

	//Showing image in picture box
	Bitmap^ bmpSrc = gcnew Bitmap(dgOpen->FileName); //converting to bitmap
	ptbSource->Image = bmpSrc;
	ptbSource->Refresh();
	src = imread(ConvertString2Char(dgOpen->FileName));
	imshow("Source image showing via OpenCV", src); //new window
}
private: System::Void btnLearn_Click(System::Object^  sender, System::EventArgs^  e) {

	//choose from where program should get data
	FolderBrowserDialog^ folder = gcnew FolderBrowserDialog();
	if (folder->ShowDialog() == System::Windows::Forms::DialogResult::Cancel)
	{
		return;
	}

	System::String^ folderPath;
	folderPath = folder->SelectedPath;
	char buffer1[100] = { 0 };
	if (folderPath->Length < sizeof(buffer1))
		sprintf(buffer1, "%s", folderPath);
	std::string folderPathString(buffer1); //path of the data content

	
	//choose where to save SVM file
	SaveFileDialog^ saveFile = gcnew SaveFileDialog();
	if (saveFile->ShowDialog() == System::Windows::Forms::DialogResult::Cancel)
	{
		return;
	}

	System::String^ saveFileName = saveFile->FileName; //FileName with path
	char buffer2[100] = { 0 };
	if (saveFileName->Length < sizeof(buffer2))
		sprintf(buffer2, "%s", saveFileName);
	std::string saveSVM(buffer2); //path (including name and extension) of the generated data (SVM)
}

private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
};
}
