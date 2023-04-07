#pragma once
#include "SubWindows.h"
#include "VideoSource.h"
#include "VideoClip.h"
#include "Sequence.h"
#include <wx/wxprec.h>
#include <wx/splitter.h>

#include <map>
 #ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
 class MainWindow: public wxFrame
{
private:
    std::map<std::string, VideoSource> videoSources;
    std::map<std::string, Sequence> sequences;
public:
	MainWindow(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));
	~MainWindow();

    void onNew(wxCommandEvent& event);
    void OnImport(wxCommandEvent& event);
    void onTest(wxCommandEvent& event);
    void onQuit(wxCommandEvent& event);
    void onClose(wxCloseEvent& event);
    void onHelp(wxCommandEvent& event);

    DECLARE_EVENT_TABLE();
};

