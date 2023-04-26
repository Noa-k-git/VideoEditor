#pragma once
#include <wx/panel.h>
#include <wx/scrolbar.h>
#include <chrono>
#include<atomic>
#include <memory>
#include "App.h"
#include "VideoSourcePanel.h"
#include "BufferedBitmap.h"
#include "Sequence.h"

// TODO: add a variable that contains the name of the sequence/videosource and then check if the name still exists
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
	SmallBitmapButton* m_pausePlay; // pause - play button
	SmallBitmapButton* m_gotoStart; // goto start button
	SmallBitmapButton* m_gotoEnd; // goto end button
	SmallBitmapButton* m_prevFrame; // previous frame button
	SmallBitmapButton* m_nextFrame; // next frame button

	wxSlider* timeline;

	//wxTimer* refreshTimer;

	std::atomic<bool> paused;
	std::string m_vidName;
	IPlayable<AVFrame*>* m_playablePtr;

	void PauseVideo();
	void PlayVideo();
	void SetVideoName(wxCommandEvent& event);
	void SetVideo();
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

