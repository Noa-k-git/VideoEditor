#pragma once
#include <wx/panel.h>
#include "VideoSource.h"
#include "Sequence.h"
#include "SmallBitmapButton.h"

wxDECLARE_EVENT(ADD_TO_SEQUENCE_EVT, wxCommandEvent);
wxDECLARE_EVENT(SHOW_VIDEO_EVT, wxCommandEvent);
wxDECLARE_EVENT(WIDGET_DELETED_EVT, wxCommandEvent);
/// <summary>
/// Class that represents a video source item 
/// </summary>
class VideoSourcePanel :
    public wxPanel
{
public:
    VideoSourcePanel(wxWindow* parent, IPlayable<AVFrame*>* videoSource, wxWindowID showWindow, wxWindowID sequenceWindow);
    ~VideoSourcePanel();
    virtual void onAddButtonClicked(wxCommandEvent& event);
    virtual void onEditButtonClicked(wxCommandEvent& event);
protected:
    wxWindowID m_showWindowID;
    wxWindowID m_sequenceWindowID;

    IPlayable<AVFrame*>* m_videoSource; // The video source object to be represented
    wxBoxSizer* m_mainSizer;
    wxBitmapButton* m_thumbnailButton; // the first avframe in the video source
    wxStaticText* m_videoName; // the name of the video source
    SmallBitmapButton* m_addButton; // add the video source to the sequence
    SmallBitmapButton* m_editButton; // change the video source name
    SmallBitmapButton* m_deleteButton; // deletes the video source (removes it from the video source list
    // TODO: check what happens when an object is getting deleted while it is displayed.
    void rescaleBitmap(wxBitmap&, const wxSize&);
    void onMouseLeftDown(wxMouseEvent& event);
    void onMouseLeftDoubleClick(wxMouseEvent& event);
    void onDeleteButtonClicked(wxCommandEvent& event);
};

