#include "SmallBitmapButton.h"

SmallBitmapButton::SmallBitmapButton(wxWindow* parent, wxWindowID id, wxBitmap bitmap, const wxPoint& pos, const wxSize& size, const wxSize& bitmapSize, long style, const wxValidator& validator, const wxString& name)
	: wxBitmapButton(parent, id, bitmap, pos, wxDefaultSize, style, validator, name)
{
	m_iconSize = bitmapSize;
	m_buttonSize = size;
	m_buttonColor = BTN_BACKGROUND_COLOUR;

	RescaleBitmap();
	SetBackgroundColour(m_buttonColor);
	SetSize(m_buttonSize);
}

void SmallBitmapButton::ChangeBitmap(wxBitmap bitmap) {
	this->SetBitmap(bitmap);
	RescaleBitmap();
}
void SmallBitmapButton::RescaleBitmap()
{
	wxImage img = this->GetBitmap().ConvertToImage();
	img.Rescale(m_iconSize.GetWidth(), m_iconSize.GetHeight(), wxIMAGE_QUALITY_HIGH);
	wxImage finalImg(m_buttonSize);
	finalImg.InitAlpha();
	// Set all pixels in the image to white
	for (int x = 0; x < m_buttonSize.GetWidth(); x++) {
		for (int y = 0; y < m_buttonSize.GetHeight(); y++) {
			finalImg.SetRGB(x, y, 255, 255, 255); // set the pixel color to white
			finalImg.SetAlpha(x, y, 0); // set the pixel alpha to fully opaque
		}
	}
	wxPoint pastePos((m_buttonSize.GetWidth() - m_iconSize.GetWidth()) / 2, (m_buttonSize.GetHeight() - m_iconSize.GetHeight()) / 2);
	finalImg.Paste(img, pastePos.x, pastePos.y);

	this->SetBitmap(finalImg);

}
