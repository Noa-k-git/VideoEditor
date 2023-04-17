#include "MainWindow.h"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    //EVT_MENU(wxID_NEW, MainWindow::OnImport)
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

    wxSplitterWindow* mainSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_LIVE_UPDATE);
    mainSplitter->SetBackgroundColour(wxColor(200, 100, 100));

    wxSplitterWindow* topSplitter = new wxSplitterWindow(mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_BORDER | wxSP_LIVE_UPDATE);
    //topSplitter->SetBackgroundColour(wxColor(200, 100, 200));

    wxSplitterWindow* videoWindowSplitter= new wxSplitterWindow(topSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_BORDER | wxSP_LIVE_UPDATE);
    videoWindowSplitter->SetBackgroundColour(wxColor(100, 200, 100));

    wxSplitterWindow* bottomSplitter = new wxSplitterWindow(mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_BORDER | wxSP_LIVE_UPDATE);
    bottomSplitter->SetBackgroundColour(wxColor(100, 0, 100));
    
    wxPanel* sourcesPanel_ = new wxPanel(topSplitter, wxID_ANY);

    m_sourcesWindow = new wxScrolledWindow(sourcesPanel_, wxID_ANY, wxDefaultPosition);
    m_sourcesWindow->SetBackgroundColour(wxColor(230, 230, 100));

    wxBoxSizer* sourcesLayoutBoxSizer_ = new wxBoxSizer(wxVERTICAL);
    wxStaticText* sourcesTitle_ = new wxStaticText(sourcesPanel_, wxID_ANY, "Sources");
    sourcesTitle_->SetForegroundColour(wxColor(240, 240, 240));
    std::string iconColor = "white";
    wxBitmap addIcon(iconColor + (std::string)"Add.png", wxBITMAP_TYPE_PNG);
    SmallBitmapButton* importVideoButton = new SmallBitmapButton(sourcesPanel_, wxID_ANY, addIcon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW | wxBORDER_NONE);
    importVideoButton->SetToolTip(new wxToolTip("Import Video"));
    importVideoButton->Bind(wxEVT_BUTTON, &MainWindow::OnImport, this);

    sourcesLayoutBoxSizer_->Add(sourcesTitle_, 0, wxALL, 10);
    sourcesLayoutBoxSizer_->Add(m_sourcesWindow, 1, wxEXPAND);
    sourcesLayoutBoxSizer_->Add(importVideoButton, 0, wxEXPAND);
    sourcesPanel_->SetSizer(sourcesLayoutBoxSizer_);
    
    m_sourcesSizer = new wxWrapSizer(wxHORIZONTAL);
    m_sourcesWindow->SetSizer(m_sourcesSizer);
    m_sourcesWindow->SetScrollRate(FromDIP(5), FromDIP(5));
    // Setting the video panel for the final version
    ShowVideoPanel* finalVideoPanel = new ShowVideoPanel(videoWindowSplitter);
    ogShowVideoPanel = new ShowVideoPanel(videoWindowSplitter);

    finalVideoPanel->SetBackgroundColour(wxColor(50, 50, 80));


    wxPanel* fake = new wxPanel(bottomSplitter);
    fake->SetBackgroundColour(wxColor(5, 203, 32));
    wxPanel* fake1 = new wxPanel(bottomSplitter);
    fake1->SetBackgroundColour(wxColor(82, 34, 234));

    videoWindowSplitter->SplitVertically(ogShowVideoPanel, finalVideoPanel, 0.5);
    topSplitter->SplitVertically(sourcesPanel_, videoWindowSplitter, 0.5);
    bottomSplitter->SplitVertically(fake, fake1,0.5);
    mainSplitter->SplitHorizontally(topSplitter, bottomSplitter, 0.5);

    statusBar = CreateStatusBar();
    statusBar->SetStatusText(_("Ready!"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::onNew(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("MainWindow::onNew");
    PushStatusText(_("MainWinsow::onNew"));

    wxSleep(5);
    PopStatusText();
}

void MainWindow::OnImport(wxCommandEvent& WXUNUSED(event))
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
    auto v = new VideoSource(filePath);
    statusBar->PushStatusText(_("Processing Video: " + v->GetName()));
    wxMessageOutputDebug().Printf("Processing Video...");
    for (auto& thread : *ISource<std::vector<AVFrame*>>::readingThreads) {
        if (thread.joinable())
            thread.join();
    }
    auto vsPanel = new VideoSourcePanel(m_sourcesWindow, v, ogShowVideoPanel->GetId());
    //statusBar->SetStatusText(_("Finished"));

    m_sourcesSizer->Add(vsPanel, 1, wxALL, 10);

    m_sourcesSizer->Layout();
    statusBar->PopStatusText();
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

//void MainWindow::OnWindowSize(wxSizeEvent& event)
//{
//    event.Skip();
//    //this->SetSizerAndFit(mainSizer);
//    row1Sizer->Layout();
//}


