#include "MainForm.h"
#include "OpticalDisc.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew IAPDBurnCdDvd::MainForm());
	OpticalDisc opt;
	long k = opt.getDeviceCount();
	return 0;
}