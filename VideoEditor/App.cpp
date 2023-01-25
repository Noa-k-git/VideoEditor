#include "App.h"
#include "id.h"
#include "MainWindow.h"
#include "VideoSource.h"
#include "Client.h"

#include <ostream>
IMPLEMENT_APP(App) //Main function & instsnce of the App class

App::App()
{
}

bool App::OnInit()
{
	CreateConnection();
	if (!wxApp::OnInit())
		return false;

	VideoSource* vs = new VideoSource("C:\\Users\\cyber\\source\\repos\\Noa-k-git\\VideoEditor\\video.mp4");
	wxDisplay * display = new wxDisplay();
	
	MainWindow* main = new MainWindow(nullptr, window::id::MAINWINDOW, _("Main Window"));//, display->GetGeometry().GetPosition(), display->GetGeometry().GetSize());
	main->SetSize(display->GetClientArea());
	
	main->Show();
	return true;
}

App::~App()
{
}
