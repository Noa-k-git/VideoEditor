#include "SeqControlWindow.h"

SeqControlWindow::SeqControlWindow(wxWindow* parent, wxStaticText* title, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
	wxScrolledWindow(parent, id, pos, size, style, name)
{
	this->SetScrollRate(FromDIP(5), FromDIP(5));
	m_title = title;
	m_clipsSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(m_clipsSizer);
	m_seqName = "";
	m_sequencePtr = nullptr;
	Bind(SET_WORKING_SEQUENCE_EVT, &SeqControlWindow::SetSeqName, this);
	Bind(ADD_TO_SEQUENCE_EVT, &SeqControlWindow::OnAddClip, this);
}

void SeqControlWindow::SetSequencePtr()
{
	m_sequencePtr = nullptr;
	auto findSeq = Sequence::sequences.Contains(m_seqName);
	if (findSeq.second) {
		m_sequencePtr = *findSeq.first;
	}
}

void SeqControlWindow::SetSeqName(wxCommandEvent& event_)
{
	m_clipsSizer->Clear();
	m_seqName = event_.GetString().ToStdString();
	m_title->SetLabel(m_seqName);
	m_title->SetFont(m_title->GetFont().Bold());
	wxMessageOutputDebug().Printf("%zu", this->GetParent()->GetSizer()->GetItemCount());
	wxMessageOutputDebug().Printf("\t\t\t%d", GetParent()->GetSizer()->GetItem(size_t(0))->GetId());

	//this->GetParent()->GetSizer()->Add(m_clipsSizer);
	SetSequencePtr();
	if (m_sequencePtr) {
		SetSequence();
	}
}

void SeqControlWindow::SetSequence()
{
	m_clipsSizer->Clear();
	for (int i = 0; i < m_sequencePtr->GetLength(); i++) {
		m_clipsSizer->Add(new ClipItemPanel(m_sequencePtr->GetClipAt(i), this, wxID_ANY), 0, wxEXPAND|wxBOTTOM, 5);
	}
	m_clipsSizer->Layout();
	Refresh();
	GetParent()->Refresh();
	SetSizerAndFit(m_clipsSizer);
}

void SeqControlWindow::OnAddClip(wxCommandEvent& event_)
{

	SetSequencePtr();
	if (m_sequencePtr) {
		auto findVid = VideoSource::videoSources.Contains(event_.GetString().ToStdString());
		if (findVid.second) {
			m_sequencePtr->AddClip(new VideoClip(*findVid.first));
			SetSequence();
		}
	}
}

