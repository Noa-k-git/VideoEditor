#include "SeqSourcePanel.h"

wxDEFINE_EVENT(SET_WORKING_SEQUENCE_EVT, wxCommandEvent);
SeqSourcePanel::SeqSourcePanel(wxWindow* parent, IPlayable<AVFrame*>* videoSource, wxWindowID showWindow, wxWindowID sequenceWindow)
	: VideoSourcePanel(parent, videoSource, showWindow, sequenceWindow)
{
	m_addButton->Unbind(wxEVT_BUTTON, &VideoSourcePanel::onAddButtonClicked, this);
	m_addButton->Bind(wxEVT_BUTTON, &SeqSourcePanel::onAddButtonClicked, this);
}
void SeqSourcePanel::onAddButtonClicked(wxCommandEvent& event_)
{
	wxCommandEvent newEvt(SET_WORKING_SEQUENCE_EVT, GetId());
	newEvt.SetEventObject(this);
	newEvt.SetString(m_videoSource->GetName());
	wxWindow* window = FindWindowById(m_sequenceWindowID);
	if (window != nullptr) {
		wxPostEvent(window, newEvt);
	}

	wxMouseEvent mEvt(wxEVT_LEFT_DOWN);
	this->onMouseLeftDown(mEvt);
}

void SeqSourcePanel::onEditButtonClicked(wxCommandEvent& event_)
{
	Sequence::sequences.UpdateName(m_videoSource->GetName());
	m_videoName->SetLabelText(m_videoSource->GetName());
	m_mainSizer->Layout();
	SetSizerAndFit(m_mainSizer);
}