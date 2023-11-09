#include "Main.h"
#include "MyWorkerThread.h"
#include "Events.h"




wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_StartWorkerMenuItem, MyFrame::OnStartWorkerMenuItemClick)
    EVT_MENU(wxID_EXIT, MyFrame::OnExitMenuItemClick)

    EVT_BUTTON(ID_StartWorkerButton, MyFrame::OnStartWorkerButtonClick) // Tell the OS to run MainFrame::OnExit when

    EVT_THREAD(ID_WORKER_EVENT, MyFrame::OnWorkerEvent)

wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(300, 175));
    frame->Show(true);
    return true;
}


wxPanel* MyFrame::buildMainPanel()
{
    panel = new wxPanel(this);

    wxStaticText* staticText1 = new wxStaticText(panel, wxID_ANY, "A worker thread will update the progress bar", { 20, 20 }, { 270, 20 });



    gauge = new wxGauge(panel, ID_TestGauge,100, { 20, 50 }, { 150, 20 }, wxGA_HORIZONTAL);

    startWorkerButton = new wxButton(panel, ID_StartWorkerButton, _T("Start!"), { 185, 45 }, wxDefaultSize, 0);

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


    startWorkerMenuItem = menuFile->Append(ID_StartWorkerMenuItem, "&Start Worker!...\tCtrl-H", "Will spawn a worker thread to update the progress bar");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");

    SetMenuBar(menuBar);
    CreateStatusBar();

    return menuBar;
}



/************************************************************************
                      Section of the Events Handlers 
*************************************************************************/


void MyFrame::OnStartWorkerMenuItemClick(wxCommandEvent& event)
{
    startWorker();
    startWorker();
    toggleControls(THREAD_RUNNING);
}

void MyFrame::OnExitMenuItemClick(wxCommandEvent& event)
{
    Close(true);
}


void MyFrame::OnStartWorkerButtonClick(wxCommandEvent& WXUNUSED(event))
{
    startWorker();
    toggleControls(THREAD_RUNNING);
}





// This is perhaps the most important event handler of the worker thread
// as all the updates are done in this method...
void MyFrame::OnWorkerEvent(wxThreadEvent& event)
{
    int n = event.GetInt();
    if (n == -1)
    {
     
        // the dialog is aborted because the event came from another thread, so
        // we may need to wake up the main event loop for the dialog to be
        // really closed
        wxWakeUpIdle();
    }
    else
    {

        int current = gauge->GetValue();
        
        if (current < 100)
        { 
            current += 4;
            gauge->SetValue(current);

            wxString displayValue = wxString::Format(wxT("Value %i"), current);
            SetStatusText(displayValue);
        }
        else
        {
            m_running = false;
            toggleControls(THREAD_STOPPED);
            SetStatusText("Worker Done!");
        }
    }
}


/************************************************************************
        Section of the Utility Functions/Methods
*************************************************************************/

bool MyFrame::Cancelled()
{
    wxCriticalSectionLocker lock(m_csCancelled);

    return m_cancelled;
}

// This is the core method to start a worker thread..
void MyFrame::startWorker()
{
    MyWorkerThread* thread = new MyWorkerThread(this);

    if (thread->Create() != wxTHREAD_NO_ERROR)
    {
        wxLogError(wxT("Can't create thread!"));
        return;
    }


    // thread is not running yet, no need for crit sect
    m_cancelled = false;
    m_running = true;

    gauge->SetValue(0);
    thread->Run();
}

void MyFrame::toggleControls(bool threadRunning)
{
    startWorkerButton->Enable(!threadRunning);
    startWorkerMenuItem->Enable(!threadRunning);

}







