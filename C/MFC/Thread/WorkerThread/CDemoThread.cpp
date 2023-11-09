// CMathThread.cpp : implementation file
//

#include "pch.h"
#include "SimpleThread.h"
#include "CDemoThread.h"


// CMathThread

IMPLEMENT_DYNCREATE(CDemoThread, CWinThread)



CDemoThread::CDemoThread()
{
	TRACE("CMathThread Constructor\n");
}

CDemoThread::~CDemoThread()
{
	TRACE("CMathThread Destructor\n");

}

BOOL CDemoThread::InitInstance()
{
	TRACE("CMathThread InitInstance\n");

	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CDemoThread::ExitInstance()
{

	TRACE("CMathThread ExitInstance\n");
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}


int CDemoThread::Run()
{
	TRACE("CMathThread::Run()\n");

	return 0;
}


BEGIN_MESSAGE_MAP(CDemoThread, CWinThread)
END_MESSAGE_MAP()


// CMathThread message handlers
