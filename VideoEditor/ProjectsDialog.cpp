#include "ProjectsDialog.h"
#include "ServerProtocol.h"
#include "string_utils.h"
using namespace string_utils;
ProjectDialog::ProjectDialog(ServerClient* c, wxWindow* parent) : wxDialog(parent, wxID_ANY, "My Projects")
{
	client = c;
	wxScrolledWindow* window = new wxScrolledWindow(this, wxID_ANY);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	window->SetScrollbars(1, 1, 1, 1);
	window->SetMinSize(wxSize(400, 400));
	if (c->IsValidId()) {
		auto res = c->PullInfo();

		if (std::get<0>(res))
		{
			std::vector<std::string> allInfo = server_protocol::ParseMessage(std::get<1>(res));
			for (std::string proj : allInfo)
			{
				std::vector <std::string> projElm = server_protocol::ParseMessage(proj);
				if (proj.size() == 3) {
					wxButton* b = new wxButton(window, wxID_ANY, proj.at(1) + "\t" + JoinString(server_protocol::ParseMessage(projElm.at(2)), ', '));
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
}

void ProjectDialog::OnButtonClicked(wxCommandEvent& event_)
{
	wxButton* button = dynamic_cast<wxButton*>(event_.GetEventObject());
	if (button)
	{
		SetTitle("My Projects -> Pulling project from server");
		std::tuple<bool, std::string> info = client->PullProject(button->GetName().ToStdString());
		if (std::get<0>(info)) {
			
		} else wxMessageBox("ERROR", "Error Occured in Pulling Project From Server", wxCANCEL | wxICON_ERROR);

	}
	else wxMessageBox("ERROR", "Error Occured in Project Id", wxCANCEL | wxICON_ERROR);
	Close();
}
