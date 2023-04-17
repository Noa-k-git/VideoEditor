#include "ShowVideoPanel.h"

#undef av_err2str(errnum)
//extern char x[AV_ERROR_MAX_STRING_SIZE];
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
	paused = true;
	syncFrames = new std::vector<SyncObject<AVFrame*>>();
	wxBitmap frameBitmap(wxSize(1, 1));
	m_frameBufferedBitmap = new BufferedBitmap(this, frameBitmap);

	m_zoomInButton = new wxButton(this, wxID_ANY, "Zoom In");
	m_zoomOutButton = new wxButton(this, wxID_ANY, "Zoom Out");

	m_frameControlSizer = new wxBoxSizer(wxHORIZONTAL);
	m_frameControlSizer->Add(m_zoomInButton, 0, wxLEFT, FromDIP(5));
	m_frameControlSizer->Add(m_zoomOutButton, 0, wxLEFT, FromDIP(5));

	m_gotoStart = new wxButton(this, wxID_ANY, "<<", wxDefaultPosition);
	m_prevFrame = new wxButton(this, wxID_ANY, "<");
	m_pausePlay = new wxButton(this, wxID_ANY, "| |");
	m_nextFrame = new wxButton(this, wxID_ANY, ">");
	m_gotoEnd = new wxButton(this, wxID_ANY, ">>");
	timeline = new wxSlider(this, wxID_ANY, 0, 0, 1, wxDefaultPosition);

	wxBoxSizer* hMediabuttonSizer_ = new wxBoxSizer(wxHORIZONTAL);
	hMediabuttonSizer_->Add(m_gotoStart, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);
	hMediabuttonSizer_->Add(m_prevFrame, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);
	hMediabuttonSizer_->Add(m_pausePlay, 2, wxEXPAND | wxLEFT | wxRIGHT, 5);
	hMediabuttonSizer_->Add(m_nextFrame, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);
	hMediabuttonSizer_->Add(m_gotoEnd, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);
	
	m_mediaControlSizer = new wxBoxSizer(wxVERTICAL);
	m_mediaControlSizer->Add(hMediabuttonSizer_, 0, wxEXPAND);
	m_mediaControlSizer->Add(timeline, 0, wxEXPAND);

	m_mainSizer = new wxBoxSizer(wxVERTICAL);
	m_mainSizer->Add(m_frameBufferedBitmap, 1, wxEXPAND | wxALL, 10);
	m_mainSizer->Add(m_frameControlSizer, 0, wxALIGN_LEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
	m_mainSizer->Add(m_mediaControlSizer, 0, wxEXPAND| wxLEFT | wxRIGHT, 10);
	m_mainSizer->Layout();
	SetSizerAndFit(m_mainSizer);
	//parent->Bind(wxEVT_SIZE, &ShowVideoPanel::OnParentSize, this);

	Bind(SHOW_VIDEO_EVENT, &ShowVideoPanel::SetVideo, this);
	m_zoomInButton->Bind(wxEVT_BUTTON, &ShowVideoPanel::OnZoomIn, this);
	m_zoomOutButton->Bind(wxEVT_BUTTON, &ShowVideoPanel::OnZoomOut, this);
	m_pausePlay->Bind(wxEVT_BUTTON, &ShowVideoPanel::OnPausePlay, this);
	timeline->Bind(wxEVT_SLIDER, &ShowVideoPanel::OnTimelineScroll, this);
	//refreshTimer = new wxTimer(this, wxID_ANY);
	//this->Bind(wxEVT_TIMER, [this](wxTimerEvent&) { 
	//	m_frameBufferedBitmap->Refresh();
	//	this->Layout();
	//	}, refreshTimer->GetId());
	//refreshTimer->Start(100000);
}

void ShowVideoPanel::SetVideo(wxCommandEvent& event_)
{
	auto findVid = VideoSource::videoSources.Contains(event_.GetString().ToStdString());
	if (findVid.second) {
		syncFrames = &(*findVid.first)->GetSource();
		timeline->SetValue(0);
		timeline->SetMax(syncFrames->size());
		std::thread t(&ShowVideoPanel::ShowVideo, this);
		t.detach();
	} else {
		/*auto findSeq = Sequence::sequences.Contains(event_.GetString().ToStdString());
		if (findSeq.second) {
			std::thread t(&ShowVideoPanel::ShowVideo, this, (*findSeq.first).GetVideo());
			t.detach();
		}*/
	}
}

void ShowVideoPanel::ShowVideo()
{
	int videoLength = syncFrames->size();
	if (videoLength == 0)
		return;
	//int pos = timeline->GetValue();
	int pos = timeline->GetValue();
	auto startTime = std::chrono::steady_clock::now();
	const int fps = 30;
	auto timeInterval = duration<double>(1.0 / fps);
	//const int sleep_time_us = 1000000 / fps;  // sleep time in microseconds

	AVFrame* rgbFrame = av_frame_alloc(); // a copy frame of the first frame in the videoSource
		// set the properties of the copy to match the og frame
	auto firstFrame = syncFrames->front().GetObject();
	rgbFrame->width =firstFrame->width;
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
	while (pos < videoLength) {
		pos = timeline->GetValue();
		auto& syncframe = syncFrames->at(pos);
		pos++;

		timeline->SetValue(pos);
		auto frame = syncframe.GetObject();


		// Convert the frame from YUV to RGB
		sws_scale(swsContext, frame->data, frame->linesize, 0,
			frame->height, rgbFrame->data, rgbFrame->linesize);

		wxImage image(rgbFrame->width, rgbFrame->height, rgbFrame->data[0], true);
		while(duration<double>(std::chrono::steady_clock::now() - startTime).count() < timeInterval.count())
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
			return;
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
	paused.store(!x);
	if (x) {
		std::thread t(&ShowVideoPanel::ShowVideo, this);
		t.detach();
	}

}

void ShowVideoPanel::OnTimelineScroll(wxCommandEvent& event)
{
	this->paused.store(true);
	std::thread t(&ShowVideoPanel::ShowVideo, this);
	t.detach();
}
