#include "SmallBitmapButton.h"

SmallBitmapButton::SmallBitmapButton(wxWindow* parent, wxWindowID id, wxBitmap bitmap, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
	: wxBitmapButton(parent, id, bitmap, pos, size, style, validator, name)
{
	RescaleBitmap();
	SetBackgroundColour(m_buttonColor);
	SetSize(m_buttonSize);
}

void SmallBitmapButton::RescaleBitmap()
{
	wxImage img = this->GetBitmap().ConvertToImage();
	img.Rescale(m_iconSize.GetWidth(), m_iconSize.GetHeight(), wxIMAGE_QUALITY_HIGH);
	this->SetBitmap(img);

}
