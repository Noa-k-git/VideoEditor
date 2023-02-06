#include "MainWindow.h"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    //EVT_MENU(wxID_NEW, MainWindow::OnImport)
    //EVT_MENU(wxID_NEW, MainWindow::onNew)
    //EVT_MENU(wxID_EXIT, MainWindow::onQuit)
    //EVT_TOOL(wxID_HELP, MainWindow::onHelp)
    //EVT_CLOSE(MainWindow::onClose)
END_EVENT_TABLE()


MainWindow::MainWindow(int, wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :
    wxFrame(parent, id, title, pos, size, style, name) {



}

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
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* row1Sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* row2Sizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(row1Sizer,1, wxEXPAND);
    mainSizer->Add(row2Sizer,1, wxEXPAND);


    
    wxPanel* videoPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition);
    videoPanel->SetBackgroundColour(wxColor(50, 50, 80));
    VideoWindow* vw = new VideoWindow(videoPanel, 0, 100);
    videoPanel->SetSizer(vw->main);
    
    this->SetSizerAndFit(mainSizer);
    videoPanel->Fit();
    //sizer->Add(b);
    row1Sizer->Add(videoPanel, 1, wxEXPAND | wxALL, 10);

    wxSize* n = new wxSize(vw->timeline->slider->GetMaxWidth(), -1);
    vw->timeline->slider->SetSize(wxSize(vw->main->GetSize().GetWidth(), -1));
    mainSizer->Layout();
    vw->timeline->handler->Layout();
    vw->main->Layout();
    //wxMessageBox(std::to_string(vw->handlerSizer->GetItem(1)->GetId()));
    //wxMessageBox(std::to_string(vw->handlerSizer->GetItem(1)->IsKindOf(CLASSINFO(wxButton))));
    //Bind(wxEVT_BUTTON, &MainWindow::OnImport, this, vw->handlerSizer->GetItem(2)->GetId());
    wxMessageBox(std::to_string(vw->pausePlay->IsKindOf(CLASSINFO(wxButton))));
    Bind(wxEVT_BUTTON, &MainWindow::OnImport, this, vw->pausePlay->GetId());

    //VideoSource* vd = new VideoSource("D:\\Downloads\\20190612_195422.mp4");
    //vd->Show();
    //delete vd;
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
    wxMessageBox("Start Import");
    wxFileDialog fileDialog(this, "Open File", "", "", "*.mp4", wxFD_OPEN);
    std::string filePath;
    if (fileDialog.ShowModal() == wxID_OK)
    {
        filePath = fileDialog.GetPath().ToStdString();
    }
    else
        return;
    //VideoSource* vs = new VideoSource("C:\\Users\\cyber\\source\\repos\\Noa-k-git\\VideoEditor\\video.mp4");
    wxMessageBox("Processing video");
    VideoSource* vs = new VideoSource(filePath);

}


