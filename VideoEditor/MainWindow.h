#pragma once
#include <wx/wxprec.h>
 #ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/display.h>
#include <wx/artprov.h>
#include "VideoSource.h"
#include "SubWindows.h"
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
    MainWindow(int a, wxWindow* parent,
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

