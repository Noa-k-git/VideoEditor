#include "BufferedBitmap.h"

BufferedBitmap::BufferedBitmap(wxWindow* parent, const wxBitmap& bitmap)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxVSCROLL|wxHSCROLL)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT); // needed for windows

	this->Bind(wxEVT_PAINT, &BufferedBitmap::OnPaint, this);
	this->SetBitmap(bitmap);
}

void BufferedBitmap::OnPaint(wxPaintEvent& evt)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    DoPrepareDC(dc);

    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    if (gc)
    {
        // scaling consistent with wxStaticBitmap
        const wxSize drawSize = ToDIP(GetVirtualSize());

        const wxSize bmpSize = GetScaledBitmapSize();

        double w = bmpSize.GetWidth();
        double h = bmpSize.GetHeight();

        double x = (drawSize.GetWidth() - w) / 2;
        double y = (drawSize.GetHeight() - h) / 2;
        
        gc->DrawBitmap(m_bitmap, gc->FromDIP(x), gc->FromDIP(y), gc->FromDIP(w), gc->FromDIP(h));

        delete gc;
    }
}

void BufferedBitmap::SetBitmap(const wxBitmap bitmap)
{
    this->m_bitmap = bitmap;

    SetScrollRate(FromDIP(10), FromDIP(10));
    
    SetVirtualSize(FromDIP(GetScaledBitmapSize()));
    auto a = GetVirtualSize();
    this->Refresh();
}

const wxBitmap& BufferedBitmap::GetBitmap() const
{
    return m_bitmap;
}

double BufferedBitmap::GetZoomMultiplier() const
{
    return pow(ZOOM_FACTOR, zoomLevel);
}

double BufferedBitmap::GetZoomPercentage() const
{
    return GetZoomMultiplier() * 100;
}

void BufferedBitmap::ZoomIn()
{
    auto centerPos = CalcUnscrolledPosition(wxPoint(GetClientSize().GetWidth() / 2, GetClientSize().GetHeight() / 2));

    zoomLevel++;

    CenterAfterZoom(centerPos, centerPos * ZOOM_FACTOR);
    SetVirtualSize(FromDIP(GetScaledBitmapSize()));

    this->Refresh();
}

void BufferedBitmap::ZoomOut()
{
    auto centerPos = CalcUnscrolledPosition(wxPoint(GetClientSize().GetWidth() / 2, GetClientSize().GetHeight() / 2));

    zoomLevel--;

    CenterAfterZoom(centerPos, centerPos * (1.0 / ZOOM_FACTOR));
    SetVirtualSize(FromDIP(GetScaledBitmapSize()));

    this->Refresh();
}

wxSize BufferedBitmap::GetScaledBitmapSize() const
{
    const wxSize bmpSize = m_bitmap.GetSize();
    const double zoom = GetZoomMultiplier();
    return wxSize(bmpSize.GetWidth() * zoom, bmpSize.GetHeight() * zoom);
}

void BufferedBitmap::CenterAfterZoom(wxPoint previousCenter, wxPoint currentCenter)
{
    wxPoint pixelsPerUnit;
    GetScrollPixelsPerUnit(&pixelsPerUnit.x, &pixelsPerUnit.y);

    auto delta = currentCenter - previousCenter;

    auto destX = GetViewStart().x + delta.x / pixelsPerUnit.x;
    auto destY = GetViewStart().y + delta.y / pixelsPerUnit.y;

    Scroll(destX, destY);
}
