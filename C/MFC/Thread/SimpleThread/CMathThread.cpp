// CMathThread.cpp : implementation file
//

#include "pch.h"
#include "SimpleThread.h"
#include "CMathThread.h"


// CMathThread

IMPLEMENT_DYNCREATE(CMathThread, CWinThread)



CMathThread::CMathThread()
{
	TRACE("CMathThread Constructor\n");
}

CMathThread::~CMathThread()
{
	TRACE("CMathThread Destructor\n");

}

BOOL CMathThread::InitInstance()
{
	TRACE("CMathThread InitInstance\n");

	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CMathThread::ExitInstance()
{

	TRACE("CMathThread ExitInstance\n");
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}


int CMathThread::Run()
{
	TRACE("CMathThread::Run()\n");

	return 0;
}


BEGIN_MESSAGE_MAP(CMathThread, CWinThread)
END_MESSAGE_MAP()


// CMathThread message handlers
