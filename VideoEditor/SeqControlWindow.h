#include <wx/scrolwin.h>
#include "SeqSourcePanel.h"
#include "ClipItemPanel.h"
#include "Client.h"
class SeqControlWindow : public wxScrolledWindow
{
public:
	SeqControlWindow(ServerClient* clientPtr, wxWindow* parent, wxStaticText* title, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxHSCROLL | wxVSCROLL, const wxString& name = "scrolledWindow");
	void SetSequencePtr();
	void SetSeqName(wxCommandEvent& event_);
	void SetSequence();
private:
	ServerClient* client;
	std::string m_seqName;
	Sequence* m_sequencePtr;
	wxStaticText* m_title;
	wxBoxSizer* m_clipsSizer;

	void SwapClips(int idx1, int idx2, bool isource = true);
	void OnAddClip(wxCommandEvent& event_);
	void OnSwapWithPrev(wxCommandEvent& event_);
	void OnSwapWithNext(wxCommandEvent& event_);

};
