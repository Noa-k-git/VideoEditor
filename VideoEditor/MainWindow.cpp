#include <fstream>
#include "MainWindow.h"
#include "DesignConstatns.h"
#include "string_utils.h"
using namespace string_utils;


BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_SAVE, MainWindow::OnSave)
    EVT_MENU(wxID_OPEN, MainWindow::OnOpenProject)
    EVT_MENU(wxID_NEW, MainWindow::OnOpenProject)
    
    //EVT_MENU(wxID_NEW, MainWindow::onNew)
    //EVT_MENU(wxID_EXIT, MainWindow::onQuit)
    //EVT_TOOL(wxID_HELP, MainWindow::onHelp)
    //EVT_CLOSE(MainWindow::onClose)
END_EVENT_TABLE()


//MainWindow::MainWindow(int, wxWindow* parent,
//    wxWindowID id,
//    const wxString& title,
//    const wxPoint& pos,
//    const wxSize& size,
//    long style,
//    const wxString& name) :
//    wxFrame(parent, id, title, pos, size, style, name) {}


MainWindow::MainWindow(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :
    wxFrame(parent, id, title, pos, size, style, name)
{   
    Bind(SAVE_PROJECT_EVENT, &MainWindow::OnSave, this);
    Bind(LOAD_PROJECT_EVENT, &MainWindow::OnLoadProjectFromFile, this);

   
    /* 
    
    Base architecture
    
    --------------------------------|
    horizontal  |     row   |  1    |
    --------------------------------|
    horizontal  |     row   |  2    |
    --------------------------------|
    
    */
    clientPtr = new ServerClient();
    SetBackgroundColour(WINDOW_BRIGHT_BACKGOUND_COLOUR);
    wxBoxSizer* layoutSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* menuSizer = new wxBoxSizer(wxHORIZONTAL);
    layoutSizer->SetDimension(GetSize().x, GetSize().y, GetSize().GetWidth(), GetSize().GetHeight());
    layoutSizer->Add(menuSizer, 0, wxEXPAND);

    wxButton* pushProjBtn = new wxButton(this, wxID_ANY, "PUSH PROJECT");
    pushProjBtn->Bind(wxEVT_BUTTON, &MainWindow::OnPushProject, this);
    menuSizer->Add(pushProjBtn, 0.1, wxEXPAND | wxALL, 5);


    wxButton* pullProjectsBtn = new wxButton(this, wxID_ANY, "PULL My Projects");
    pullProjectsBtn->Bind(wxEVT_BUTTON, &MainWindow::OnPullProjects, this);
    menuSizer->Add(pullProjectsBtn, 0.1, wxEXPAND|wxALL, 5);
    
    wxButton* exportBtn = new wxButton(this, wxID_ANY, "Export");
    exportBtn->Bind(wxEVT_BUTTON, &MainWindow::OnExportSequence, this);
    wxBoxSizer* exportSizer = new wxBoxSizer(wxVERTICAL);
    exportSizer->Add(exportBtn, 1, wxALIGN_RIGHT | wxRIGHT, 10);
    menuSizer->Add(exportSizer, 1, wxEXPAND|wxALL, 5);

    wxBitmap userIcon((std::string)"user4.png", wxBITMAP_TYPE_PNG);
    SmallBitmapButton* loginButton = new SmallBitmapButton(this, wxID_ANY, userIcon, wxDefaultPosition, wxSize(32, 32), wxSize(26, 26), wxBU_AUTODRAW);
    loginButton->Bind(wxEVT_BUTTON, &MainWindow::OnUser, this);
    loginButton->SetForegroundColour(WINDOW_BRIGHT_FOREGROUND_COLOUR);
    loginButton->SetBackgroundColour(WINDOW_BRIGHT_FOREGROUND_COLOUR);
    loginButton->SetToolTip(new wxToolTip("User"));
    wxBoxSizer* loginSizer = new wxBoxSizer(wxVERTICAL);
    loginSizer->Add(loginButton, 0, wxALIGN_RIGHT|wxRIGHT, 10);
    menuSizer->Add(loginSizer , 1, wxALL, 5);
    menuSizer->Layout();
    wxSplitterWindow* mainSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_LIVE_UPDATE);
    //mainSplitter->SetBackgroundColour(wxColor(200, 100, 100));
    mainSplitter->SetBackgroundColour(WINDOW_BACKGOUND_COLOUR);
    layoutSizer->Add(mainSplitter, 1, wxEXPAND);
    layoutSizer->Layout();
    // add buttons to the menu

    wxSplitterWindow* topSplitter = new wxSplitterWindow(mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_BORDER | wxSP_LIVE_UPDATE);
    //topSplitter->SetBackgroundColour(wxColor(200, 100, 200));

    wxSplitterWindow* videoWindowSplitter= new wxSplitterWindow(topSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_BORDER | wxSP_LIVE_UPDATE);
    //videoWindowSplitter->SetBackgroundColour(wxColor(100, 200, 100));
    videoWindowSplitter->SetBackgroundColour(WINDOW_BACKGOUND_COLOUR);

    wxSplitterWindow* bottomSplitter = new wxSplitterWindow(mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_BORDER | wxSP_LIVE_UPDATE);
    //bottomSplitter->SetBackgroundColour(wxColor(100, 0, 100));
    bottomSplitter->SetBackgroundColour(WINDOW_BACKGOUND_COLOUR);
    
    wxPanel* sourcesPanel_ = new wxPanel(topSplitter, wxID_ANY);

    m_sourcesWindow = new wxScrolledWindow(sourcesPanel_, wxID_ANY, wxDefaultPosition);
    //m_sourcesWindow->SetBackgroundColour(wxColor(230, 230, 100));
    m_sourcesWindow->SetBackgroundColour(WINDOW_BACKGOUND_COLOUR);

    wxBoxSizer* sourcesLayoutBoxSizer_ = new wxBoxSizer(wxVERTICAL);
    wxStaticText* sourcesTitle_ = new wxStaticText(sourcesPanel_, wxID_ANY, "Sources");
    //sourcesTitle_->SetForegroundColour(wxColor(240, 240, 240));
    std::string iconColor = "white";
    wxBitmap addIcon(iconColor + (std::string)"Add.png", wxBITMAP_TYPE_PNG);
    SmallBitmapButton* newObjectButton = new SmallBitmapButton(sourcesPanel_, wxID_ANY, addIcon, wxDefaultPosition, BTN_SIZE, BTN_ICON_SIZE, wxBU_AUTODRAW | wxBORDER_NONE);
    newObjectButton->SetToolTip(new wxToolTip("Append Object.."));

    wxMenu* addMenu = new wxMenu();
    wxMenuItem* importItem_ = new wxMenuItem(addMenu, wxID_ANY, "Import Video");
    Connect(importItem_->GetId(), wxEVT_MENU, wxCommandEventHandler(MainWindow::OnImport));
    addMenu->Append(importItem_);
    wxMenuItem* newSeqItem_ = new wxMenuItem(addMenu, wxID_ANY, "New Sequence");
    Connect(newSeqItem_->GetId(), wxEVT_MENU, wxCommandEventHandler(MainWindow::OnNewSequence));
    addMenu->Append(newSeqItem_);
    newObjectButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {
        // show the menu
        m_sourcesWindow->Refresh();
        PopupMenu(addMenu);
        });

    sourcesLayoutBoxSizer_->Add(sourcesTitle_, 0, wxALL, 10);
    sourcesLayoutBoxSizer_->Add(m_sourcesWindow, 1, wxEXPAND);
    sourcesLayoutBoxSizer_->Add(newObjectButton, 0, wxEXPAND);
    sourcesPanel_->SetSizer(sourcesLayoutBoxSizer_);
    
    m_sourcesSizer = new wxWrapSizer(wxHORIZONTAL);
    m_sourcesWindow->SetSizer(m_sourcesSizer);
    m_sourcesWindow->SetScrollRate(FromDIP(5), FromDIP(5));
    // Setting the video panel for the final version
    finalVideoPanel = new ShowVideoPanel(videoWindowSplitter);
    ogShowVideoPanel = new ShowVideoPanel(videoWindowSplitter);

    //finalVideoPanel->SetBackgroundColour(wxColor(50, 50, 80));
    finalVideoPanel->SetBackgroundColour(WINDOW_BACKGOUND_COLOUR);


    wxPanel* m_sequenceWindow = new wxPanel(bottomSplitter);
    m_sequenceWindow->SetBackgroundColour(WINDOW_BACKGOUND_COLOUR);
    wxPanel* effectWindow = new wxPanel(bottomSplitter);
    //effectWindow->SetBackgroundColour(wxColor(82, 34, 234));
    m_sequenceWindow->SetBackgroundColour(WINDOW_BACKGOUND_COLOUR);
    //m_sequenceWindow->SetBackgroundColour(wxColor(5, 203, 32));

    wxBoxSizer* sequenceWindowSizer_ = new wxBoxSizer(wxVERTICAL);
    m_sequenceWindow->SetSizer(sequenceWindowSizer_);
    
    wxStaticText* seqNameText_ = new wxStaticText(m_sequenceWindow, wxID_ANY, "Sequence here...");
    seqNameText_->SetFont(seqNameText_->GetFont().Italic());
    sequenceWindowSizer_->Add(seqNameText_, 0, wxALL, 10);
    wxMessageOutputDebug().Printf("\t\t\t%d", m_sequenceWindow->GetId());
    wxMessageOutputDebug().Printf("\t\t\t%d", seqNameText_->GetId());

    m_sequenceControlWindow = new SeqControlWindow(clientPtr, m_sequenceWindow, seqNameText_);
    sequenceWindowSizer_->Add(m_sequenceControlWindow, 1, wxEXPAND|wxALL, 10);

    sequenceWindowSizer_->Layout();
    
    Layout();
    videoWindowSplitter->SplitVertically(ogShowVideoPanel, finalVideoPanel, 0.5);
    videoWindowSplitter->SetSashGravity(1);
    topSplitter->SplitVertically(sourcesPanel_, videoWindowSplitter, 0.5);
    bottomSplitter->SplitVertically(effectWindow, m_sequenceWindow, 0.5);
    mainSplitter->SplitHorizontally(topSplitter, bottomSplitter, 0.5);
    
    videoWindowSplitter->SetMinimumPaneSize(50);
    topSplitter->SetMinimumPaneSize(50);
    bottomSplitter->SetMinimumPaneSize(50);
    mainSplitter->SetMinimumPaneSize(50);
    Layout();
    //int a = (int)mainSplitter->GetClientSize().GetHeight() * 0.5;
    //mainSplitter->SetSashPosition((int)mainSplitter->GetClientSize().GetHeight() * 0.5);
    //statusBar = CreateStatusBar();
    statusBar = new wxStatusBar(this, wxID_ANY, 0);
    SetStatusBar(statusBar);
    statusBar->PushStatusText(_("Ready!"));
    
    this->Bind(WIDGET_DELETED_EVT, &MainWindow::OnRefresh, this);
    wxAcceleratorEntry entries[3];
    entries[0].Set(wxACCEL_CTRL, (int)'S', wxID_SAVE);
    entries[1].Set(wxACCEL_CTRL, (int)'O', wxID_OPEN);
    entries[2].Set(wxACCEL_CTRL, (int)'N', wxID_NEW);
    wxAcceleratorTable accel(3, entries);
    SetAcceleratorTable(accel);
    SetSizer(layoutSizer);

    clientPtr->SetPath("");
    SetProjectName("Project.vprojn");
    Show();
    CreateStartDlg();
    while (clientPtr->GetPath() == "")
    {
        startupDlg->ShowModal();
    }
}

MainWindow::~MainWindow()
{
    ogShowVideoPanel->PauseVideo();
    finalVideoPanel->PauseVideo();
    for (int i = 0; i < importThreads.size(); i++)
        importThreads.at(i).join();
}

void MainWindow::SetProjectName(std::string name)
{
    clientPtr->SetPrName(name);
    wxGetApp().CallAfter([=]() {
        SetTitle(SplitString(name, '.').at(0));
        });
}

void MainWindow::CreateStartDlg()
{
    startupDlg = new wxDialog(this, wxID_ANY, "Startup Menu");
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->SetMinSize(600, 400);

    wxFont font(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_LIGHT);
    wxButton* newProjectBtn = new wxButton(startupDlg, wxID_ANY, "Create New Project");
    newProjectBtn->SetFont(font);
    newProjectBtn->SetMinSize(wxSize(-1, 50));
    newProjectBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event_) {
        OnNew(event_);
        startupDlg->Close(); 
        });

    mainSizer->Add(newProjectBtn, 1, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 30);
    mainSizer->AddSpacer(30);

    wxButton* openProjBtn = new wxButton(startupDlg, wxID_ANY, "Open Existing Project");
    openProjBtn->SetFont(font);
    openProjBtn->SetMinSize(wxSize(-1, 50));
    openProjBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event_) {
        OnOpenProject(event_);
        startupDlg->Close();
    });

    mainSizer->Add(openProjBtn, 1, wxEXPAND | wxRIGHT|wxLEFT|wxBOTTOM, 30);


    startupDlg->SetSizerAndFit(mainSizer);
    wxPoint parentCenter((int)((this->GetPosition().x + this ->GetSize().GetWidth()) / 2),
        (int)((this->GetPosition().y + this->GetSize().GetHeight()) / 2));
    wxPoint thisCenter((int)(startupDlg->GetSize().GetWidth() / 2), (int)(startupDlg->GetSize().GetHeight() / 2));
    startupDlg->SetPosition(wxPoint(parentCenter.x - thisCenter.x, parentCenter.y - thisCenter.y));
    if (startupDlg->GetPosition().x < 0 || startupDlg->GetPosition().y < 0)
        startupDlg->SetPosition(wxDefaultPosition);

}

void MainWindow::OnOpenProject(wxCommandEvent& event_)
{
    wxFileDialog fileDialog(this, "Open Project", "", "", "*.vprojn", wxFD_OPEN| wxFD_FILE_MUST_EXIST);
    if (fileDialog.ShowModal() == wxID_OK)
    {
        std::vector<std::string> p = SplitString(fileDialog.GetPath().ToStdString(), '\\');
        SetProjectName(p.at(p.size() - 1));
        p.pop_back();
        clientPtr->SetPath(JoinString(p, '\\') + '\\');
    }
    else
        return;

    OnLoadProjectFromFile(event_);
}

void MainWindow::OnSave(wxCommandEvent& event_)
{
    if (clientPtr->GetPath() == "")
    {
        wxFileDialog fileDialog(this, "Save Project", "", clientPtr->GetPrName(), "*.vprojn", wxFD_SAVE);
        if (fileDialog.ShowModal() == wxID_OK)
        {
            std::vector<std::string> p = SplitString(fileDialog.GetPath().ToStdString(), '\\');
            SetProjectName(p.at(p.size() - 1));
            p.pop_back();
            clientPtr->SetPath(JoinString(p, '\\') + '\\');
        }
        else
            return;
    }
    std::ofstream file(clientPtr->GetPath() + clientPtr->GetPrName());
    file << clientPtr->GetPrName() << std::endl;
    std::vector<VideoSource*>& vSources = *VideoSource::videoSources.GetRecords();
    int x = vSources.size() - 1;
    bool a = 0 < x;
    for (int i = 0; i < x; i++) {
        file << vSources.at(i)->Write(clientPtr->GetPath()) << '|';
    }
    if (x >= 0)
        file << vSources.at(vSources.size() - 1)->Write(clientPtr->GetPath()) << std::endl;

    std::vector<Sequence*>& allSeq = *Sequence::sequences.GetRecords();
    x = allSeq.size() - 1;
    for (int i = 0; i < x; i++) {
        file << allSeq.at(i)->Write() << '|';
    }
    if (x >= 0)
        file << allSeq.at(allSeq.size() - 1)->Write() << std::endl;
    file.close();
}

void MainWindow::OnPushProject(wxCommandEvent& event_)
{
    OnSave(event_);
    if (clientPtr->GetProjId() == "-1")
        wxMessageBox("ERROR", "Project is not Connected", wxICON_ERROR);
    else
        clientPtr->PushProject();
}

void MainWindow::OnPullProjects(wxCommandEvent& event_)
{
    ProjectDialog* dlg = new ProjectDialog(clientPtr, this);
    wxButton* button = dynamic_cast<wxButton*>(event_.GetEventObject());
    if (button) {
        dlg->SetPosition(wxPoint(button->GetPosition().x, button->GetPosition().y + button->GetSize().GetHeight()+20));
    }
    dlg->ShowModal();
}

void MainWindow::OnExportSequence(wxCommandEvent& event_)
{
    ExportSeqDlg dlg(clientPtr, this);
    dlg.ShowModal();
}

void MainWindow::OnNew(wxCommandEvent& event_)
{
    OnSave(event_);
}

void MainWindow::OnImport(wxCommandEvent& WXUNUSED(event_))
{
    wxFileDialog fileDialog(this, "Open Source File", clientPtr->GetPath(), "", "*.mp4", wxFD_OPEN);
    std::string filePath;
    if (fileDialog.ShowModal() == wxID_OK)
    {
        filePath = fileDialog.GetPath().ToStdString();
    }
    else
        return;
    ImportSourceVid(FindDifference(clientPtr->GetPath(), filePath), false);
    m_sourcesSizer->FitInside(m_sourcesWindow);

}

void MainWindow::OnUser(wxCommandEvent& event_)
{
    UserDialog* dialog = new UserDialog(clientPtr, this);
    dialog->ShowModal();
}

void MainWindow::OnNewSequence(wxCommandEvent& WXUNUSED(event_))
{
    Sequence* s = new Sequence();
    AddSequence(s);
    m_sourcesSizer->FitInside(m_sourcesWindow);

}

void MainWindow::OnLoadProjectFromFile(wxCommandEvent& WXUNUSED(event_))
{
    Sequence::sequences.RemoveAllRecords();
    VideoSource::videoSources.RemoveAllRecords();
    m_sourcesWindow->DestroyChildren();
    m_sequenceControlWindow->DestroyChildren();

    Layout();

    std::ifstream file(clientPtr->GetPath() + clientPtr->GetPrName());
    std::string data;

    // read the file line by line
    if (std::getline(file, data)) {
        SetProjectName(data);
    }
    else {
        return;
    }
    if (std::getline(file, data)) {
        std::vector<std::string> vsElems = SplitString(data, '|');
        for (std::string& vsData : vsElems) {
            ImportSourceVid(vsData, true);
        }
        for (std::thread& t : importThreads) { t.join(); }
        importThreads.clear();
    }
    else {
        file.close();
        return;
    }

    if (std::getline(file, data)) {
        std::vector<std::string> seqElms = SplitString(data, '|');
        for (std::string& seqData : seqElms) {
            Sequence* s = new Sequence(seqData, true);
            AddSequence(s);
        }
    }

    file.close();
    m_sourcesSizer->FitInside(m_sourcesWindow);
    Layout();
}

void MainWindow::OnRefresh(wxCommandEvent& event_)
{
    this->m_sourcesWindow->Layout();
}

void MainWindow::ImportSourceVid(std::string sourcePath, bool load)
{
    if (sourcePath != "") {
        std::thread t([=]() {
            wxGetApp().CallAfter([=]() {
                std::lock_guard<std::mutex> lock(statusBarMutex);
                for (int i = 0; i < statusBar->GetFieldsCount(); i++)
                {
                    wxString text = statusBar->GetStatusText(i);
                    wxMessageOutputDebug().Printf("\t+\tb Status bar field %d: %s", i, text);
                    wxMessageOutputDebug().Printf("%d", statusBar->GetFieldsCount());
                }
                statusBar->PushStatusText(_("Processing Video...")); // +std::to_string(VideoSource::videoSources.GetRecords()->size()) + _("0..."));
                //statusBar->PushStatusText(_("Processing Video ") + std::to_string(VideoSource::videoSources.GetRecords()->size()) + _("1..."));
                //statusBar->PushStatusText(_("Processing Video ") + std::to_string(VideoSource::videoSources.GetRecords()->size()) + _("2..."));
                for (int i = 0; i < statusBar->GetFieldsCount(); i++)
                {
                    wxString text = statusBar->GetStatusText(i);
                    wxMessageOutputDebug().Printf("%d", statusBar->GetFieldsCount());
                    wxMessageOutputDebug().Printf("\t+\ta Status bar field %d: %s", i, text);
                }
                wxMessageOutputDebug().Printf("Processing Video: %d", statusBar->GetFieldsCount());
                });
            VideoSource* v = new VideoSource(clientPtr->GetPath(), sourcePath, load);
            wxGetApp().CallAfter([this]() {
                for (int i = 0; i < statusBar->GetFieldsCount(); i++)
                {
                    wxString text = statusBar->GetStatusText(i);
                    wxMessageOutputDebug().Printf("\t-\tStatus bar field %d: %s", i, text);
                }
                std::lock_guard<std::mutex> lock(statusBarMutex);
                statusBar->PopStatusText();
                wxMessageOutputDebug().Printf("Remove Processing Video: %d", statusBar->GetFieldsCount());

                //statusBar->PopStatusText();
                });
            if (v->GetCreated())
            {

                /*for (auto& thread : *ISource<std::vector<SyncObject<AVFrame*>>>::readingThreads) {
                    if (thread.joinable())
                    {
                        thread.join();
                    }
                }*/
                wxGetApp().CallAfter([this, v]() {
                    auto vsPanel = new VideoSourcePanel(m_sourcesWindow, v, ogShowVideoPanel->GetId(), m_sequenceControlWindow->GetId());
                    //statusBar->SetStatusText(_("Finished"));
                    m_sourcesSizer->Add(vsPanel, 1, wxALL, 10);
                    m_sourcesSizer->FitInside(m_sourcesWindow);
                    m_sourcesSizer->Layout();
                    });
            }
            else {
                if (v->GetError())
                {
                    VideoSource::videoSources.RemoveRecord(v->GetName());
                    wxGetApp().CallAfter([&]() {
                        wxMessageBox(_("ERROR IN IMPORTING VIDEO"), _("Error in importing video"), wxICON_ERROR);
                        });
                }
                else {
                    delete v;
                wxGetApp().CallAfter([&]() {
                    wxMessageBox(_("Video is already defined... If not try first to change the video name"), _("Error in importing video"));
                    });
                }
            }
            });
        importThreads.push_back(std::move(t));
    }
}

void MainWindow::AddSequence(Sequence* seq)
{
    if (seq->GetCreated()) {
        wxGetApp().CallAfter([this, seq]() {
            auto vsPanel = new SeqSourcePanel(m_sourcesWindow, seq, finalVideoPanel->GetId(), m_sequenceControlWindow->GetId());
            //statusBar->SetStatusText(_("Finished"));
            m_sourcesSizer->Add(vsPanel, 1, wxALL, 10);
            m_sourcesSizer->Layout();
            });
    }
    else {
        delete seq;
    }
}

//void MainWindow::OnWindowSize(wxSizeEvent& event)
//{
//    event.Skip();
//    //this->SetSizerAndFit(mainSizer);
//    row1Sizer->Layout();
//}