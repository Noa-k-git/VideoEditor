#pragma once
#include <wx/panel.h>
#include "VideoSource.h"
class VideoSourcePanel :
    public wxPanel
{
public:
    VideoSourcePanel(wxWindow* parent, VideoSource* videoSource);
    ~VideoSourcePanel();
private:
    VideoSource* m_videoSource; // The video source object to be represented
    wxStaticBitmap* m_thumbnaeil; // the first avframe in the video source
    wxStaticText* m_videoName; // the name of the video source
    wxButton* m_addButton; // add the video source to the sequence
    wxButton* m_editButton; // change the video source name
    wxButton* m_deleteButton; // deletes the video source (removes it from the video source list
    

};

