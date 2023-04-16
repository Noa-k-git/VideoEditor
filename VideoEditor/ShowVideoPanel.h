#pragma once
#include <wx/panel.h>
#include <wx/scrolbar.h>
#include "VideoSource.h"

class ShowVideoPanel : public wxPanel
{
public:
	ShowVideoPanel(wxWindow* parent);
	~ShowVideoPanel() {};
	void SetVideo(std::vector<AVFrame*>);
private:
	wxScrolledWindow* m_videoScrolledWindow; // the video window
	wxStaticBitmap* m_frameBitmap;
	double ratio;
	//Sizers
	wxBoxSizer* m_mainSizer;
	wxBoxSizer* m_mediaControlSizer;

	// media controlls
	wxButton* m_pausePlay;
	wxButton* m_gotoStart;
	wxButton* m_gotoEnd;
	wxButton* m_prevFrame;
	wxButton* m_nextFrame;

	wxSlider* timeline;

	void OnParentSize(wxSizeEvent&);
};

