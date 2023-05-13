#include "UserDialog.h"

UserDialog::UserDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, "User Info", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	sizer = new wxBoxSizer(wxVERTICAL);
	title = new wxStaticText(this, wxID_ANY, "User Page", wxDefaultPosition, wxDefaultSize, wxCENTER);
	wxFont font(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD);
	title->SetFont(font);
	sizer->Add(title, 0, wxEXPAND|wxTOP, 40);	
	mainSizer->SetMinSize(wxSize(450, 700));
	// Set the sizer for the dialog
	SetBackgroundColour(ORANGE_BACKGROUND);
	wxPoint parentCenter((int)((parent->GetPosition().x + parent->GetSize().GetWidth()) / 2),
		(int)((parent->GetPosition().y + parent->GetSize().GetHeight()) / 2));
	wxPoint thisCenter((int)(GetSize().GetWidth() / 2), (int)(GetSize().GetHeight() / 2));
	SetPosition(wxPoint(parentCenter.x - thisCenter.x, parentCenter.y - thisCenter.y));
	if (GetPosition().x < 0)
		SetPosition(wxDefaultPosition);

	usernameLabel = new wxStaticText(this, wxID_ANY, "Username");
	usernameInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	passwordLabel = new wxStaticText(this, wxID_ANY, "Password");
	passwordInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	wxBitmap eyeIcon("passwordeye.png", wxBITMAP_TYPE_PNG);
	wxSize eyeSize(passwordInput->GetSize().GetHeight(), passwordInput->GetSize().GetHeight());
	eyeButton = new SmallBitmapButton(this, wxID_ANY, eyeIcon, wxDefaultPosition, eyeSize, wxSize(eyeSize.GetWidth() - 3, eyeSize.GetWidth() - 3), wxBU_AUTODRAW| wxBORDER_NONE);
	eyeButton->SetBackgroundColour(ORANGE_BACKGROUND);
	// Add the password field and toggle button to a sizer
	wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
	passwordSizer->Add(passwordInput, 1, wxRIGHT, 5);
	passwordSizer->Add(eyeButton, 0);
	Bind(wxEVT_BUTTON, [this, passwordSizer](wxCommandEvent& event_) {
		wxBitmap eyeIcon;
		wxTextCtrl* newPassInput;
		if (passwordInput->GetWindowStyleFlag() & wxTE_PASSWORD) {
			eyeIcon.LoadFile("passwordeye.png", wxBITMAP_TYPE_PNG);
			newPassInput = new wxTextCtrl(this, wxID_ANY, passwordInput->GetLineText(0), passwordInput->GetPosition(), wxDefaultSize);

		}
		else {
			eyeIcon.LoadFile("openeye.png", wxBITMAP_TYPE_PNG);
			newPassInput = new wxTextCtrl(this, wxID_ANY, passwordInput->GetLineText(0), passwordInput->GetPosition(), wxDefaultSize, wxTE_PASSWORD);
		}
		
		passwordSizer->Replace(passwordInput, newPassInput);
		delete passwordInput;
		passwordInput = newPassInput;
		eyeButton->SetBitmap(eyeIcon);
		eyeButton->RescaleBitmap();
		passwordInput->Refresh();
		passwordInput->Update();

		Layout();
		});
	sizer->Add(usernameLabel, 0, wxEXPAND | wxTOP, 50);
	sizer->Add(usernameInput, 0, wxEXPAND | wxTOP, SH_ENTER_SPACE);
	sizer->Add(passwordLabel, 0, wxEXPAND | wxTOP, ENTER_SPACE);
	sizer->Add(passwordSizer, 0, wxEXPAND | wxTOP, SH_ENTER_SPACE);
	wxSize s = passwordInput->GetSize();
	ShowLogin();

	mainSizer->Add(sizer, 0, wxALIGN_CENTER_HORIZONTAL);
	SetSizerAndFit(mainSizer);
}

void UserDialog::ShowLogin()
{
	// Add a label to the sizer
	title->SetLabel("LOGIN");



	// Add a button to the sizer
	//sizer->Add(button, wxSizerFlags().Center().Border(wxALL, 10));

}

void UserDialog::ShowSignup()
{
}

