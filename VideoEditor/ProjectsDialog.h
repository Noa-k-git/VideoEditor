#pragma once
#include "Client.h"
#include "App.h"
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/scrolwin.h>

wxDECLARE_EVENT(SAVE_PROJECT_EVENT, wxCommandEvent);
wxDECLARE_EVENT(LOAD_PROJECT_EVENT, wxCommandEvent);


class ProjectDialog : public wxDialog
{
public:
	ProjectDialog(ServerClient* c, wxWindow* parent);
	void OnButtonClicked(wxCommandEvent& event_);
private:
	ServerClient* client;
};