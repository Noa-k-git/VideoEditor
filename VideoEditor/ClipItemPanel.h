#pragma once
#include <wx/scrolwin.h>
#include "VideoClip.h"
#include "SmallBitmapButton.h"
class ClipItemPanel :
    public wxScrolledWindow
{
public:
	ClipItemPanel(VideoClip* videoClip, wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxHSCROLL | wxVSCROLL, const wxString& name = "scrolledWindow");
private:
	VideoClip* m_videoClip;

	void OnUpArrow(wxCommandEvent& event_);
	void OnDownArrow(wxCommandEvent& event_);
};

