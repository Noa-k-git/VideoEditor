#include "ClipItemPanel.h"

ClipItemPanel::ClipItemPanel(VideoClip* videoClip, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxScrolledWindow(parent, id, pos, size, style, name)
{
	m_videoClip = videoClip;
	SetBackgroundColour(PANEL_WIDGET_BACKGROUND_COLOUR);
	wxBoxSizer* mainSizer_ = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* arrowsSizer_ = new wxBoxSizer(wxVERTICAL);
	
	wxImage rightArrow_("rightarrow.png", wxBITMAP_TYPE_PNG);
	wxImage upArrowImg_ = rightArrow_.Rotate90(false);
	wxImage downArrowImg_ = rightArrow_.Rotate90(true);

	SmallBitmapButton* upButton_ = new SmallBitmapButton(this, wxID_ANY, upArrowImg_, wxDefaultPosition, wxSize(15, 15), wxSize(10, 10), wxBU_AUTODRAW|wxBORDER_NONE);
	SmallBitmapButton* downButton_ = new SmallBitmapButton(this, wxID_ANY, downArrowImg_, wxDefaultPosition, wxSize(15, 15), wxSize(10, 10), wxBU_AUTODRAW | wxBORDER_NONE);

	upButton_->Bind(wxEVT_BUTTON, &ClipItemPanel::OnUpArrow, this);
	downButton_->Bind(wxEVT_BUTTON, &ClipItemPanel::OnDownArrow, this);

	arrowsSizer_->Add(upButton_, 0, wxBOTTOM, 3);
	arrowsSizer_->Add(downButton_);
	arrowsSizer_->Layout();
	wxStaticText* clipName_ = new wxStaticText(this, wxID_ANY, videoClip->GetName());
	clipName_->SetFont(clipName_->GetFont().Italic());
	clipName_->SetForegroundColour(TEXT_COLOUR);
	mainSizer_->Add(arrowsSizer_);
	mainSizer_->Add(clipName_, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL| wxALL , 10);
	mainSizer_->Layout();
	SetSizer(mainSizer_);
	Refresh();

}

void ClipItemPanel::OnUpArrow(wxCommandEvent& event_)
{
	// get the parent sizer
	wxBoxSizer* parentSizer = dynamic_cast<wxBoxSizer*>(GetParent()->GetSizer());

	// get the index of this button in the sizer
	int currIdx = parentSizer->GetItemCount() - 1;
	bool found = false;
	const wxSizerItemList& children = parentSizer->GetChildren();
	wxSizerItemList::const_iterator it = children.begin(), end = children.end();
	it++;
	int idx = 0;
	wxMessageOutputDebug().Printf("\thj\t%d", currIdx+1);

	for (; it != end; it++) {
		idx++;
		wxSizerItem* p_item = *it;
		if (p_item->GetWindow() == this) {
			currIdx = idx;
			found = true;
			break;
		}
	}
	
	if (found)
	{
		// detach this button from the sizer
		parentSizer->Detach(this);

		// insert this button above the button before it
		parentSizer->Insert(currIdx - 1, this);

		// refresh the sizer to update the layout
		GetParent()->Layout();
	}
}

void ClipItemPanel::OnDownArrow(wxCommandEvent& event_)
{
	// //get the parent sizer
	//wxBoxSizer* parentSizer = dynamic_cast<wxBoxSizer*>(GetParent()->GetSizer());

	//// get the index of this button in the sizer
	//int currIdx = parentSizer->GetItemCount() - 1;
	//bool found = false;
	//parentSizer->GetChildren().Find(this);
	//for (int idx = 0; idx < parentSizer->GetItemCount()-1; idx++) {
	//	if ((*parentSizer->GetChildren().Item(idx)) >GetItem(idx)->GetId() == this->GetId()) {
	//		currIdx = idx;
	//		found = true;
	//		break;
	//	}
	//}
	//if (found) {

	//	// detach this button from the sizer
	//	parentSizer->Detach(this);

	//	// insert this button above the button before it
	//	parentSizer->Insert(currIdx + 1, this);

	//	// refresh the sizer to update the layout
	//	GetParent()->Layout();
	//}
		// get the parent sizer
	wxBoxSizer* parentSizer = dynamic_cast<wxBoxSizer*>(GetParent()->GetSizer());

	// get the index of this button in the sizer
	int currIdx = parentSizer->GetItemCount() - 1;
	bool found = false;
	const wxSizerItemList& children = parentSizer->GetChildren();
	wxSizerItemList::const_iterator it = children.begin(), end = children.end();
	end--;
	int idx = 0;
	for (; it != end; ++it) {
		idx++;
		wxSizerItem* p_item = *it;
		if (p_item->GetWindow() == this) {
			currIdx = idx;
			found = true;
			break;
		}
	}

	if (found)
	{
		// detach this button from the sizer
		parentSizer->Detach(this);

		// insert this button above the button before it
		parentSizer->Insert(currIdx - 1, this);

		// refresh the sizer to update the layout
		GetParent()->Layout();
	}
}
