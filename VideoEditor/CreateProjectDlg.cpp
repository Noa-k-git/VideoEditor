#include "CreateProjectDlg.h"
wxDEFINE_EVENT(SAVE_PROJECT_EVENT, wxCommandEvent);

CreateProjectDlg::CreateProjectDlg(ServerClient* clientPtr, wxWindow* parent) : wxDialog(parent, wxID_ANY, "Create New Shared Project", wxDefaultPosition,
	wxDefaultSize)
{
	client = clientPtr;
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	
	wxStaticText* title = new wxStaticText(this, wxID_ANY, "Export:", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	wxFont titleFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD);
	title->SetFont(titleFont);
	mainSizer->Add(title, 0, wxEXPAND);
	mainSizer->SetMinSize(wxSize(300, 430));
	// Set the sizer for the dialog
	SetBackgroundColour(ORANGE_BACKGROUND);

	wxFont textFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);

	wxStaticText* helpLabel = new wxStaticText(this, wxID_ANY, "Choose a sequence to export");
	helpLabel->SetFont(textFont);
	mainSizer->Add(helpLabel, 0, wxEXPAND | wxTOP, ENTER_SPACE*2);
	
	emailsScrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	emailsScrolledWindow->SetScrollRate(FromDIP(10), FromDIP(10));
	emailsSizer = new wxBoxSizer(wxVERTICAL);
	emailsScrolledWindow->SetSizer(emailsSizer);
	mainSizer->Add(emailsScrolledWindow, 1, wxEXPAND);
	mainSizer->AddSpacer(ENTER_SPACE+SH_ENTER_SPACE);

	// create button
	createBtn = new wxButton(this, wxID_ANY, "Create Project");
	createBtn->SetMinSize(wxSize(createBtn->GetSize().GetWidth(), createBtn->GetSize().GetHeight() * 2));
	createBtn->Bind(wxEVT_BUTTON, &CreateProjectDlg::OnCreate, this);
	mainSizer->Add(createBtn, 1, wxEXPAND | wxTOP, ENTER_SPACE);

	AddMailInput();

	wxBoxSizer* borderSizer = new wxBoxSizer(wxVERTICAL);
	borderSizer->Add(mainSizer, 1, wxALL, 40);
	SetSizerAndFit(borderSizer);
	Layout();
	// Set window Position
	wxPoint parentCenter((int)((parent->GetPosition().x + parent->GetSize().GetWidth()) / 2),
		(int)((parent->GetPosition().y + parent->GetSize().GetHeight()) / 2));
	wxPoint thisCenter((int)(GetSize().GetWidth() / 2), (int)(GetSize().GetHeight() / 2));
	SetPosition(wxPoint(parentCenter.x - thisCenter.x, parentCenter.y - thisCenter.y));
	if (GetPosition().x < 0 || GetPosition().y < 0)
		SetPosition(wxDefaultPosition);

}

void CreateProjectDlg::AddMailInput()
{
	wxTextCtrl* mailInput = new wxTextCtrl(emailsScrolledWindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 
		wxTE_PROCESS_ENTER|wxTAB_TRAVERSAL);
	mailInput->SetHint("Enter memeber's email");
	mailInput->Bind(wxEVT_TEXT_ENTER, &CreateProjectDlg::OnEnter, this);
	emailsSizer->Add(mailInput, 0, wxEXPAND | wxTOP, SH_ENTER_SPACE);
	Layout();
}

void CreateProjectDlg::OnEnter(const wxCommandEvent& event_)
{
	AddMailInput();
	emailsSizer->FitInside(emailsScrolledWindow);
	Traveler(event_);
}

void CreateProjectDlg::OnCreate(const wxCommandEvent& event_)
{
	std::vector<std::string> reqElems;
	reqElems.push_back(nameInput->GetLineText(0).ToStdString());
	size_t itemCount = emailsSizer->GetItemCount();
	for (size_t i = 0; i < itemCount; i++) {
		wxSizerItem* item = emailsSizer->GetItem(i);
		wxWindow* window = item->GetWindow();
		wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(window);

		if (textCtrl) {
			std::string input = textCtrl->GetValue().ToStdString();
			if (input != "") reqElems.push_back(input);
		}
	}
	Close();
	wxWindow* mainWindow = wxGetApp().GetTopWindow();
	wxCommandEvent saveEvt_(SAVE_PROJECT_EVENT, GetId());
	saveEvt_.SetEventObject(this);
	if (mainWindow != nullptr) {
		wxPostEvent(mainWindow, saveEvt_);
	}
	wxGetActiveWindow()->CallAfter([=]() {
		std::tuple<bool, std::string> info = client->CreateProject(reqElems);
		if (std::get<0>(info)) {
			client->SetProjId(std::get<1>(info));
			client->PushProject();
		}
		else {
			wxMessageBox("ERROR", std::get<1>(info), wxICON_ERROR);
		}
		});
}

void CreateProjectDlg::Traveler(const wxCommandEvent& event_)
{
	wxWindow* currentFocus = FindFocus();

	if (currentFocus)
	{
		wxWindow* parent = currentFocus->GetParent();
		wxWindowList& siblings = parent->GetChildren();

		// Find the next sibling in the tab order
		bool foundCurrent = false;
		wxWindow* nextWidget = nullptr;
		for (wxWindow* sibling : siblings)
		{
			if (foundCurrent && sibling->IsShownOnScreen() && sibling->IsEnabled())
			{
				nextWidget = sibling;
				break;
			}
			if (sibling == currentFocus)
			{
				foundCurrent = true;
			}
		}

		// If no next sibling found, wrap around to the first sibling
		if (!nextWidget)
		{
			for (wxWindow* sibling : siblings)
			{
				if (sibling->IsShownOnScreen() && sibling->IsEnabled())
				{
					nextWidget = sibling;
					break;
				}
			}
		}

		// Set the focus to the next widget
		if (nextWidget)
		{
			nextWidget->SetFocus();
		}
	}
}

