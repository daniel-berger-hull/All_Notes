#include "SimpleGUIThread.h"

wxThread::ExitCode SimpleGUIThread.cpp::Entry()
{
    // uncomment this to check that disabling logging here does disable it for
    // this thread -- but not the main one if you also comment out wxLogNull
    // line in MyFrame::OnStartGUIThread()
    //wxLogNull noLog;

    // this goes to the main window
    wxLogMessage("GUI thread starting");

    // use a thread-specific log target for this thread to show that its
    // messages don't appear in the main window while it runs
    wxLogBuffer logBuf;
    wxLog::SetThreadActiveTarget(&logBuf);

    for (int i=0; i<GUITHREAD_NUM_UPDATES && !TestDestroy(); i++)
    {
        // inform the GUI toolkit that we're going to use GUI functions
        // from a secondary thread:
        wxMutexGuiEnter();

        {
            wxCriticalSectionLocker lock(m_dlg->m_csBmp);

            // draw some more stuff on the bitmap
            wxMemoryDC dc(m_dlg->m_bmp);
            dc.SetBrush((i%2)==0 ? *wxBLUE_BRUSH : *wxGREEN_BRUSH);
            dc.DrawRectangle(rand()%GUITHREAD_BMP_SIZE, rand()%GUITHREAD_BMP_SIZE, 30, 30);

            // simulate long drawing time:
            wxMilliSleep(200);
        }

        // if we don't release the GUI mutex the MyImageDialog won't be able to refresh
        wxMutexGuiLeave();

        // notify the dialog that another piece of our masterpiece is complete:
        wxThreadEvent event( wxEVT_THREAD, GUITHREAD_EVENT );
        event.SetInt(i+1);
        wxQueueEvent( m_dlg, event.Clone() );

        if ( !((i + 1) % 10) )
        {
            // this message will go to the buffer
            wxLogMessage("Step #%d.", i + 1);
        }

        // give the main thread the time to refresh before we lock the GUI mutex again
        // FIXME: find a better way to do this!
        wxMilliSleep(100);
    }

    // now remove the thread-specific thread target
    wxLog::SetThreadActiveTarget(NULL);

    // so that this goes to the main window again
    wxLogMessage("GUI thread finished.");

    return (ExitCode)0;
}