#pragma once
#include "DesignConstatns.h"
#include "wx/wx.h"

#ifndef SMALL_BITMAP_BTN_CONSTATNTS
#define SMALL_BITMAP_BTN_CONSTATNTS

const wxSize BTN_ICON_SIZE = wxSize(15, 15);
const wxSize BTN_SIZE = wxSize(25, 30);

#endif

class SmallBitmapButton: public wxBitmapButton
{
public:

    SmallBitmapButton(wxWindow* parent, wxWindowID id, wxBitmap bitmap,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = BTN_SIZE, const wxSize& bitmapSize = BTN_ICON_SIZE,
        long style = wxBU_AUTODRAW, const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxButtonNameStr);
    void ChangeBitmap(wxBitmap bimap);
    wxSize GetBitmapSize();
    void RescaleBitmap();
private:
    wxSize m_iconSize;
    wxSize m_buttonSize;
    wxColour m_buttonColor;

};

