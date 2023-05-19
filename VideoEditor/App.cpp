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
	//client::CreateConnection();
	if (!wxApp::OnInit())
		return false;
	
	//wxLocale::AddCatalogLookupPathPrefix(".");
	//wxLocale local(wxLANGUAGE_ENGLISH, wxLOCALE_DONT_LOAD_DEFAULT);
	//wxLocale::Init(wxLANGUAGE_ENGLISH, wxLOCALE_DONT_LOAD_DEFAULT);
	wxImage::AddHandler(new wxPNGHandler);
	//VideoSource* vs = new VideoSource("C:\\Users\\cyber\\source\\repos\\Noa-k-git\\VideoEditor\\video.mp4");
	wxDisplay * display = new wxDisplay();
	
	MainWindow* main = new MainWindow(nullptr, window::id::MAINWINDOW, _("Main Window"), wxPoint(1,1), display->GetClientArea().GetSize());//, display->GetGeometry().GetPosition(), display->GetGeometry().GetSize());
	//main->SetSize(display->GetClientArea());

	return true;
}

App::~App()
{
	//for (auto& thread : *ISource<std::vector<AVFrame>>::readingThreads) {
	//	if (thread.joinable())
	//		thread.join();
	//}
}
