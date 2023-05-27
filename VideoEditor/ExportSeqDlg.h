#pragma once
#include <wx/wx.h>
#include <wx/dialog.h>
#include "string_utils.h"
#include "DesignConstatns.h"
#include "Sequence.h"
#include "Client.h"

using namespace string_utils;

class ExportSeqDlg : public wxDialog {
public:
	ExportSeqDlg(ServerClient* c, wxWindow* parent);
private:
	ServerClient* client;
	wxScrolledWindow* seqScrolledWindow;
	wxBoxSizer* seqSizer;
	void AddSequence(std::string);
	void OnExport(const wxCommandEvent& event_);
};