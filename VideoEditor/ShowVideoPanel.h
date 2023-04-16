#pragma once
#include <wx/panel.h>
#include <wx/scrolbar.h>
#include "VideoSource.h"
#include "BufferedBitmap.h"

class ShowVideoPanel : public wxPanel
{
public:
	ShowVideoPanel(wxWindow* parent);
	~ShowVideoPanel() {};
	void SetVideo(std::vector<AVFrame*>);
private:
	BufferedBitmap* m_frameBufferedBitmap; // the video window
	
	//Sizers
	wxBoxSizer* m_mainSizer; // The main sizer
	wxBoxSizer* m_frameControlSizer; // The sizer with frame display controllers (zoom-in/out)
	wxBoxSizer* m_mediaControlSizer; // The sizer of media controls

	// frame display controllers

	wxButton* m_zoomInButton;
	wxButton* m_zoomOutButton;

	// media controls
	wxButton* m_pausePlay; // pause - play button
	wxButton* m_gotoStart; // goto start button
	wxButton* m_gotoEnd; // goto end button
	wxButton* m_prevFrame; // previous frame button
	wxButton* m_nextFrame; // next frame button

	wxSlider* timeline;

};

