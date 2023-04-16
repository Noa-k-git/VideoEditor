#include "ShowVideoPanel.h"

ShowVideoPanel::ShowVideoPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	//auto a = GetSize();
	//int width = 1920, height = 1080;
	//SetVirtualSize(wxSize(width, height));

	//wxImage image(width, height);

	//image.SetRGB(wxRect(0, 0, width, height), 0, 0, 0);

	//wxBitmap frameBitmap(image);
	wxBitmap frameBitmap(wxSize(1, 1));
	m_frameBufferedBitmap = new BufferedBitmap(this, frameBitmap);

	m_zoomInButton = new wxButton(this, wxID_ANY, "Zoom In");
	m_zoomOutButton = new wxButton(this, wxID_ANY, "Zoom Out");

	m_frameControlSizer = new wxBoxSizer(wxHORIZONTAL);
	m_frameControlSizer->Add(m_zoomInButton, 0, wxLEFT, FromDIP(5));
	m_frameControlSizer->Add(m_zoomOutButton, 0, wxLEFT, FromDIP(5));

	m_gotoStart = new wxButton(this, wxID_ANY, "<<", wxDefaultPosition, wxSize(wxDefaultSize.GetX(), wxDefaultSize.GetY() + 50));
	m_prevFrame = new wxButton(this, wxID_ANY, "<");
	m_pausePlay = new wxButton(this, wxID_ANY, "| |");
	m_nextFrame = new wxButton(this, wxID_ANY, ">");
	m_gotoEnd = new wxButton(this, wxID_ANY, ">>");
	timeline = new wxSlider(this, wxID_ANY, 0, 0, 1, wxDefaultPosition, wxSize(-1, -1));

	wxBoxSizer* hMediabuttonSizer_ = new wxBoxSizer(wxHORIZONTAL);
	hMediabuttonSizer_->Add(m_gotoStart, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	hMediabuttonSizer_->Add(m_prevFrame, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	hMediabuttonSizer_->Add(m_pausePlay, 2, wxEXPAND | wxLEFT | wxRIGHT, 30);
	hMediabuttonSizer_->Add(m_nextFrame, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	hMediabuttonSizer_->Add(m_gotoEnd, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	
	m_mediaControlSizer = new wxBoxSizer(wxVERTICAL);
	m_mediaControlSizer->Add(hMediabuttonSizer_, 0, wxEXPAND);
	m_mediaControlSizer->Add(timeline, 0, wxEXPAND);

	m_mainSizer = new wxBoxSizer(wxVERTICAL);
	m_mainSizer->Add(m_frameBufferedBitmap, 1, wxEXPAND | wxALL, 10);
	m_mainSizer->Add(m_frameControlSizer, 0, wxALIGN_LEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
	m_mainSizer->Add(m_mediaControlSizer, 0, wxLEFT | wxRIGHT, 10);
	m_mainSizer->Layout();
	SetSizerAndFit(m_mainSizer);
	//parent->Bind(wxEVT_SIZE, &ShowVideoPanel::OnParentSize, this);

	Bind(SHOW_VIDEO_EVENT, &ShowVideoPanel::SetVideo, this);
}

void ShowVideoPanel::SetVideo(wxCommandEvent& event)
{
	auto find = VideoSource::videoSources.Contains(event.GetString().ToStdString());
	if (find.second) {
		std::thread t(&ShowVideoPanel::ShowVideo, this, (* find.first)->GetSource());
	}
}

void ShowVideoPanel::ShowVideo(std::vector<SyncObject<AVFrame*>> syncFrames)
{

}
