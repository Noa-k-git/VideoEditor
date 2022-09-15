#include "MainWindow.h"
#include <wx/artprov.h>
#include "test.xpm"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_NEW, MainWindow::onNew)
    EVT_MENU(wxID_EXIT, MainWindow::onQuit)
    EVT_TOOL(wxID_HELP, MainWindow::onHelp)
    //EVT_CLOSE(MainWindow::onClose)
END_EVENT_TABLE()

MainWindow::MainWindow(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name):
    wxFrame(parent, id,title, pos, size, style, name)
{
    wxMenuBar *menuBar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu();

    //wxWidgets default wxMenuItem example
    fileMenu->Append(wxID_NEW);

    //Custom wxMenuItem
    wxMenuItem * testItem = fileMenu->Append(wxID_ANY, _("&Test\tCtrl+T"));
    Bind(wxEVT_MENU, &MainWindow::onTest, this, testItem->GetId());


    //Create seperator
    fileMenu->AppendSeparator();

    //Submenu example
    /*
    wxMenu* subMenu = new wxMenu();
    subMenu->Append(wxID_CUT);
    subMenu->Append(wxID_COPY);
    subMenu->Append(wxID_PASTE);
    fileMenu->AppendSubMenu(subMenu, _("SubMenu"));

    fileMenu->AppendSeparator();
    */

    //Create wxMenuItem example
    wxMenuItem* quitItem = new wxMenuItem(fileMenu, wxID_EXIT);
    quitItem->SetBitmap(wxArtProvider::GetBitmap("wxART_QUIT"));
    fileMenu->Append(quitItem);
    menuBar->Append(fileMenu, _("&File"));
    SetMenuBar(menuBar);

    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::onClose, this);
    Unbind(wxEVT_CLOSE_WINDOW, &MainWindow::onClose, this);

    wxToolBar *toolBar = CreateToolBar();

    //Create new tool
    toolBar->AddTool(wxID_NEW, _("New"), wxArtProvider::GetBitmap("wxART_NEW"));
    
    //Create test tool
    wxBitmap testBmp(result);
    toolBar->AddTool(testItem->GetId(), _("Test"), testBmp);
    toolBar->SetToolBitmapSize(wxSize(16, 16));
    toolBar->AddSeparator();
    // Create quit tool
    toolBar->AddTool(wxID_EXIT, _("Quit"), wxArtProvider::GetBitmap("wxART_QUIT", wxART_OTHER, wxSize(16, 16)));

    toolBar->AddStretchableSpace();
    //Create help tool
    toolBar->AddTool(wxID_HELP, _("Help"), wxArtProvider::GetBitmap("wxART_HELP", wxART_OTHER, wxSize(16, 16)));
    toolBar->SetToolShortHelp(wxID_HELP, _("This is the help button"));
    toolBar->SetToolLongHelp(wxID_HELP, _("This is the help long text"));
    toolBar->Realize();


    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);


    wxPanel* panel = new wxPanel(this);
    mainSizer->Add(panel, 1, wxEXPAND);

    wxBoxSizer* panelMainSizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(panelMainSizer);

    wxBoxSizer* firstNameSizer = new wxBoxSizer(wxHORIZONTAL);

    //Create first name label
    wxStaticText* firstNameLabel = new wxStaticText(panel, wxID_ANY, _("First Name"));
    firstNameLabel->SetMinSize(wxSize(120, firstNameLabel->GetMinSize().y));
    firstNameSizer->Add(firstNameLabel);
    //Create first name box
    wxTextCtrl* firstNameBox = new wxTextCtrl(panel, wxID_ANY);
    // firstNameBox->SetMinSize(wxSize(100, 100));
    //mainSizer->Add(firstNameBox, 1, wxEXPAND | wxRIGHT | wxTOP, 10);
    //mainSizer->Add(firstNameBox, 0, wxALIGN_RIGHT | wxRIGHT | wxTOP, 10);
    //firstNameSizer->AddSpacer(20);
    firstNameSizer->Add(firstNameBox, 1);

    // create last name sizer
    wxBoxSizer* lastNameSizer = new wxBoxSizer(wxHORIZONTAL);

    //Create last name label
    wxStaticText* lastNameLabel = new wxStaticText(panel, wxID_ANY, _("Last Name"));
    lastNameLabel->SetMinSize(wxSize(120, lastNameLabel->GetMinSize().y));
    lastNameSizer->Add(lastNameLabel);
    //Create last name box
    wxTextCtrl* lastNameBox = new wxTextCtrl(panel, wxID_ANY);
    
    lastNameSizer->Add(lastNameBox, 1);

    panelMainSizer->Add(firstNameSizer, 0, wxEXPAND | wxALL, 10);
    panelMainSizer->Add(lastNameSizer, 0, wxEXPAND | wxBOTTOM | wxLEFT | wxRIGHT, 10);


    SetSizer(mainSizer);
    CreateStatusBar();

    SetMinSize(wxSize(400, 200));
    mainSizer->Fit(this);

    SetStatusText(_("Ready!"));

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

void MainWindow::onTest(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("MainWindow::onTest");
}

void MainWindow::onQuit(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("MainWindow::onQuit");
    bool veto = Close();
}

void MainWindow::onClose(wxCloseEvent& event)
{
    // wxMessageBox("MainWindow::onClose");

    if (event.CanVeto())
    {
        int answer = wxMessageBox(_("Exit?"), _("Confirm"), wxYES_NO);
        if (answer != wxYES)
        {
            event.Veto();
            return;
        }
    }
    Destroy();
}

void MainWindow::onHelp(wxCommandEvent& event)
{
    wxMessageBox("MainWimdow::onHelp");
}

