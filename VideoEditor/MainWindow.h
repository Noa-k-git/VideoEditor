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
 /// <summary>
 /// The class that represent the main window object of the app
 /// </summary>
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
    /// <summary>
    /// Setting the current project name
    /// </summary>
    /// <param name="name">project name</param>
    void SetProjectName(std::string name);
    /// <summary>
    /// Creates the start dialog
    /// </summary>
    void CreateStartDlg();
    /// <summary>
    /// Opens existing project
    /// </summary>
    /// <param name="event_">event</param>
    void OnOpenProject(wxCommandEvent& event_);
    /// <summary>
    /// Saving the changes made in the project
    /// </summary>
    /// <param name="event_">event</param>
    void OnSave(wxCommandEvent& event_);
    /// <summary>
    /// Saving and Pushing the project to the server
    /// </summary>
    /// <param name="event_">event</param>
    void OnPushProject(wxCommandEvent& event_);
    /// <summary>
    /// Showing the project's dialog
    /// </summary>
    /// <param name="event_">event</param>
    void OnPullProjects(wxCommandEvent& event_);
    /// <summary>
    /// Exporting the sequence to mp4 file
    /// </summary>
    /// <param name="event_"></param>
    void OnExportSequence(wxCommandEvent& event_);
    /// <summary>
    /// Creates new project
    /// </summary>
    /// <param name="event_"></param>
    void OnNew(wxCommandEvent& event_);
    /// <summary>
    /// Import new video
    /// </summary>
    /// <param name="event_">event</param>
    void OnImport(wxCommandEvent& event_);
    /// <summary>
    /// Showes the User dialog
    /// </summary>
    /// <param name="event_"></param>
    void OnUser(wxCommandEvent& event_);
    /// <summary>
    /// Creating new sequence
    /// </summary>
    /// <param name="event_">event</param>
    void OnNewSequence(wxCommandEvent& event_);
    //void onTest(wxCommandEvent& event_);
    //void onQuit(wxCommandEvent& event);
    //void onClose(wxCloseEvent& event);
    //void onHelp(wxCommandEvent& event);

    /// <summary>
    /// Loading a project from a file
    /// </summary>
    /// <param name="">event</param>
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

