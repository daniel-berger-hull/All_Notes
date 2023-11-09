#pragma once
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/menu.h>
#include <wx/textctrl.h>

#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include <wx/button.h>


/*
    This is a basic example of wxWidgets application, with few minimal GUI controls,
    like Menu Bar, Push button, Editable Text and their events handlers.

    Aside adding the file (Main.h) and its cpp counter part (Main.cpp), and make sure to:
    i) Add the approriate Include and Lib configuration on the project 
       (Additionnal Include/Library Directories in the C/C++ and linker section of the property)
    ii) Set the Linker \System to Windows (not Console)

*/



class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    
    wxButton* testButton;
    wxTextCtrl* mainEditBox;
    

private:

    wxMenuBar* buildMenuBar();
    wxPanel*   buildMainPanel();

    void OnMenuItemClick(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnButtonClick(wxCommandEvent& event);


    wxDECLARE_EVENT_TABLE();
};




