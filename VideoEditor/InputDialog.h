#pragma once
#include <wx/wx.h>
/// <summary>
/// The dialog for receiving new input from the user
/// </summary>
class InputDialog : public wxDialog
{
public:
	InputDialog(wxWindow* parent, const wxString& title, const wxString&);
	wxString GetValue() const;
private:
	wxTextCtrl* input_;
	void OnCharHooK(wxKeyEvent& event);
};

