#pragma once
#include <wx/wx.h>

class InputDialog : public wxDialog
{
public:
	InputDialog(wxWindow* parent, const wxString& title, const wxString&);
	wxString GetValue() const;
private:
	wxTextCtrl* input_;
};

