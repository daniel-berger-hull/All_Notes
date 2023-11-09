// CDemoThread.cpp : implementation file
//

#include "pch.h"
#include "CPostMsgThreadApp.h"
#include "CDemoThread.h"



// CDemoThread

IMPLEMENT_DYNCREATE(CDemoThread, CWinThread)



CDemoThread::CDemoThread()
	: owner(NULL), stayAliveDuration(DEFAULT_STAY_ALIVE_DURATION_MSEC)
{


#ifdef DEBUG_TRACE
	TRACE("CDemoThread Constructor\n");
#endif 



}



CDemoThread::~CDemoThread()
{
#ifdef DEBUG_TRACE
	TRACE("CDemoThread Destructor\n");
#endif 
}

// Note: The message map appears to be ignored here, and the Run method of this
//        thread needs to poll messages (GetMessage function) and this should be not the case
//        as the MFC framework should handle this with the message map below
//        Need to research why!
//        Some articles on the web mention that the user defined messages ((WM_USER + X)
//        are not the same as the window message (WM_PAINT, WM_CREATE, etc)
//         and the perhaps, the ON_THREAD_MESSAGE should not be the one I use here
//          (i.e another ON_SOMETHING(...) macro from MFC)
BEGIN_MESSAGE_MAP(CDemoThread, CWinThread)
	ON_THREAD_MESSAGE(WM_STOP_JOB, OnStopJobMsg)
	ON_THREAD_MESSAGE(WM_SEND_INFO_TO_JOB, OnInfoMsg)

	//ON_MESSAGE(WM_STOP_JOB, OnStopJobMsg)
	//ON_MESSAGE(WM_SEND_INFO_TO_JOB, OnInfoMsg)

END_MESSAGE_MAP()



BOOL CDemoThread::InitInstance()
{

#ifdef DEBUG_TRACE
	TRACE("CDemoThread InitInstance\n");
#endif 
	return TRUE;
}

int CDemoThread::ExitInstance()
{
#ifdef DEBUG_TRACE
	TRACE("CDemoThread ExitInstance\n");
#endif
	
	return CWinThread::ExitInstance();
}


int CDemoThread::Run()
{
#ifdef DEBUG_TRACE
	TRACE("CDemoThread::Run()  Thread ID #%d\n", m_nThreadID);
#endif
	
	
	int iteration = 0;
	MSG msg;

	DWORD tid = GetCurrentThreadId();

	// See note below in the message map section, as we should not poll the messages 
	// manually like it is done in this while loop...  
	while (GetMessage(&msg, NULL, 0, 0))
	{

		if (msg.message == WM_STOP_JOB)
		{
#ifdef DEBUG_TRACE
			TRACE("CDemoThread::Run()  message WM_STOP_JOB\n");
#endif
			break;
		}
		else if (msg.message == WM_SEND_INFO_TO_JOB)
		{
#ifdef DEBUG_TRACE
			TRACE("CDemoThread::Run()  message WM_SEND_INFO_TO_JOB\n");
#endif
		}

		if (++iteration > 500)
		{
			break;
		}
		Sleep(50);
	}




	
#ifdef DEBUG_TRACE
	TRACE("CDemoThread::Run()  Completed\n");
#endif

	owner->PostMessage(WM_THREAD_COMPLETED, 0, 0); // Tell our parent thread that this thread has finished work                    

#ifdef DEBUG_TRACE
	TRACE("CDemoThread::Run()  Message WM_THREAD_COMPLETED sent\n");
#endif

	
	return 0;
}




void CDemoThread::OnStopJobMsg(WPARAM wParam, LPARAM lParam)
{
#ifdef DEBUG_TRACE
	TRACE("CDemoThread::OnStopJobMsg message received!\n");
#endif

	

	return;
}

void CDemoThread::OnInfoMsg(WPARAM wParam, LPARAM lParam)
{
#ifdef DEBUG_TRACE
	TRACE("CDemoThread::OnInfoMsg message received!\n");
#endif

	return;
}





