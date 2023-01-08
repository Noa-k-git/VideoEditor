#include "SubWindows.h"

TimeLine::TimeLine(wxWindow* parent, int start, int end) {
	main = new wxBoxSizer(wxVERTICAL);
	slider = new wxSlider(parent, wxID_ANY, 0, start, end);
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
	main = new wxBoxSizer(wxVERTICAL);
	handler = new wxBoxSizer(wxHORIZONTAL);
	// create the frame with temp values
	int width = 1080, height = 1920;
	frameBitmap = new wxBitmap(width, height);
	dc = new wxMemoryDC(*frameBitmap);
	dc->SetBrush(wxBrush((0, 0, 0)));
	dc->DrawRectangle(0, 0, width, height);
	//dc->Clear();

	frame = new wxStaticBitmap(parent, wxID_ANY, *frameBitmap);
	
	gotoStart = new wxButton(parent, wxID_ANY, "<<", wxDefaultPosition, wxSize(wxDefaultSize.GetX(), wxDefaultSize.GetY() + 50));
	frameBefore = new wxButton(parent, wxID_ANY, "<");
	pausePlay = new wxButton(parent, wxID_ANY, "| |");
	frameAfter = new wxButton(parent, wxID_ANY, ">");
	gotoEnd = new wxButton(parent, wxID_ANY, ">>");

	handler->Add(gotoStart, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	handler->Add(frameBefore, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	handler->Add(pausePlay, 2, wxEXPAND | wxLEFT | wxRIGHT, 30);
	handler->Add(frameAfter, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);
	handler->Add(gotoEnd, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);

	timeline = new TimeLine(parent, start, end);

	main->Add(frame, 0, wxEXPAND | wxALL, 10);
	main->Add(handler, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
	main->Add(timeline->main, 0, 0);
}

VideoWindow::~VideoWindow() {
	delete main;
	delete frameBitmap;
	delete handler;
	
	delete pausePlay;
	delete gotoStart;
	delete gotoEnd;
	delete frameBefore;
	delete frameAfter;
	delete timeline;
	
}

void VideoWindow::SetBitmap(wxBitmap * newFrame, wxMemoryDC * newDC) {
	delete frameBitmap;
	delete dc;
	delete frame;
	frameBitmap = newFrame;
	dc = newDC;
	frame = new wxStaticBitmap(parent, wxID_ANY, *frameBitmap);
}

void VideoWindow::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this->parent);
	dc.DrawBitmap(m_bitmap, 0, 0);
}
