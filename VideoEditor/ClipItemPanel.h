#pragma once
#include <wx/scrolwin.h>
#include "VideoClip.h"
#include "SmallBitmapButton.h"

wxDECLARE_EVENT(SWAP_CLIP_WITH_PREV_EVT, wxCommandEvent);
wxDECLARE_EVENT(SWAP_CLIP_WITH_NEXT_EVT, wxCommandEvent);
/// <summary>
/// A class for displaying a clip item in the sequence conterol window
/// </summary>
class ClipItemPanel :
    public wxPanel
{
public:
	ClipItemPanel(VideoClip* videoClip, int idx, wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "scrolledWindow");
private:
	VideoClip* m_videoClip;
	int m_idx; // the index of the clip in the sequence
	/// <summary>
	/// Sends up arrow event to the sequence control window
	/// </summary>
	/// <param name="event_">event</param>
	void OnUpArrow(wxCommandEvent& event_);
	/// <summary>
	/// Sends down arrow event to the sequence control window
	/// </summary>
	/// <param name="event_">event</param>
	void OnDownArrow(wxCommandEvent& event_);
};

