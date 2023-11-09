#include "MyWorkerThread.h"
#include "Events.h"


MyWorkerThread::MyWorkerThread(MyFrame* frame)
    : wxThread()
{
    m_frame = frame;
    m_count = 0;
}


wxThread::ExitCode MyWorkerThread::Entry()
{
#if TEST_YIELD_RACE_CONDITION
    if (TestDestroy())
        return NULL;

    wxThreadEvent event(wxEVT_THREAD, WORKER_EVENT);

    event.SetInt(50);
    wxQueueEvent(m_frame, event.Clone());

    event.SetInt(-1);
    wxQueueEvent(m_frame, event.Clone());
#else
    for (m_count = 0; !m_frame->Cancelled() && (m_count < 100); m_count++)
    {
        // check if we were asked to exit
        if (TestDestroy())
            break;

        // create any type of command event here
        wxThreadEvent event(wxEVT_THREAD, ID_WORKER_EVENT);
        event.SetInt(m_count);

        // send in a thread-safe way
        wxQueueEvent(m_frame, event.Clone());

        wxMilliSleep(200);
    }

    wxThreadEvent event(wxEVT_THREAD, ID_WORKER_EVENT);
    event.SetInt(-1); // that's all
    wxQueueEvent(m_frame, event.Clone());
#endif

    return NULL;
}

void MyWorkerThread::OnExit()
{
}