#include "ShowVideoPanel.h"
#undef av_err2str

#define av_err2str(errnum) \
    av_make_error_string(x, AV_ERROR_MAX_STRING_SIZE, errnum)

ShowVideoPanel::ShowVideoPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	//auto a = GetSize();
	//int width = 1920, height = 1080;
	//SetVirtualSize(wxSize(width, height));

	//wxImage image(width, height);

	//image.SetRGB(wxRect(0, 0, width, height), 0, 0, 0);

	//wxBitmap frameBitmap(image);
	SetMinSize(wxSize(50, 100));
	paused = true;
	m_playablePtr = nullptr;
	m_vidName = "";
	wxBitmap frameBitmap(wxSize(1, 1));
	m_frameBufferedBitmap = new BufferedBitmap(this, frameBitmap);

	m_zoomInButton = new wxButton(this, wxID_ANY, "Zoom In");
	m_zoomOutButton = new wxButton(this, wxID_ANY, "Zoom Out");

	m_frameControlSizer = new wxBoxSizer(wxHORIZONTAL);
	m_frameControlSizer->Add(m_zoomInButton, 0, wxLEFT, FromDIP(5));
	m_frameControlSizer->Add(m_zoomOutButton, 0, wxLEFT, FromDIP(5));

	wxBitmap gotoStartIcon("gotostart.png", wxBITMAP_TYPE_PNG);
	m_gotoStart = new SmallBitmapButton(this, wxID_ANY, gotoStartIcon, wxDefaultPosition, BTN_SIZE, BTN_ICON_SIZE, wxBU_EXACTFIT | wxBU_AUTODRAW | wxBORDER_NONE);
	m_gotoStart->SetToolTip(new wxToolTip(_("Goto Start")));

	wxBitmap prevFrameIcon("prevframe.png", wxBITMAP_TYPE_PNG);
	m_prevFrame = new SmallBitmapButton(this, wxID_ANY, prevFrameIcon, wxDefaultPosition, BTN_SIZE, BTN_ICON_SIZE, wxBU_EXACTFIT | wxBU_AUTODRAW | wxBORDER_NONE);
	m_prevFrame->SetToolTip(new wxToolTip(_("Previous Frame")));

	wxBitmap playIcon("play.png", wxBITMAP_TYPE_PNG);
	m_pausePlay = new SmallBitmapButton(this, wxID_ANY, playIcon, wxDefaultPosition, BTN_SIZE, BTN_ICON_SIZE, wxBU_EXACTFIT | wxBU_AUTODRAW | wxBORDER_NONE);
	m_pausePlay->SetToolTip(new wxToolTip(_("Play")));

	wxBitmap nextFrameIcon("nextframe.png", wxBITMAP_TYPE_PNG);
	m_nextFrame = new SmallBitmapButton(this, wxID_ANY, nextFrameIcon, wxDefaultPosition, BTN_SIZE, BTN_ICON_SIZE, wxBU_EXACTFIT | wxBU_AUTODRAW | wxBORDER_NONE);
	m_nextFrame->SetToolTip(new wxToolTip(_("Next Frame")));

	wxBitmap gotoEndIcon("gotoend.png", wxBITMAP_TYPE_PNG);
	m_gotoEnd = new SmallBitmapButton(this, wxID_ANY, gotoEndIcon, wxDefaultPosition, BTN_SIZE, BTN_ICON_SIZE, wxBU_EXACTFIT | wxBU_AUTODRAW | wxBORDER_NONE);
	m_gotoEnd->SetToolTip(new wxToolTip(_("Goto End")));

	timeline = new wxSlider(this, wxID_ANY, 0, 0, 1, wxDefaultPosition, wxDefaultSize);

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
	m_mainSizer->Add(m_mediaControlSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 0);
	m_mainSizer->Layout();
	SetSizerAndFit(m_mainSizer);
	//parent->Bind(wxEVT_SIZE, &ShowVideoPanel::OnParentSize, this);

	Bind(SHOW_VIDEO_EVT, &ShowVideoPanel::SetVideoName, this);
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

void ShowVideoPanel::PauseVideo() {
	paused.store(true);
	wxBitmap playIcon("play.png", wxBITMAP_TYPE_PNG);
	m_pausePlay->ChangeBitmap(playIcon);
	m_pausePlay->SetToolTip(new wxToolTip(_("Play")));
}
void ShowVideoPanel::PlayVideo() {
	paused.store(false);
	wxBitmap pauseIcon("pause.png", wxBITMAP_TYPE_PNG);
	m_pausePlay->ChangeBitmap(pauseIcon);
	m_pausePlay->SetToolTip(new wxToolTip(_("Pause")));
}
void ShowVideoPanel::SetVideo() {
	m_playablePtr = nullptr;
	try {
		auto findVid = VideoSource::videoSources.Contains(m_vidName);
		if (findVid.second) {
			//m_playablePtr = dynamic_cast<IPlayable<AVFrame*>*>(*findVid.first);
			m_playablePtr = dynamic_cast<IPlayable<AVFrame*>*>(*findVid.first);
		}
	}
	catch (std::bad_cast&) {
		auto findVid = Sequence::sequences.Contains(m_vidName);
		if (findVid.second) {
			m_playablePtr = dynamic_cast<IPlayable<AVFrame*>*>(*findVid.first);
		}
	}
}
void ShowVideoPanel::SetVideoName(wxCommandEvent& event_)
{
	PauseVideo();
	m_vidName = event_.GetString().ToStdString();
	SetVideo();
	timeline->SetValue(0);
	if (m_playablePtr) {
		timeline->SetMax(m_playablePtr->GetSize());
		std::thread t(&ShowVideoPanel::ShowVideo, this);
		t.detach();
	}
}

void ShowVideoPanel::ShowVideo()
{
	int videoLength;
	SetVideo();
	if (m_playablePtr)
		videoLength = m_playablePtr->GetSize();
	else
		return;
	//int pos = timeline->GetValue();
	int pos = timeline->GetValue();
	auto startTime = std::chrono::steady_clock::now();
	//const int fps = 30;
	const double fps = 30;
	auto timeInterval = duration<double>(1.0 / fps);
	//const int sleep_time_us = 1000000 / fps;  // sleep time in microseconds

	AVFrame* rgbFrame = av_frame_alloc(); // a copy frame of the first frame in the videoSource
	// set the properties of the copy to match the og frame
	AVFrame* firstFrame;
	SetVideo();
	if (m_playablePtr)
		firstFrame = m_playablePtr->GetChunk(0)->GetObject();
	else
		return;

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
		AVFrame* frame = nullptr;
		SetVideo();
		if (m_playablePtr) {
			SyncObject<AVFrame*>* syncframe = m_playablePtr->GetChunk(pos);
			frame = syncframe->GetObject();
			if (!frame)
				break;
		}
		else {
			break;
		}
		pos++;
		timeline->SetValue(pos);


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
			Layout();
			});
		if (paused.load())
			break;
	}
	// Clean up
	sws_freeContext(swsContext);
}

void ShowVideoPanel::OnZoomIn(wxCommandEvent& event)
{
	m_frameBufferedBitmap->ZoomIn();
}

void ShowVideoPanel::OnZoomOut(wxCommandEvent& event)
{
	m_frameBufferedBitmap->ZoomOut();
}

void ShowVideoPanel::OnPausePlay(wxCommandEvent& event)
{
	bool x = this->paused.load();
	if (x) {
		PlayVideo();
		std::thread t(&ShowVideoPanel::ShowVideo, this);
		t.detach();
	}
	else {
		PauseVideo();
	}

}

void ShowVideoPanel::OnTimelineScroll(wxCommandEvent& event)
{
	PauseVideo();
	std::thread t(&ShowVideoPanel::ShowVideo, this);
	t.detach();
}

void ShowVideoPanel::OnGotoStart(wxCommandEvent& event_) {
	PauseVideo();
	this->timeline->SetValue(0);
	ShowVideo();
}

void ShowVideoPanel::OnGotoEnd(wxCommandEvent& event_) {
	PauseVideo();
	this->timeline->SetValue(timeline->GetMax() - 1);
	ShowVideo();
}

void ShowVideoPanel::OnPrevFrame(wxCommandEvent& event_) {
	PauseVideo();
	this->timeline->SetValue(timeline->GetValue() - 2); // this is also solves range errors
	ShowVideo();
}

void ShowVideoPanel::OnNextFrame(wxCommandEvent& event_) {
	PauseVideo();
	ShowVideo();
}