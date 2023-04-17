#pragma once
#include <wx/app.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP // if the precompiled files aren't available
	#include <wx/wx.h>
#endif

class App: public wxApp
{
public:
	App();
	bool OnInit();
	~App();
};

DECLARE_APP(App); //wxGetApp(); -> instance of the app class