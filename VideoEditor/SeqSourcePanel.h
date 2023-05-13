#pragma once
#include "VideoSourcePanel.h"
wxDECLARE_EVENT(SET_WORKING_SEQUENCE_EVT, wxCommandEvent);
class SeqSourcePanel :
    public VideoSourcePanel
{
public:
    SeqSourcePanel(wxWindow* parent, IPlayable<AVFrame*>* videoSource, wxWindowID showWindow, wxWindowID sequenceWindow);
private:
    void onAddButtonClicked(wxCommandEvent& event_) override;
    void onEditButtonClicked(wxCommandEvent& event_) override;
};

