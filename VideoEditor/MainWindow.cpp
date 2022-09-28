#include "MainWindow.h"
#include <wx/artprov.h>
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
    wxFrame(parent, id, title, pos, size, style, name)
{
    // App Architecture
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Panels
    wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    panel->SetBackgroundColour(wxColor(100, 100, 200));
    wxPanel* videoPanel = new wxPanel(this, wxID_ANY);

    sizer->Add(panel, 1, wxEXPAND | wxALL, 5);
    this->SetSizerAndFit(sizer);

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


