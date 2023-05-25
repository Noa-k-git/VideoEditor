#include "ExportSeqDlg.h"

ExportSeqDlg::ExportSeqDlg(ServerClient* c, wxWindow* parent) : wxDialog(parent, wxID_ANY, "Create New Shared Project", wxDefaultPosition,
	wxDefaultSize)
{
	client = c;
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* title = new wxStaticText(this, wxID_ANY, "Export", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	wxFont titleFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD);
	title->SetFont(titleFont);
	mainSizer->Add(title, 0, wxEXPAND);
	mainSizer->SetMinSize(wxSize(300, 430));
	// Set the sizer for the dialog

	SetBackgroundColour(ORANGE_BACKGROUND);

	wxFont textFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);

	wxStaticText* helpLabel = new wxStaticText(this, wxID_ANY, "Choose a sequence to export");
	helpLabel->SetFont(textFont);
	mainSizer->Add(helpLabel, 0, wxEXPAND | wxTOP, ENTER_SPACE+SH_ENTER_SPACE);

	seqScrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	seqScrolledWindow->SetScrollRate(FromDIP(10), FromDIP(10));
	seqSizer = new wxBoxSizer(wxVERTICAL);
	seqScrolledWindow->SetSizer(seqSizer);
	
	for (auto seq : *Sequence::sequences.GetRecords())
	{
		AddSequence(seq->GetName());
	}

	mainSizer->Add(seqScrolledWindow, 1, wxEXPAND|wxTOP, ENTER_SPACE * 2);
	mainSizer->AddSpacer(ENTER_SPACE + SH_ENTER_SPACE);

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

void ExportSeqDlg::AddSequence(std::string name)
{
	wxButton* seqButton = new wxButton(seqScrolledWindow, wxID_ANY, name, wxDefaultPosition, wxDefaultSize,
		wxTE_PROCESS_ENTER | wxTAB_TRAVERSAL);
	seqButton->SetToolTip("Export " + name);
	seqButton->Bind(wxEVT_BUTTON, &ExportSeqDlg::OnExport, this);
	seqSizer->Add(seqButton, 0, wxEXPAND | wxTOP, SH_ENTER_SPACE);
	seqSizer->FitInside(seqScrolledWindow);
	Layout();
}

void ExportSeqDlg::OnExport(const wxCommandEvent& event_)
{
	wxButton* button = static_cast<wxButton*>(event_.GetEventObject());
	std::string name = button->GetLabel().ToStdString();
	auto res = Sequence::sequences.Contains(name);
	if (res.second) {
		(*res.first)->SaveVideo(client->GetPath() + name+ ".mp4");
		wxMessageBox("INFO", "Export Finished!", wxICON_INFORMATION);
	}
	else {
		wxMessageBox("ERROR", "Sequence is not available!", wxICON_ERROR);
	}
	Close();
}
