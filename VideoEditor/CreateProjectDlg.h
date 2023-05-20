#pragma once
#include <wx/wx.h>
#include <wx/scrolwin.h>
#include "Client.h"
#include "string_utils.h"
#include "DesignConstatns.h"
using namespace string_utils;

class CreateProjectDlg : public wxDialog {
public:
	CreateProjectDlg(ServerClient* client, wxWindow* parent);
private:
	ServerClient* client;
	wxTextCtrl* nameInput;
	wxScrolledWindow* emailsScrolledWindow;
	wxBoxSizer* emailsSizer;
	wxButton* createBtn;
	void AddMailInput();
	void OnEnter(const wxCommandEvent& event_);
	void OnCreate(const wxCommandEvent& event_);
	void Traveler(const wxCommandEvent& event_);
};