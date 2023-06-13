#pragma once
#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

/// <summary>
/// A wraping to a bitmap object
/// </summary>
class BufferedBitmap : public wxScrolledWindow
{
public:
	BufferedBitmap(wxWindow* parent, const wxBitmap& bitmap);
	virtual ~BufferedBitmap();
	
	/// <summary>
	/// Painting the bitmap
	/// </summary>
	/// <param name="evt">paint event</param>
	void OnPaint(wxPaintEvent& evt);
	/// <summary>
	/// Changing the bitmap of the class and showing it
	/// </summary>
	/// <param name="bitmap">the new bitmap</param>
	void SetBitmap(const wxBitmap bitmap);
	const wxBitmap& GetBitmap() const;
	double GetZoomMultiplier() const;
	double GetZoomPercentage() const;

	/// <summary>
	/// zoom in and update the bitmap display
	/// </summary>
	void ZoomIn();
	/// <summary>
	/// zoom out and update the bitmap display
	/// </summary>
	void ZoomOut();
private:
	bool* deletedPtr;
	wxBitmap m_bitmap;
	const double ZOOM_FACTOR = 1.1;
	int zoomLevel = 0;

	wxSize GetScaledBitmapSize() const;
	/// <summary>
	/// Center the bitmap on the screen
	/// </summary>
	/// <param name="previousCenter">The previous center of the bitmap</param>
	/// <param name="currentCenter">The new center of the bitmap</param>
	void CenterAfterZoom(wxPoint previousCenter, wxPoint currentCenter);
};

