
// SimpleThread.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


#define WM_THREAD_COMPLETED       (WM_APP + 1)
#define WM_STOP_JOB               (WM_APP + 2)
#define WM_SEND_INFO_TO_JOB       (WM_APP + 3)




class CPostMsgThreadApp : public CWinApp
{
public:
	CPostMsgThreadApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPostMsgThreadApp theApp;
