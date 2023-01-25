#include "SubWindows.h"
#include "VideoSource.h"
TimeLine::TimeLine(wxWindow* parent, int start, int end) {
	main = new wxBoxSizer(wxVERTICAL);
	slider = new wxSlider(parent, wxID_ANY, 0, start, end, wxDefaultPosition, wxSize(-1, -1));
	//slider->SetThumbLength(50);
	//slider->SetLineSize(1);
	handler = new wxBoxSizer(wxHORIZONTAL);
	main->Add(slider);//, 1, wxEXPAND | wxSHAPED | wxALL, 10);
	main->Add(handler);
}

TimeLine::~TimeLine() {
	delete main;
	delete slider;
	delete handler;
}

VideoWindow::VideoWindow(wxWindow* parent, int start, int end) {
	
	this->parent = parent;
	//wxPanel* p = new wxPanel(parent, wxID_ANY);
	main = new wxBoxSizer(wxVERTICAL);
	handlerSizer = new wxBoxSizer(wxHORIZONTAL);
	// create the frame with temp values
	
	int width = 1920, height = 1080;
	ratio = width*1.0/height;
	wxImage image(width, height);

	image.SetRGB(wxRect(0, 0, width, height), 0, 0, 0);
	
	frameBitmap = new wxBitmap(image);
	frameDisplay = new wxStaticBitmap(parent, wxID_ANY, *frameBitmap);

	//wxString path("D:\\Downloads\\WhatsApp Image 2020-12-03 at 10.50.45.jpeg");
	//frameBitmap = new wxBitmap(path, wxBITMAP_TYPE_ANY);
	
	gotoStart = new wxButton(parent, wxID_ANY, "<<", wxDefaultPosition, wxSize(wxDefaultSize.GetX(), wxDefaultSize.GetY() + 50));
	frameBefore = new wxButton(parent, wxID_ANY, "<");
	pausePlay = new wxButton(parent, wxID_ANY, "| |");
	frameAfter = new wxButton(parent, wxID_ANY, ">");
	gotoEnd = new wxButton(parent, wxID_ANY, ">>");

	handlerSizer->Add(gotoStart, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	handlerSizer->Add(frameBefore, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	handlerSizer->Add(pausePlay, 2, wxEXPAND | wxLEFT | wxRIGHT, 30);
	handlerSizer->Add(frameAfter, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	handlerSizer->Add(gotoEnd, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);

	timeline = new TimeLine(parent, start, end);

	main->Add(frameDisplay, 1, wxEXPAND | wxALL, 10);
	main->Add(handlerSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
	main->Add(timeline->main, 0);

	parent->Bind(wxEVT_SIZE, &VideoWindow::OnParentSize, this);
}

VideoWindow::~VideoWindow() {
	delete main;
	delete frameDisplay;
	delete frameBitmap;
	delete handlerSizer;
	
	delete pausePlay;
	delete gotoStart;
	delete gotoEnd;
	delete frameBefore;
	delete frameAfter;
	delete timeline;
	
}

void VideoWindow::SetBitmap(wxBitmap * newFrame, wxMemoryDC * newDC) {
	delete frameBitmap;
	//delete dc;
	delete frameDisplay;
	frameBitmap = newFrame;
	//dc = newDC;
	frameDisplay = new wxStaticBitmap(parent, wxID_ANY, *frameBitmap);
}

void VideoWindow::OnParentSize(wxSizeEvent& event) {
	event.Skip(); // First call the base resize method
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
		wxImage* image = new wxImage(frameBitmap->ConvertToImage());
		int a = (int)width, u = (int)height;
		image->Rescale((int)width, (int)height, wxIMAGE_QUALITY_HIGH);
		delete frameBitmap;
		frameBitmap = new wxBitmap(*image);
		delete image;
		frameDisplay->SetBitmap(*frameBitmap);
		frameDisplay->Refresh();
		
		this->timeline->slider->SetSize((int)width,-1);// , (int)width, -1);
		
		this->timeline->main->Layout();
		this->main->Layout();
	}

}
void VideoWindow::OnPaint(wxPaintEvent& event) // not in use
{
	wxPaintDC dc(this->parent);
	dc.DrawBitmap(*frameBitmap, 0, 0);
}
