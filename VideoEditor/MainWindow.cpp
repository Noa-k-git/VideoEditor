#include "MainWindow.h"
#include "DesignConstatns.h"
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_SAVE, MainWindow::OnSave)
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
   
    /* 
    
    Base architecture
    
    --------------------------------|
    horizontal  |     row   |  1    |
    --------------------------------|
    horizontal  |     row   |  2    |
    --------------------------------|
    
    */
    SetBackgroundColour(WINDOW_BRIGHT_BACKGOUND_COLOUR);
    wxBoxSizer* layoutSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* menuSizer = new wxBoxSizer(wxHORIZONTAL);
    layoutSizer->SetDimension(GetSize().x, GetSize().y, GetSize().GetWidth(), GetSize().GetHeight());
    layoutSizer->Add(menuSizer, 0, wxEXPAND);

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

    m_sequenceControlWindow = new SeqControlWindow(m_sequenceWindow, seqNameText_);
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
    wxAcceleratorEntry entries[1];
    entries[0].Set(wxACCEL_CTRL, (int)'S', wxID_SAVE);
    wxAcceleratorTable accel(1, entries);
    SetAcceleratorTable(accel);
    SetSizer(layoutSizer);

    clientPtr = new ServerClient();
}

MainWindow::~MainWindow()
{
}

void MainWindow::onNew(wxCommandEvent& WXUNUSED(event_))
{
    wxMessageBox("MainWindow::onNew");
    PushStatusText(_("MainWinsow::onNew"));

    wxSleep(5);
    PopStatusText();
}

void MainWindow::OnImport(wxCommandEvent& WXUNUSED(event_))
{
    //wxMessageBox("Start Import");
    wxFileDialog fileDialog(this, "Open File", "", "", "*.mp4", wxFD_OPEN);
    std::string filePath;
    if (fileDialog.ShowModal() == wxID_OK)
    {
        filePath = fileDialog.GetPath().ToStdString();
    }
    else
        return;
    
    //VideoSource* vs = new VideoSource("C:\\Users\\cyber\\source\\repos\\Noa-k-git\\VideoEditor\\video.mp4");
    //wxMessageBox("Processing video");
    //VideoSource* vs = new VideoSource(filePath);
    //for (auto& thread : *ISource<std::vector<AVFrame*>>::readingThreads) {
    //    if (thread.joinable())
    //        thread.join();
    //}
    //VideoClip* vc = new VideoClip(vs);
    //
    //Sequence* s = new Sequence();
    //s->AddClip(vc);
    //std::string fname = "withhelp.mp4";
    //s->SaveVideo(fname);
    //delete s;
    ////delete vc;
    //delete vs;
    //wxMessageBox("Processing video");
    //auto processVideoLambda = 
    std::thread t([this, filePath]() {
        wxGetApp().CallAfter([this]() {
            std::lock_guard<std::mutex> lock(statusBarMutex);
            for (int i = 0; i < statusBar->GetFieldsCount(); i++)
            {
                wxString text = statusBar->GetStatusText(i);
                wxMessageOutputDebug().Printf("\t+\tb Status bar field %d: %s", i, text);
                wxMessageOutputDebug().Printf("%d", statusBar->GetFieldsCount());
            }
            statusBar->PushStatusText(_("Processing Video ") + std::to_string(VideoSource::videoSources.GetRecords()->size()) + _("0..."));
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
        VideoSource* v = new VideoSource(filePath);
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
                m_sourcesSizer->Layout();
                });
        }
        else {
            delete v;
            wxGetApp().CallAfter([&]() {
                wxMessageBox(_("Video is already defined... If not try first to change the video name"), _("Error in importing video"));
                });
        }
        });

    t.detach();

    //for (auto vs : *VideoSource::videoSources.GetRecords()) {
    //    auto x = new VideoSourcePanel(sourcesPanel, vs);
    //    m_sourcesSizer->Add(x);
    //}
    //new VideoClip()
    //new Sequence("a");
    //(*Sequence::sequences.Contains("a").first)->AddClip(new VideoClip(*VideoSource::videoSources.Contains("v").first));
    //std::string fname = "RecordVideo.mp4";
    //(*Sequence::sequences.Contains("a").first)->SaveVideo(fname);
}

void MainWindow::OnUser(wxCommandEvent& event_)
{
    UserDialog* dialog = new UserDialog(clientPtr, this);
    dialog->ShowModal();
}

void MainWindow::OnNewSequence(wxCommandEvent& WXUNUSED(event_))
{
    Sequence* s = new Sequence();
    if (s->GetCreated()) {
        wxGetApp().CallAfter([this, s]() {
            auto vsPanel = new SeqSourcePanel(m_sourcesWindow, s, finalVideoPanel->GetId(), m_sequenceControlWindow->GetId());
            //statusBar->SetStatusText(_("Finished"));
            m_sourcesSizer->Add(vsPanel, 1, wxALL, 10);
            m_sourcesSizer->Layout();
            });
    }
    else {
        delete s;
    }

}

void MainWindow::OnSave(wxCommandEvent& event_)
{
}

void MainWindow::OnRefresh(wxCommandEvent& event_)
{
    this->m_sourcesWindow->Layout();
}

//void MainWindow::OnWindowSize(wxSizeEvent& event)
//{
//    event.Skip();
//    //this->SetSizerAndFit(mainSizer);
//    row1Sizer->Layout();
//}