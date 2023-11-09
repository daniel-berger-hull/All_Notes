#include "Main.h"


enum
{
    TEXT_Main = wxID_HIGHEST + 1,
    ID_TestMenuItem,
    ID_TestButton

};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_TestMenuItem, MyFrame::OnMenuItemClick)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)

    EVT_BUTTON(ID_TestButton, MyFrame::OnButtonClick) // Tell the OS to run MainFrame::OnExit when
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}


wxPanel* MyFrame::buildMainPanel()
{
    wxPanel* panel = new wxPanel(this);



    wxStaticText* staticText1 = new wxStaticText(panel, wxID_ANY, "button1 clicked 0 times", { 50, 50 }, { 200, 20 });

    mainEditBox = new wxTextCtrl(panel, TEXT_Main, "Text!", { 250, 50 }, { 100, 20 },
        wxTE_MULTILINE | wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);

    testButton = new wxButton(panel, ID_TestButton, _T("Test Button!"), { 50, 100 }, wxDefaultSize, 0);


    return panel;
}


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{

    buildMenuBar();
    buildMainPanel();
    SetStatusText("Welcome to wxWidgets!");

}

wxMenuBar* MyFrame::buildMenuBar()
{
    wxMenu* menuFile = new wxMenu;

    menuFile->Append(ID_TestMenuItem, "&Test Menu!...\tCtrl-H", "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);
    CreateStatusBar();

    return menuBar;
}



void MyFrame::OnMenuItemClick(wxCommandEvent& event)
{
    wxLogMessage("Selected Menu Test Item!");
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets' Hello world sample",
        "About Hello World", wxOK | wxICON_INFORMATION);
}






void MyFrame::OnButtonClick(wxCommandEvent& event)
{
    wxLogMessage("Clicked on button!!");
}





