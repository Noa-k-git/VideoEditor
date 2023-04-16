#include "ShowVideoPanel.h"

ShowVideoPanel::ShowVideoPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	m_mainSizer = new wxBoxSizer(wxVERTICAL);
	m_mediaControlSizer = new wxBoxSizer(wxHORIZONTAL);

	int width = 1920, height = 1080;
	ratio = width * 1.0 / height;
	SetVirtualSize(wxSize(width, height));

	wxImage image(width, height);

	image.SetRGB(wxRect(0, 0, width, height), 0, 0, 0);

	wxBitmap frameBitmap(image);
	m_videoScrolledWindow = new wxScrolledWindow(this, wxID_ANY);
	m_frameBitmap = new wxStaticBitmap(m_videoScrolledWindow, wxID_ANY, frameBitmap);


	m_gotoStart = new wxButton(this, wxID_ANY, "<<", wxDefaultPosition, wxSize(wxDefaultSize.GetX(), wxDefaultSize.GetY() + 50));
	m_prevFrame = new wxButton(this, wxID_ANY, "<");
	m_pausePlay = new wxButton(this, wxID_ANY, "| |");
	m_nextFrame = new wxButton(this, wxID_ANY, ">");
	m_gotoEnd = new wxButton(this, wxID_ANY, ">>");

	m_mediaControlSizer->Add(m_gotoStart, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	m_mediaControlSizer->Add(m_prevFrame, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	m_mediaControlSizer->Add(m_pausePlay, 2, wxEXPAND | wxLEFT | wxRIGHT, 30);
	m_mediaControlSizer->Add(m_nextFrame, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	m_mediaControlSizer->Add(m_gotoEnd, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);

	timeline = new wxSlider(this, wxID_ANY, 0, 0, 1, wxDefaultPosition, wxSize(-1, -1));

	m_mainSizer->Add(m_videoScrolledWindow, 1, wxEXPAND | wxALL, 10);
	m_mainSizer->Add(m_mediaControlSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
	m_mainSizer->Add(timeline, 0);
	m_mainSizer->SetSizeHints(this);
	m_mainSizer->Layout();
	SetSizer(m_mainSizer);
	Fit();
	parent->Bind(wxEVT_SIZE, &ShowVideoPanel::OnParentSize, this);

}
void ShowVideoPanel::SetVideo(std::vector<AVFrame*>)
{
}

void ShowVideoPanel::OnParentSize(wxSizeEvent& event)
{
	event.Skip(); // First call the base resize method
	//main->Layout();
	/*wxSize newSize = main->GetSize();
	float width = newSize.GetWidth();
	float height = newSize.GetHeight();*/
	wxSize newSize = event.GetSize();
	float width = newSize.GetWidth();
	float height = newSize.GetHeight();
	if (width > 0 && height > 0) {
		/*
		int bitmapWidth = frameBitmap->GetWidth();
		int bitmapHeight = frameBitmap->GetHeight();

		double ratio = bitmapWidth * 1.0 / bitmapHeight;
		*/
		if (width / ratio <= height) {
			height = width / ratio;
		}
		else {
			width = height * ratio;
		}
	}
	SetSize(wxSize((int)width, (int)height));
	/*wxImage image(m_frameBitmap->GetBitmap().ConvertToImage());
	
	int a = (int)width, u = (int)height;
	image.Rescale((int)width, (int)height, wxIMAGE_QUALITY_HIGH);
	wxBitmap bitmap(image);
	m_frameBitmap->SetBitmap(bitmap);
	m_videoScrolledWindow->Refresh();

	this->m_mainSizer->Layout();*/
}
