#include "MainWindow.h"
#include "vld.h"

using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]
void Main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	GUICALCULATIONS::MainWindow form;
	Application::Run(%form);
}
