#pragma once
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/tglbtn.h>
#include <wx/artprov.h>
#include "DesignConstatns.h"
#include "SmallBitmapButton.h"
#include "Client.h"
/// <summary>
/// The dialog for signing up, login and logout from an account
/// </summary>
class UserDialog : public wxDialog {
public:
	UserDialog(ServerClient* c, wxWindow* parent);
	void ShowLogin();
	void ShowSignup();

private:
	ServerClient* client;
	wxStaticText* statusText;
	wxBoxSizer* sizer;
	SmallBitmapButton* logoutButton;
	wxStaticText* title;
	wxStaticText* usernameLabel;
	wxTextCtrl* usernameInput;
	wxStaticText* mailLabel;
	wxTextCtrl* mailInput;
	wxStaticText* passwordLabel;
	wxTextCtrl* passwordInput;
	SmallBitmapButton* eyeButton;
};