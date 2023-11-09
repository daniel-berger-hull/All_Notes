#pragma once
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/dc.h>
#include <wx/menu.h>
#include <wx/textctrl.h>

#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include <wx/button.h>

#include <wx/gauge.h>


/*
    This is a basic example of wxWidgets application using a GUI Thread...
    it updates
    
    Aside adding the file (Main.h) and its cpp counter part (Main.cpp), and make sure to:
    i) Add the approriate Include and Lib configuration on the project 
       (Additionnal Include/Library Directories in the C/C++ and linker section of the property)
    ii) Set the Linker \System to Windows (not Console)

*/


#define THREAD_RUNNING      true
#define THREAD_STOPPED      false



class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    
    bool Cancelled();

    wxPanel*    panel;
    wxGauge*    gauge;
    wxButton*   startWorkerButton;
    wxMenuItem* startWorkerMenuItem;

    // was the worker thread cancelled by user?
    bool m_cancelled;
    bool m_running;
    wxCriticalSection m_csCancelled;        // protects m_cancelled
    

private:

    wxMenuBar* buildMenuBar();
    wxPanel*   buildMainPanel();

    void startWorker();

    void OnStartWorkerMenuItemClick(wxCommandEvent& event);
    void OnExitMenuItemClick(wxCommandEvent& event);
    void OnStartWorkerButtonClick(wxCommandEvent& WXUNUSED(event));
    
    void OnWorkerEvent(wxThreadEvent& event);

    void toggleControls(bool threadRunning);

  
    wxDECLARE_EVENT_TABLE();
};




