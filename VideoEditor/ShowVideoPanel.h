#pragma once
#include <wx/panel.h>
#include "VideoSource.h"

class ShowVideoPanel : public wxPanel
{
public:
	ShowVideoPanel(wxWindow* parent);
	~ShowVideoPanel();
	void SetVideo(std::vector<AVFrame*>);
private:
	wxBoxSizer* m_mainSizer;
	wxBoxSizer* m_mediaControlSizer;

	wxButton* pausePlay;
	wxButton* gotoStart;
	wxButton* gotoEnd;
	wxButton* prevFrame;
	wxButton* nextFrame;

	wxSlider* timeline;
};

