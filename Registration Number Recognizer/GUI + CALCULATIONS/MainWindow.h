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
	Mat src; //need to create a Mat typpe for the src image.

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
	private: System::Windows::Forms::Button^  btnBrowse;
	private: System::Windows::Forms::Button^  btnProcess;
	private: System::Windows::Forms::PictureBox^  ptbSource;
	private: System::Windows::Forms::Label^  lbSource;

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
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->btnProcess = (gcnew System::Windows::Forms::Button());
			this->ptbSource = (gcnew System::Windows::Forms::PictureBox());
			this->lbSource = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ptbSource))->BeginInit();
			this->SuspendLayout();
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(12, 12);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(90, 35);
			this->btnBrowse->TabIndex = 0;
			this->btnBrowse->Text = L"Browse";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &MainWindow::btnBrowse_Click);
			// 
			// btnProcess
			// 
			this->btnProcess->Location = System::Drawing::Point(108, 12);
			this->btnProcess->Name = L"btnProcess";
			this->btnProcess->Size = System::Drawing::Size(93, 35);
			this->btnProcess->TabIndex = 1;
			this->btnProcess->Text = L"Process";
			this->btnProcess->UseVisualStyleBackColor = true;
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
			// lbSource
			// 
			this->lbSource->AutoSize = true;
			this->lbSource->Location = System::Drawing::Point(356, 23);
			this->lbSource->Name = L"lbSource";
			this->lbSource->Size = System::Drawing::Size(73, 13);
			this->lbSource->TabIndex = 3;
			this->lbSource->Text = L"Source Image";
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(577, 370);
			this->Controls->Add(this->lbSource);
			this->Controls->Add(this->ptbSource);
			this->Controls->Add(this->btnProcess);
			this->Controls->Add(this->btnBrowse);
			this->Name = L"MainWindow";
			this->Text = L"MainWindow";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ptbSource))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) {

		OpenFileDialog^ dgOpen = gcnew OpenFileDialog();
		dgOpen->Filter = "Image(*.bmp; *.jpg)|*.bmp;*.jpg|All files (*.*)|*.*||";
		if (dgOpen->ShowDialog() == System::Windows::Forms::DialogResult::Cancel)
		{
			return;
		}
		Bitmap^ bmpSrc = gcnew Bitmap(dgOpen->FileName);
		ptbSource->Image = bmpSrc;
		ptbSource->Refresh();
		src = imread(ConvertString2Char(dgOpen->FileName));
		imshow("Source image showing via OpenCV", src);

	}

	private: char* ConvertString2Char(System::String^ str) {
		char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
		return str2;
	}


};
}
