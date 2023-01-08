#include "MainWindow.h"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    //EVT_MENU(wxID_NEW, MainWindow::onNew)
    //EVT_MENU(wxID_EXIT, MainWindow::onQuit)
    //EVT_TOOL(wxID_HELP, MainWindow::onHelp)
    //EVT_CLOSE(MainWindow::onClose)
END_EVENT_TABLE()


MainWindow::MainWindow(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :
    wxFrame(parent, id, title, pos, size, style, name) {

    int width = 1080, height = 1920;
  
    wxBitmap* frameBitmap = new wxBitmap(width, height);
    wxMemoryDC* dc;
    dc->SelectObject(frameBitmap);
    //dc->DrawBitmap(frameBitmap, &wxPoint(0, 0));
    dc->SetBrush(wxBrush(wxColour((0, 0, 0))));
    dc->DrawRectangle(0, 0, width, height);
    dc->SelectObject(wxNullBitmap);
    
    wxEvtHandler::Bind(wxEVT_PAINT, &MainWindow::OnPaint, this);
    //dc->Clear();

    wxStaticBitmap* frame = new wxStaticBitmap(this, wxID_ANY, *frameBitmap);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(frame);
    this->SetSizerAndFit(sizer);

}

MainWindow::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    dc.DrawBitmap(frameBitmap, 0, 0);
}
MainWindow::MainWindow(int a, wxWindow* parent,
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
    mainSizer->Add(row1Sizer);
    mainSizer->Add(row2Sizer);

    /*
    
    Video architecture

    ------------------------------|

    |---------------------------|
    |                           |
    |                           |
    |         video here        |
    |                           |
    |___________________________|

        <<   <   ||   >   >>

    ------------------------------|
    
    */

    wxBoxSizer* videoBase = new wxBoxSizer(wxVERTICAL);
    
    wxPanel* videoPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition);
    wxBoxSizer* videoCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
    videoPanel->SetBackgroundColour(wxColor(12, 20, 80));


    videoBase->Add(videoPanel, 1, wxEXPAND|wxBOTTOM, 30);
    videoBase->Add(videoCtrlSizer, 1, wxEXPAND|wxALL);
    /*
    wxButton* gotoStart = new wxButton(this, wxID_ANY, "<<", wxDefaultPosition, wxSize(wxDefaultSize.GetX(), wxDefaultSize.GetY()+ 50));
    wxButton* frameBefore = new wxButton(this, wxID_ANY, "<");
    wxButton* pausePlay = new wxButton(this, wxID_ANY, "| |");
    wxButton* frameAfter = new wxButton(this, wxID_ANY, ">");
    wxButton* gotoEnd = new wxButton(this, wxID_ANY, ">>");
    videoCtrlSizer->Add(gotoStart, 1, wxEXPAND|wxLEFT|wxRIGHT, 30);
    videoCtrlSizer->Add(frameBefore,1, wxEXPAND | wxLEFT|wxRIGHT, 30);
    videoCtrlSizer->Add(pausePlay, 2, wxEXPAND | wxLEFT|wxRIGHT, 30);
    videoCtrlSizer->Add(frameAfter, 1, wxEXPAND | wxLEFT|wxRIGHT, 30);
    videoCtrlSizer->Add(gotoEnd, 1, wxEXPAND | wxLEFT|wxRIGHT, 30);

    */

    VideoWindow* vw = new VideoWindow(this, 0, 1);
    videoCtrlSizer->Add(vw->main,1 ,wxEXPAND);

    //wxBoxSizer* ShowPanel = new wxPanel(this, wxID_ANY,)
    // App Architecture
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Panels
    wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    panel->SetBackgroundColour(wxColor(100, 0, 0));
    
    //wxPanel* videoPanel = new wxPanel(this, wxID_ANY);

    //wxBoxSizer* s = new wxBoxSizer(wxVERTICAL);
    //wxButton* b = new wxButton(this, wxID_ANY, "hello");
    //panel->AddChild(b);
    //s->Add(b);
    //panel->SetSizer(s);

    this->SetSizerAndFit(sizer);
    //sizer->Add(b);
    sizer->Add(panel, 0, wxEXPAND | wxALL, 5);
    sizer->Add(videoBase, 1, wxEXPAND|wxALL, 10);
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

}


