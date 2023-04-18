#pragma once
#include <wx/panel.h>
#include "VideoSource.h"
#include "SmallBitmapButton.h"


wxDECLARE_EVENT(SHOW_VIDEO_EVENT, wxCommandEvent);
wxDECLARE_EVENT(WIDGET_DELETED_EVENT, wxCommandEvent);

class VideoSourcePanel :
    public wxPanel
{
public:
    VideoSourcePanel(wxWindow* parent, VideoSource* videoSource, wxWindowID showWindow);
    ~VideoSourcePanel();
private:
    wxWindowID m_showWindowID;

    VideoSource* m_videoSource; // The video source object to be represented
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
    void onAddButtonClicked(wxCommandEvent& event);
    void onEditButtonClicked(wxCommandEvent& event);
    void onDeleteButtonClicked(wxCommandEvent& event);
};

