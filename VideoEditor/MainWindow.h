#pragma once
#include <libavutil/imgutils.h>
#include <libavutil/frame.h>
#include "SubWindows.h"
#include "VideoSourcePanel.h"
#include "VideoSource.h"
#include "VideoClip.h"
#include "Sequence.h"
#include "ShowVideoPanel.h"
#include <wx/wxprec.h>
#include <wx/splitter.h>
#include <wx/wrapsizer.h>
#include "Map.h"

 #ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

 class MainWindow: public wxFrame
{
public:
	MainWindow(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));
	~MainWindow();

    void onNew(wxCommandEvent& event_);
    void OnImport(wxCommandEvent& event_);
    void OnNewSequence(wxCommandEvent& event_);
    void onTest(wxCommandEvent& event);
    void onQuit(wxCommandEvent& event);
    void onClose(wxCloseEvent& event);
    void onHelp(wxCommandEvent& event);
    inline void OnRefresh(wxCommandEvent& event_);

    DECLARE_EVENT_TABLE();
private:
    std::mutex statusBarMutex;
    wxStatusBar* statusBar;
    ShowVideoPanel* ogShowVideoPanel;
    ShowVideoPanel* finalVideoPanel;
    wxScrolledWindow* m_sourcesWindow;
    wxWrapSizer* m_sourcesSizer;
};

