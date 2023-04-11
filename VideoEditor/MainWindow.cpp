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
    wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer * row1Sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer * row2Sizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(row1Sizer,1, wxEXPAND);
    mainSizer->Add(row2Sizer,1, wxEXPAND);


    // Setting the video panel for the final version
    wxPanel* finalVideoWindowPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition);
    finalVideoWindowPanel->SetBackgroundColour(wxColor(50, 50, 80));
    VideoWindow* finalVideoWindow = new VideoWindow(finalVideoWindowPanel, 0, 100);

    finalVideoWindow->timeline->slider->SetSize(wxSize(finalVideoWindow->main->GetSize().GetWidth(), -1));
    
    Bind(wxEVT_BUTTON, &MainWindow::OnImport, this, finalVideoWindow->pausePlay->GetId());

    // Setting the video panel for raw videos
    wxPanel* ogVideoWindowPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition);
    ogVideoWindowPanel->SetBackgroundColour(wxColor(80, 40, 80));
    VideoWindow* ogVideoWindow = new VideoWindow(ogVideoWindowPanel, 0, 100);

    ogVideoWindow->timeline->slider->SetSize(wxSize(ogVideoWindow->main->GetSize().GetWidth(), -1));
    
    // Configurations
    mainSizer->Layout();
    ogVideoWindow->timeline->handler->Layout();
    ogVideoWindow->main->Layout();
    finalVideoWindow->timeline->handler->Layout();
    finalVideoWindow->main->Layout();
    Bind(wxEVT_BUTTON, &MainWindow::OnImport, this, ogVideoWindow->pausePlay->GetId());
    

    row1Sizer->Add(ogVideoWindowPanel, 1, wxEXPAND | wxALL, 10);
    row1Sizer->Add(finalVideoWindowPanel, 1, wxEXPAND | wxALL, 10);
    //ogVideoWindowPanel->SetSizer(row1Sizer);
    //finalVideoWindowPanel->SetSizer(row1Sizer);
    
    this->SetSizerAndFit(mainSizer);
    row1Sizer->Layout();
    ogVideoWindowPanel->SetSizer(ogVideoWindow->main);
    finalVideoWindowPanel->SetSizer(finalVideoWindow->main);
    //this->Bind(wxEVT_SIZE, &MainWindow::OnWindowSize, this);

    //finalVideoWindowPanel->Fit();
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
    wxFileDialog fileDialog(this, "Open File", "", "", "*.*", wxFD_OPEN);
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

    wxMessageBox("Processing video");
    new VideoSource(filePath, "v");
    for (auto& thread : *ISource<std::vector<AVFrame*>>::readingThreads) {
        if (thread.joinable())
            thread.join();
    }
    //new VideoClip()
    new Sequence("a");
    Sequence::sequences.myMap.find("a")->second->AddClip(new VideoClip(VideoSource::videoSources.myMap.find("v")->second));
    std::string fname = "firstattempt.mp4";
    Sequence::sequences.myMap.find("a")->second->SaveVideo(fname);


}

//void MainWindow::OnWindowSize(wxSizeEvent& event)
//{
//    event.Skip();
//    //this->SetSizerAndFit(mainSizer);
//    row1Sizer->Layout();
//}


