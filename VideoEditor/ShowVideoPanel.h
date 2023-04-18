#pragma once
#include <wx/panel.h>
#include <wx/scrolbar.h>
#include <chrono>
#include<atomic>
#include "App.h"
#include "VideoSourcePanel.h"
#include "BufferedBitmap.h"
#include "Sequence.h"

template <typename T>
using duration = std::chrono::duration<T>;

class ShowVideoPanel : public wxPanel
{
public:
	ShowVideoPanel(wxWindow* parent);
	~ShowVideoPanel() {};
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

	//wxTimer* refreshTimer;

	std::atomic<bool> paused;
	std::vector<SyncObject<AVFrame*>>* syncFrames;

	void SetVideo(wxCommandEvent& event);
	void ShowVideo();
	void OnZoomIn(wxCommandEvent& event);
	void OnZoomOut(wxCommandEvent& event);
	void OnPausePlay(wxCommandEvent& event);
	void OnGotoStart(wxCommandEvent& event_);
	void OnGotoEnd(wxCommandEvent& event_);
	void OnPrevFrame(wxCommandEvent& event_);
	void OnNextFrame(wxCommandEvent& event_);
	void OnTimelineScroll(wxCommandEvent& event);
};
