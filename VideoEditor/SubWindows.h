#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/display.h>

typedef struct TimeLine{
    /*
    The timeline will have option to control the proportion
    */
    wxBoxSizer* main; // a vertical box sizer
    wxSlider* slider; // the slider for the timeline
    
    wxBoxSizer* handler; // an horizontal wxboxsizer which contain all of the propeties of the slider

    TimeLine(wxWindow*, int,int);

    virtual ~TimeLine();

};

typedef struct VideoWindow {
    wxBoxSizer* main; // a vertical box sizer
    wxBoxSizer* handler; // horizontal box sizer. contains play/pause, forward, backwards, to the beggining and to the end
    
    wxButton* pausePlay;
    wxButton* gotoStart;
    wxButton* gotoEnd;
    wxButton* frameBefore;
    wxButton* frameAfter;
    
    TimeLine* timeline;

    VideoWindow(wxWindow*);
    virtual ~VideoWindow();
    void SetBitmap(wxBitmap*, wxMemoryDC*);
protected:
    wxBitmap* frameBitmap; // the bitmap which contains the frame
    wxMemoryDC * dc;

};