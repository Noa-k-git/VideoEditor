#pragma once
#include <libavutil/imgutils.h>
#include <libavutil/frame.h>

#include "SubWindows.h"
#include "VideoSourcePanel.h"
#include "SeqSourcePanel.h"
#include "SeqControlWindow.h"
#include "VideoSource.h"
#include "VideoClip.h"
#include "Sequence.h"
#include "ShowVideoPanel.h"
#include <wx/wxprec.h>
#include <wx/splitter.h>
#include <wx/statline.h>
#include <wx/wrapsizer.h>
#include "UserDialog.h"
#include "ProjectsDialog.h"
#include "CreateProjectDlg.h"
#include "Client.h"
#include "ExportSeqDlg.h"
#include "ProjectsDialog.h"
#include "id.h"

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
    void SetProjectName(std::string name);
    void CreateStartDlg();
    void OnOpenProject(wxCommandEvent& event_);
    void OnSave(wxCommandEvent& event_);
    void OnPushProject(wxCommandEvent& event_);
    void OnPullProjects(wxCommandEvent& event_);
    void OnExportSequence(wxCommandEvent& event_);
    void OnNew(wxCommandEvent& event_);
    void OnImport(wxCommandEvent& event_);
    void OnUser(wxCommandEvent& event_);
    void OnNewSequence(wxCommandEvent& event_);
    void onTest(wxCommandEvent& event_);
    void onQuit(wxCommandEvent& event);
    void onClose(wxCloseEvent& event);
    void onHelp(wxCommandEvent& event);
    void OnLoadProjectFromFile(wxCommandEvent&);
    inline void OnRefresh(wxCommandEvent& event_);

    DECLARE_EVENT_TABLE();
    ServerClient* clientPtr;
private:
    std::vector<std::thread> importThreads;
    std::mutex statusBarMutex;
    wxDialog* startupDlg;
    wxStatusBar* statusBar;
    ShowVideoPanel* ogShowVideoPanel;
    ShowVideoPanel* finalVideoPanel;
    wxScrolledWindow* m_sourcesWindow;
    SeqControlWindow* m_sequenceControlWindow;
    wxWrapSizer* m_sourcesSizer;
    void ImportSourceVid(std::string sourcePath, bool load);
    void AddSequence(Sequence* seq);
};

