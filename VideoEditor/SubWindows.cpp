#include "SubWindows.h"

TimeLine::TimeLine(wxWindow* parent, int start, int end) {
	main = new wxBoxSizer(wxVERTICAL);
	slider = new wxSlider(parent, wxID_ANY, 0, start, end, wxDefaultPosition, wxSize(100, -1));
	slider->SetThumbLength(50);
	//slider->SetLineSize(1);
	handler = new wxBoxSizer(wxHORIZONTAL);

	main->Add(slider);
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
	wxImage image(width, height);
	// TODO: add a ratio porportion
	float ratio = width / height;
	image.SetRGB(wxRect(0, 0, (int)(height*ratio), height), 0, 0, 0);
	wxSize size(parent->GetSize());
	image.Scale(size.GetWidth(), size.GetHeight(), wxIMAGE_QUALITY_HIGH);
	frameBitmap = new wxBitmap(image);
	frame = new wxStaticBitmap(parent, wxID_ANY, *frameBitmap);
	
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

	main->Add(frame, 1, wxEXPAND | wxALL, 10);
	main->Add(handlerSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
	main->Add(timeline->main, 0);
}

VideoWindow::~VideoWindow() {
	delete main;
	delete frame;
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
	delete frame;
	frameBitmap = newFrame;
	//dc = newDC;
	frame = new wxStaticBitmap(parent, wxID_ANY, *frameBitmap);
}

void VideoWindow::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this->parent);
	dc.DrawBitmap(*frameBitmap, 0, 0);
}
