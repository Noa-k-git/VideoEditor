#pragma once
#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

class BufferedBitmap : public wxScrolledWindow
{
public:
	BufferedBitmap(wxWindow* parent, const wxBitmap& bitmap);
	void OnPaint(wxPaintEvent& evt);
	void SetBitmap(const wxBitmap& bitmap);
	const wxBitmap& GetBitmap() const;
	double GetZoomMultiplier() const;
	double GetZoomPercentage() const;
	void ZoomIn();
	void ZoomOut();
private:
	wxBitmap m_bitmap;
	const double ZOOM_FACTOR = 1.1;
	int zoomLevel = 0;

	wxSize GetScaledBitmapSize() const;
	void CenterAfterZoom(wxPoint previousCenter, wxPoint currentCenter);
};

