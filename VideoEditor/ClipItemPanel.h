#pragma once
#include <wx/scrolwin.h>
#include "VideoClip.h"
#include "SmallBitmapButton.h"

wxDECLARE_EVENT(SWAP_CLIP_WITH_PREV_EVT, wxCommandEvent);
wxDECLARE_EVENT(SWAP_CLIP_WITH_NEXT_EVT, wxCommandEvent);

class ClipItemPanel :
    public wxPanel
{
public:
	ClipItemPanel(VideoClip* videoClip, int idx, wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "scrolledWindow");
private:
	VideoClip* m_videoClip;
	int m_idx;
	void OnUpArrow(wxCommandEvent& event_);
	void OnDownArrow(wxCommandEvent& event_);
};

