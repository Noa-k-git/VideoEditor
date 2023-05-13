#pragma once
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/tglbtn.h>
#include <wx/artprov.h>
#include "DesignConstatns.h"
#include "SmallBitmapButton.h"
class UserDialog : public wxDialog {
public:
	UserDialog(wxWindow* parent);
	void ShowLogin();
	void ShowSignup();

private:
	wxBoxSizer* sizer;
	wxStaticText* title;
	wxStaticText* usernameLabel;
	wxTextCtrl* usernameInput;
	wxStaticText* passwordLabel;
	wxTextCtrl* passwordInput;
	SmallBitmapButton* eyeButton;
};