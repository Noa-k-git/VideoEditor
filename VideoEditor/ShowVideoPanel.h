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

template <typename T>
using duration = std::chrono::duration<T>;

template <typename T>
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
	std::shared_ptr<IPlayable<AVFrame*>> m_playablePtr;

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

#undef av_err2str

#define av_err2str(errnum) \
    av_make_error_string(x, AV_ERROR_MAX_STRING_SIZE, errnum)

template <typename T>
ShowVideoPanel<T>::ShowVideoPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	//auto a = GetSize();
	//int width = 1920, height = 1080;
	//SetVirtualSize(wxSize(width, height));

	//wxImage image(width, height);

	//image.SetRGB(wxRect(0, 0, width, height), 0, 0, 0);

	//wxBitmap frameBitmap(image);
	paused = true;
	m_playablePtr = nullptr;
	wxBitmap frameBitmap(wxSize(1, 1));
	m_frameBufferedBitmap = new BufferedBitmap(this, frameBitmap);

	m_zoomInButton = new wxButton(this, wxID_ANY, "Zoom In");
	m_zoomOutButton = new wxButton(this, wxID_ANY, "Zoom Out");

	m_frameControlSizer = new wxBoxSizer(wxHORIZONTAL);
	m_frameControlSizer->Add(m_zoomInButton, 0, wxLEFT, FromDIP(5));
	m_frameControlSizer->Add(m_zoomOutButton, 0, wxLEFT, FromDIP(5));

	m_gotoStart = new wxButton(this, wxID_ANY, "<<", wxDefaultPosition);
	m_gotoStart->SetToolTip(new wxToolTip(_("Goto Start")));

	m_prevFrame = new wxButton(this, wxID_ANY, "<");
	m_prevFrame->SetToolTip(new wxToolTip(_("Previous Frame")));

	m_pausePlay = new wxButton(this, wxID_ANY, "| |");
	m_pausePlay->SetToolTip(new wxToolTip(_("Play/Pause")));

	m_nextFrame = new wxButton(this, wxID_ANY, ">");
	m_nextFrame->SetToolTip(new wxToolTip(_("Next Frame")));

	m_gotoEnd = new wxButton(this, wxID_ANY, ">>");
	m_gotoEnd->SetToolTip(new wxToolTip(_("Goto End")));

	timeline = new wxSlider(this, wxID_ANY, 0, 0, 1, wxDefaultPosition);

	wxBoxSizer* hMediabuttonSizer_ = new wxBoxSizer(wxHORIZONTAL);
	hMediabuttonSizer_->Add(m_gotoStart, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBU_EXACTFIT, 5);
	hMediabuttonSizer_->Add(m_prevFrame, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBU_EXACTFIT, 5);
	hMediabuttonSizer_->Add(m_pausePlay, 2, wxEXPAND | wxLEFT | wxRIGHT | wxBU_EXACTFIT, 5);
	hMediabuttonSizer_->Add(m_nextFrame, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBU_EXACTFIT, 5);
	hMediabuttonSizer_->Add(m_gotoEnd, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBU_EXACTFIT, 5);

	m_mediaControlSizer = new wxBoxSizer(wxVERTICAL);
	m_mediaControlSizer->Add(hMediabuttonSizer_, 0, wxEXPAND);
	m_mediaControlSizer->Add(timeline, 0, wxEXPAND);

	m_mainSizer = new wxBoxSizer(wxVERTICAL);
	m_mainSizer->Add(m_frameBufferedBitmap, 1, wxEXPAND | wxALL, 10);
	m_mainSizer->Add(m_frameControlSizer, 0, wxALIGN_LEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
	m_mainSizer->Add(m_mediaControlSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
	m_mainSizer->Layout();
	SetSizerAndFit(m_mainSizer);
	//parent->Bind(wxEVT_SIZE, &ShowVideoPanel::OnParentSize, this);

	Bind(SHOW_VIDEO_EVENT, &ShowVideoPanel::SetVideo, this);
	m_zoomInButton->Bind(wxEVT_BUTTON, &ShowVideoPanel::OnZoomIn, this);
	m_zoomOutButton->Bind(wxEVT_BUTTON, &ShowVideoPanel::OnZoomOut, this);
	m_pausePlay->Bind(wxEVT_BUTTON, &ShowVideoPanel::OnPausePlay, this);
	m_gotoStart->Bind(wxEVT_BUTTON, &ShowVideoPanel::OnGotoStart, this);
	m_gotoEnd->Bind(wxEVT_BUTTON, &ShowVideoPanel::OnGotoEnd, this);
	m_prevFrame->Bind(wxEVT_BUTTON, &ShowVideoPanel::OnPrevFrame, this);
	m_nextFrame->Bind(wxEVT_BUTTON, &ShowVideoPanel::OnNextFrame, this);

	timeline->Bind(wxEVT_SLIDER, &ShowVideoPanel::OnTimelineScroll, this);
	//refreshTimer = new wxTimer(this, wxID_ANY);
	//this->Bind(wxEVT_TIMER, [this](wxTimerEvent&) { 
	//	m_frameBufferedBitmap->Refresh();
	//	this->Layout();
	//	}, refreshTimer->GetId());
	//refreshTimer->Start(100000);
}

template <typename T>
void ShowVideoPanel<T>::SetVideo(wxCommandEvent& event_)
{
	try {
		auto findVid = VideoSource::videoSources.Contains(event_.GetString().ToStdString());
		if (findVid.second) {
			//m_playablePtr = dynamic_cast<IPlayable<AVFrame*>*>(*findVid.first);

			m_playablePtr = std::shared_ptr<VideoSource>(*findVid.first);
			timeline->SetValue(0);
			if (m_playablePtr)
				timeline->SetMax(m_playablePtr->GetSize());
			std::thread t(&ShowVideoPanel::ShowVideo, this);
			t.detach();
		}
	}
	catch (std::bad_cast&) {
		auto findVid = Sequence::sequences.Contains(event_.GetString().ToStdString());
		if (findVid.second) {
			m_playablePtr = std::shared_ptr<Sequence>(*findVid.first);
			timeline->SetValue(0);
			timeline->SetMax(m_playablePtr.get()->GetSize());
			std::thread t(&ShowVideoPanel::ShowVideo, this);
			t.detach();
		}
	}
}
template <typename T>
void ShowVideoPanel<T>::ShowVideo()
{
	if (!m_playablePtr.get()) // if it is a null_ptr
		return;
	
	int videoLength = m_playablePtr.get()->GetSize();
	//int pos = timeline->GetValue();
	int pos = timeline->GetValue();
	auto startTime = std::chrono::steady_clock::now();
	//const int fps = 30;
	const double fps = 0.05;
	auto timeInterval = duration<double>(1.0 / fps);
	//const int sleep_time_us = 1000000 / fps;  // sleep time in microseconds

	AVFrame* rgbFrame = av_frame_alloc(); // a copy frame of the first frame in the videoSource
	// set the properties of the copy to match the og frame
	auto firstFrame = m_playablePtr.get()->GetChunk(0).GetObject();
	rgbFrame->width = firstFrame->width;
	rgbFrame->height = firstFrame->height;
	rgbFrame->format = AV_PIX_FMT_RGB24;

	// allocate memofy for the copy
	int ret = av_frame_get_buffer(rgbFrame, 0);
	if (ret < 0) {
		wxMessageBox((std::string)"ERROR: Allocat memory for VideoSourcePanel: " + av_err2str(ret));
	}

	// Set up the SwsContext* for the conversion
	SwsContext* swsContext = sws_getContext(
		firstFrame->width, firstFrame->height, (AVPixelFormat)firstFrame->format,
		rgbFrame->width, rgbFrame->height, AV_PIX_FMT_RGB24,
		SWS_BILINEAR, NULL, NULL, NULL
	);
	constexpr std::chrono::duration<double> minSleepDuration(0);
	while (pos < videoLength - 1) {
		pos = timeline->GetValue();

		if (!m_playablePtr.get()) // if nullptr
			break;
		auto& syncframe = m_playablePtr.get()->GetChunk(pos);
		pos++;
		
		timeline->SetValue(pos);

		if (std::addressof(syncframe) == nullptr)
			break;
		
		auto frame = syncframe.GetObject();


		// Convert the frame from YUV to RGB
		sws_scale(swsContext, frame->data, frame->linesize, 0,
			frame->height, rgbFrame->data, rgbFrame->linesize);

		wxImage image(rgbFrame->width, rgbFrame->height, rgbFrame->data[0], true);
		while (duration<double>(std::chrono::steady_clock::now() - startTime).count() < timeInterval.count())
		{
			auto d = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startTime);
			std::this_thread::sleep_for(minSleepDuration);
		}

		startTime = std::chrono::high_resolution_clock::now();

		wxGetApp().CallAfter([this, image]() {
			wxBitmap bitmap(image);
			m_frameBufferedBitmap->SetBitmap(bitmap);
			});
		if (paused.load())
			break;
	}
	// Clean up
	sws_freeContext(swsContext);
}
template <typename T>
void ShowVideoPanel<T>::OnZoomIn(wxCommandEvent& event)
{
	m_frameBufferedBitmap->ZoomIn();
}
template <typename T>
void ShowVideoPanel<T>::OnZoomOut(wxCommandEvent& event)
{
	m_frameBufferedBitmap->ZoomOut();
}
template <typename T>
void ShowVideoPanel<T>::OnPausePlay(wxCommandEvent& event)
{
	bool x = this->paused.load();
	paused.store(!x);
	if (x) {
		std::thread t(&ShowVideoPanel::ShowVideo, this);
		t.detach();
	}

}
template <typename T>
void ShowVideoPanel<T>::OnTimelineScroll(wxCommandEvent& event)
{
	this->paused.store(true);
	std::thread t(&ShowVideoPanel::ShowVideo, this);
	t.detach();
}
template <typename T>
void ShowVideoPanel<T>::OnGotoStart(wxCommandEvent& event_) {
	this->paused.store(true);
	this->timeline->SetValue(0);
	ShowVideo();
}
template <typename T>
void ShowVideoPanel<T>::OnGotoEnd(wxCommandEvent& event_) {
	this->paused.store(true);
	this->timeline->SetValue(timeline->GetMax() - 1);
	ShowVideo();
}
template <typename T>
void ShowVideoPanel<T>::OnPrevFrame(wxCommandEvent& event_) {
	this->paused.store(true);
	this->timeline->SetValue(timeline->GetValue() - 2); // this is also solves range errors
	ShowVideo();
}
template <typename T>
void ShowVideoPanel<T>::OnNextFrame(wxCommandEvent& event_) {
	this->paused.store(true);
	ShowVideo();
}