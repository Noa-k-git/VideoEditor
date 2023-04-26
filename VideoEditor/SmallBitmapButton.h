#pragma once
#include "wx/wx.h"
class SmallBitmapButton: public wxBitmapButton
{
public:
    const wxSize m_iconSize = wxSize(15, 15);
    const wxSize m_buttonSize = wxSize(25, 30);
    const wxColour m_buttonColor = wxColor(80, 80, 80);
    SmallBitmapButton(wxWindow* parent, wxWindowID id, wxBitmap bitmap,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxBU_AUTODRAW, const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxButtonNameStr);
    void ChangeBitmap(wxBitmap bimap);
private:
    void RescaleBitmap();

};

