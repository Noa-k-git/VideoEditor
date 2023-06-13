#pragma once
#include "VideoSourcePanel.h"
wxDECLARE_EVENT(SET_WORKING_SEQUENCE_EVT, wxCommandEvent);
/// <summary>
/// The panel that represents a sequence in the sources sub-window
/// </summary>
class SeqSourcePanel :
    public VideoSourcePanel
{
public:
    SeqSourcePanel(wxWindow* parent, IPlayable<AVFrame*>* videoSource, wxWindowID showWindow, wxWindowID sequenceWindow);
private:
    void onAddButtonClicked(wxCommandEvent& event_) override;
    void onEditButtonClicked(wxCommandEvent& event_) override;
};

