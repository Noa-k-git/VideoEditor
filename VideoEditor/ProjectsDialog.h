#pragma once
#include "Client.h"
#include <wx/wx.h>
#include <wx/scrolwin.h>

class ProjectDialog : public wxDialog
{
public:
	ProjectDialog(ServerClient* c, wxWindow* parent);
	void OnButtonClicked(wxCommandEvent& event_);
private:
	ServerClient* client;
};