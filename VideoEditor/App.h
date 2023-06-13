#pragma once
#include <wx/app.h>
#include <wx/wxprec.h>
#include <wx/display.h>
#ifndef WX_PRECOMP // if the precompiled files aren't available
	#include <wx/wx.h>
#endif

/// <summary>
/// The Application
/// </summary>
class App: public wxApp
{
public:
	App();
	bool OnInit();
	~App();
};

DECLARE_APP(App); //wxGetApp(); -> instance of the app class