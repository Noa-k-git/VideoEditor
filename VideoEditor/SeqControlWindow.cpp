#include "SeqControlWindow.h"

SeqControlWindow::SeqControlWindow(ServerClient* clientPtr, wxWindow* parent, wxStaticText* title, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
	wxScrolledWindow(parent, id, pos, size, style, name)
{
	client = clientPtr;
	this->SetScrollRate(FromDIP(5), FromDIP(5));
	m_title = title;
	m_clipsSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(m_clipsSizer);
	m_seqName = "";
	m_sequencePtr = nullptr;
	Bind(SET_WORKING_SEQUENCE_EVT, &SeqControlWindow::SetSeqName, this);
	Bind(ADD_TO_SEQUENCE_EVT, &SeqControlWindow::OnAddClip, this);
	//Bind(SWAP_CLIP_WITH_PREV_EVT, [&](wxCommandEvent& event_) {
	//	wxMessageOutputDebug().Printf("here!");
	//	SeqControlWindow::SwapClips(event_.GetInt() - 1, event_.GetInt());
	//	}, this);
	//Bind(SWAP_CLIP_WITH_PREV_EVT, [&](wxCommandEvent& event_) {
	//	SeqControlWindow::SwapClips(event_.GetInt() + 1, event_.GetInt());
	//	}, this);
	Bind(SWAP_CLIP_WITH_PREV_EVT, &SeqControlWindow::OnSwapWithPrev, this);
	Bind(SWAP_CLIP_WITH_NEXT_EVT, &SeqControlWindow::OnSwapWithNext, this);
	Bind(SWAP_CLIP_SERVER_EVT, [=](wxCommandEvent& event_) {
		int idx1 = event_.GetInt(), idx2 = event_.GetExtraLong();
		if (m_sequencePtr && event_.GetString().ToStdString() == m_sequencePtr->GetName()) {
			this->SwapClips(event_.GetInt(), event_.GetExtraLong());
		} else {
			auto findSeq = Sequence::sequences.Contains(event_.GetString().ToStdString());
			if (findSeq.second)
				(*findSeq.first)->SwapClipsAt(idx1, idx2);
			else
				wxMessageBox("Message", "Please Pull Changes", wxICON_INFORMATION);
		}
		});

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
	wxMessageOutputDebug().Printf("here2");

	m_clipsSizer->Clear(true);
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
	m_clipsSizer->Clear(true);
	for (int i = 0; i < m_sequencePtr->GetLength(); i++) {
		m_clipsSizer->Add(new ClipItemPanel(m_sequencePtr->GetClipAt(i), i, this, wxID_ANY), 0, wxEXPAND|wxBOTTOM, 10);
	}
	m_clipsSizer->Layout();
	GetParent()->GetSizer()->Layout();

}

void SeqControlWindow::SwapClips(int idx1, int idx2)
{
	if (m_sequencePtr) {
		if (idx1 > idx2) {
			idx1 += idx2;
			idx2 = idx1 - idx2;
			idx1 = idx1 - idx2;
		}
		if (!m_sequencePtr->SwapClipsAt(idx1, idx2))
			return;
		wxSizerItem* item1 = m_clipsSizer->GetItem(idx1);
		wxSizerItem* item2 = m_clipsSizer->GetItem(idx2);

		ClipItemPanel* clip1 = new ClipItemPanel(m_sequencePtr->GetClipAt(idx1), idx1, this);
		ClipItemPanel* clip2 = new ClipItemPanel(m_sequencePtr->GetClipAt(idx2), idx2, this);
		m_clipsSizer->Replace(item1->GetWindow(), clip1);
		m_clipsSizer->Replace(item2->GetWindow(), clip2);

		m_clipsSizer->Layout();
		Layout();
		Refresh();
		client->SetSwapWindow(this);
		client->Switch(m_seqName, idx1, idx2);
	}
}

void SeqControlWindow::OnAddClip(wxCommandEvent& event_)
{

	SetSequencePtr();
	if (m_sequencePtr) {
		auto findVid = VideoSource::videoSources.Contains(event_.GetString().ToStdString());
		if (findVid.second) {
			m_sequencePtr->AddClip(new VideoClip(*findVid.first));
			int idx = m_sequencePtr->GetLength() - 1;
			m_clipsSizer->Add(new ClipItemPanel(m_sequencePtr->GetClipAt(idx), idx, this, wxID_ANY), 0, wxEXPAND | wxBOTTOM, 10);
			//SetSequence();
			m_clipsSizer->Layout();
			GetParent()->GetSizer()->Layout();
		}
	}
}

void SeqControlWindow::OnSwapWithPrev(wxCommandEvent& event_)
{
	SeqControlWindow::SwapClips(event_.GetInt() - 1, event_.GetInt());
}

void SeqControlWindow::OnSwapWithNext(wxCommandEvent& event_)
{
	SeqControlWindow::SwapClips(event_.GetInt() + 1, event_.GetInt());

}




