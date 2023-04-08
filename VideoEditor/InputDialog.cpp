#include "InputDialog.h"

InputDialog::InputDialog(wxWindow* parent, const wxString& title, const wxString& text): wxDialog(parent, wxID_ANY, title)
{
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	wxStaticText* label = new wxStaticText(this, wxID_ANY, text);
	vbox->Add(label, 0, wxALIGN_CENTER | wxTOP, 20);

	input_ = new wxTextCtrl(this, wxID_ANY);
	vbox->Add(input_, 0, wxALIGN_CENTER | wxALL, 5);

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

	wxButton* okButton = new wxButton(this, wxID_OK, wxT("OK"));
	hbox->Add(okButton, 1, wxALL, 5);

	wxButton* cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
	hbox->Add(cancelButton, 1, wxALL, 5);

	vbox->Add(hbox, 0, wxALIGN_CENTER|wxBOTTOM, 20);

	SetSizer(vbox);
	Center();
}

wxString InputDialog::GetValue() const {
	return input_->GetValue();
}
