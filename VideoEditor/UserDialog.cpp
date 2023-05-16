#include "UserDialog.h"

UserDialog::UserDialog(ServerClient* c, wxWindow* parent) : wxDialog(parent, wxID_ANY, "User Info", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{	
	const wxString logoutString("LOGOUT");
	const wxString loginString("LOGIN");
	const wxString signupString("SIGNUP");
	this->client = c;

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	
	sizer = new wxBoxSizer(wxVERTICAL);
	title = new wxStaticText(this, wxID_ANY, loginString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	wxFont titleFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD);
	title->SetFont(titleFont);
	sizer->Add(title, 1, wxEXPAND|wxTOP, 40);	
	mainSizer->SetMinSize(wxSize(450, 650));
	// Set the sizer for the dialog
	SetBackgroundColour(ORANGE_BACKGROUND);
	
	wxFont textFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD);

	// username
	usernameLabel = new wxStaticText(this, wxID_ANY, "Full Name");
	usernameLabel->SetFont(textFont);
	usernameInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString);

	// mail
	mailLabel = new wxStaticText(this, wxID_ANY, "Email");
	mailLabel->SetFont(textFont);
	mailInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);

	// password
	passwordLabel = new wxStaticText(this, wxID_ANY, "Password");
	passwordLabel->SetFont(textFont);
	passwordInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	wxBitmap eyeIcon("passwordeye.png", wxBITMAP_TYPE_PNG);
	wxSize eyeSize(passwordInput->GetSize().GetHeight() - 5, passwordInput->GetSize().GetHeight() - 5);
	eyeButton = new SmallBitmapButton(this, wxID_ANY, eyeIcon, wxDefaultPosition, eyeSize, eyeSize, wxBU_AUTODRAW| wxBORDER_NONE);
	eyeButton->SetBackgroundColour(ORANGE_BACKGROUND);

	// Add the password field and toggle button to a sizer
	wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
	passwordSizer->Add(passwordInput, 1, wxRIGHT, 5);
	passwordSizer->Add(eyeButton, 0, wxALIGN_CENTER_VERTICAL);
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

	// send info to server button
	wxButton* sendButton = new wxButton(this, wxID_ANY, loginString, wxDefaultPosition, wxDefaultSize);
	sendButton->SetMinSize(wxSize(sendButton->GetSize().GetWidth(), sendButton->GetSize().GetHeight() + 10));
	sendButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event_) {
		statusText->SetLabel("Sending request to server..");
		std::tuple<bool, std::string> loged;
		if (sendButton->GetLabel() == loginString) {
			loged = client->Login(mailInput->GetLineText(0).ToStdString(), passwordInput->GetLineText(0).ToStdString());
		}
		else {
			loged = client->Signup(usernameInput->GetLineText(0).ToStdString(), mailInput->GetLineText(0).ToStdString(), passwordInput->GetLineText(0).ToStdString());
		}
		if (std::get<0>(loged)) {
			Close();
		}
		else {
			statusText->SetLabel(_(std::get<1>(loged)));
		}
		});
	
	// screen switcher button

	wxButton* switchButton = new wxButton(this, wxID_ANY, signupString);
	switchButton->SetMinSize(wxSize(switchButton->GetSize().GetWidth(), switchButton->GetSize().GetHeight() + 10));
	switchButton->SetBackgroundColour(WINDOW_BRIGHT_BACKGOUND_COLOUR);
	switchButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event_) {
		if (switchButton->GetLabel() == signupString) {
			title->SetLabel(signupString);

			usernameLabel->Show();
			usernameInput->Show();
			
			sizer->Insert(2, usernameInput, 0, wxEXPAND | wxTOP, ENTER_SPACE);
			sizer->Insert(2, usernameLabel, 0, wxEXPAND | wxTOP, SH_ENTER_SPACE);
			
			
			sendButton->SetLabel(signupString);
			switchButton->SetLabel(loginString);
		}
		else {
			title->SetLabel(loginString);
			usernameLabel->Hide();
			usernameInput->Hide();
			sizer->Detach(usernameLabel);
			sizer->Detach(usernameInput);
			sendButton->SetLabel(loginString);
			switchButton->SetLabel(signupString);
		}
		statusText->SetLabel(wxEmptyString);
		sizer->Layout();
		Layout();
		});

	// Adding to sizer
	sizer->AddSpacer(SH_ENTER_SPACE * 2);
	sizer->Add(mailLabel, 0, wxEXPAND | wxTOP, ENTER_SPACE);
	sizer->Add(mailInput, 0, wxEXPAND | wxTOP, SH_ENTER_SPACE);
	sizer->Add(passwordLabel, 0, wxEXPAND | wxTOP, ENTER_SPACE);
	sizer->Add(passwordSizer, 0, wxEXPAND | wxTOP, SH_ENTER_SPACE);
	sizer->AddSpacer(ENTER_SPACE);
	sizer->Add(sendButton, 0, wxEXPAND | wxTOP, ENTER_SPACE);
	sizer->AddSpacer(ENTER_SPACE * 2);
	//sizer->Add(switchButton, 0, wxEXPAND | wxTOP, SH_ENTER_SPACE);
	usernameInput->Hide();
	usernameLabel->Hide();
	
	mainSizer->Add(sizer, 0, wxALIGN_CENTER_HORIZONTAL);
	
	// status text
	statusText = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	wxFont statusFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_LIGHT);
	statusText->SetFont(statusFont);

	statusText->SetForegroundColour(wxColour(100, 100, 100));
	if (client->IsValidId())
	{
		wxString status((std::string)"You are already LOGED :)");
		statusText->SetLabel(status);
	}
	statusText->SetSize(wxSize(mainSizer->GetSize().GetWidth(), -1));

	mainSizer->Add(statusText, 1, wxEXPAND|wxRIGHT|wxLEFT, ENTER_SPACE*3);

	wxBitmap logouticon("logout_icon.png", wxBITMAP_TYPE_PNG);
	logoutButton = new SmallBitmapButton(this, wxID_ANY, logouticon, wxDefaultPosition, wxSize(30, 30), wxSize(30, 30));
	logoutButton->SetToolTip(logoutString);
	logoutButton->SetBackgroundColour(WINDOW_BRIGHT_BACKGOUND_COLOUR);
	logoutButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event_) {client->Logout(); });

	mainSizer->Add(logoutButton, 0, wxEXPAND|wxTOP| wxBU_AUTODRAW , 60);
	SetSizerAndFit(mainSizer);
	Layout();
	// Set window Position
	wxPoint parentCenter((int)((parent->GetPosition().x + parent->GetSize().GetWidth()) / 2),
		(int)((parent->GetPosition().y + parent->GetSize().GetHeight()) / 2));
	wxPoint thisCenter((int)(GetSize().GetWidth() / 2), (int)(GetSize().GetHeight() / 2));
	SetPosition(wxPoint(parentCenter.x - thisCenter.x, parentCenter.y - thisCenter.y));
	if (GetPosition().x < 0 || GetPosition().y < 0)
		SetPosition(wxDefaultPosition);

	
}

void UserDialog::ShowLogin()
{
	// Add a label to the sizer


	// Add a button to the sizer
	//sizer->Add(button, wxSizerFlags().Center().Border(wxALL, 10));

}

void UserDialog::ShowSignup()
{
}

