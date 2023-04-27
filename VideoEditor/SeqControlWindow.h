#include <wx/scrolwin.h>
#include "SeqSourcePanel.h"
#include "ClipItemPanel.h"
class SeqControlWindow : public wxScrolledWindow
{
public:
	SeqControlWindow(wxWindow* parent, wxStaticText* title, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxHSCROLL | wxVSCROLL, const wxString& name = "scrolledWindow");
	void SetSequencePtr();
	void SetSeqName(wxCommandEvent& event_);
	void SetSequence();
private:
	std::string m_seqName;
	Sequence* m_sequencePtr;
	wxStaticText* m_title;
	wxBoxSizer* m_clipsSizer;

	void OnAddClip(wxCommandEvent& event_);

};
