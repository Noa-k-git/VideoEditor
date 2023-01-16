#include "App.h"
#include "id.h"
#include "MainWindow.h"

#include "Client.h"

IMPLEMENT_APP(App) //Main

App::App()
{
}

bool App::OnInit()
{
	CreateConnection();
	if (!wxApp::OnInit())
		return false;

	MainWindow* main = new MainWindow(nullptr, window::id::MAINWINDOW, _("Main Window"));
	
	main->Show();
	return true;
}

App::~App()
{
}
