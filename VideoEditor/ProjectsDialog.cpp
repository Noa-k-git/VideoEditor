#include "ProjectsDialog.h"
#include "ServerProtocol.h"
#include "CreateProjectDlg.h"
#include "string_utils.h"
using namespace string_utils;

wxDEFINE_EVENT(LOAD_PROJECT_EVENT, wxCommandEvent);

ProjectDialog::ProjectDialog(ServerClient* c, wxWindow* parent) : wxDialog(parent, wxID_ANY, "My Projects")
{
	client = c;
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	
	wxScrolledWindow* window = new wxScrolledWindow(this, wxID_ANY);
	mainSizer->Add(window, 1, wxEXPAND);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	window->SetScrollbars(1, 1, 1, 1);
	window->SetMinSize(wxSize(400, 400));
	if (c->IsValidId()) {
		auto res = c->PullInfo();

		if (std::get<0>(res))
		{
			wxFont textFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);

			std::vector<std::string> allInfo = server_protocol::ParseMessage(std::get<1>(res)); // TODO: add ';' dummy data at the end of the list
			for (std::string proj : allInfo)
			{
				std::vector <std::string> projElm = server_protocol::ParseMessage(proj);
				if (projElm.size() == 3) {
					wxButton* b = new wxButton(window, wxID_ANY,"Name: " + projElm.at(1) + "\n\n" + "Users: " + JoinString(server_protocol::ParseMessage(projElm.at(2)), ', '),
						wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
					b->SetFont(textFont);
					b->SetName(projElm.at(0));
					b->Bind(wxEVT_BUTTON, &ProjectDialog::OnButtonClicked, this);
					sizer->Add(b, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);
				}
			}
			if (sizer->GetItemCount() == 0) {
				wxStaticText* text = new wxStaticText(window, wxID_ANY, "You don't have any projects yet.");
				wxFont font(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_LIGHT);
				text->SetFont(font);
				sizer->Add(text, 1, wxEXPAND | wxALL, 30);
			} else sizer->AddSpacer(10);
			wxButton* createSharedBtn = new wxButton(this, wxID_ANY, "Create Project");
			createSharedBtn->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event_) {
				Close();
				CreateProjectDlg(client, parent).ShowModal();
				});
			mainSizer->Add(createSharedBtn, 0, wxEXPAND|wxTOP, 10);
		}
	}
	else {
		wxStaticText* text = new wxStaticText(window, wxID_ANY, "You have to LOGIN first!");
		wxFont font(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
		text->SetFont(font);
		sizer->Add(text, 1, wxEXPAND|wxALL, 15);
		wxStaticText* comment = new wxStaticText(window, wxID_ANY, "You can login through the user button in the upper right corner");
		sizer->Add(comment, 1, wxEXPAND | wxALL, 15);
	}
	window->SetSizer(sizer);
	SetSizerAndFit(mainSizer);
}

void ProjectDialog::OnButtonClicked(wxCommandEvent& event_)
{
	wxButton* button = dynamic_cast<wxButton*>(event_.GetEventObject());
	if (button)
	{
		SetTitle("My Projects -> Pulling project from server");
		client->SetProjId(button->GetName().ToStdString());
		std::tuple<bool, std::string> info = client->PullProject();
		if (std::get<0>(info)) {
			wxWindow* mainWindow = wxGetApp().GetTopWindow();
			wxCommandEvent loadEvt_(LOAD_PROJECT_EVENT, GetId());
			loadEvt_.SetEventObject(this);

			std::vector<std::string> nameContent = server_protocol::ParseMessage(std::get<1>(info), 1);
			client->SetPrName(nameContent.at(0));
			
			std::ofstream file(client->GetPath() + client->GetPrName());
			file << JoinString(SplitString(nameContent.at(1), "\\n"), '\n');
			file.close();

			if (mainWindow != nullptr) {
				wxPostEvent(mainWindow, loadEvt_);
			}

		} else wxMessageBox("ERROR", "Error Occured in Pulling Project From Server", wxCANCEL | wxICON_ERROR);

	}
	else wxMessageBox("ERROR", "Error Occured in Project Id", wxCANCEL | wxICON_ERROR);
	Close();
}
