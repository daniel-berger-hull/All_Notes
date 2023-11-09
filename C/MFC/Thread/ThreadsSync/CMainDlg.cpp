
// CMainDlg.cpp : implementation file
//

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "pch.h"
#include "framework.h"
#include "SimpleThread.h"
#include "CMainDlg.h"
#include "afxdialogex.h"

#include "Data.h"

#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIMPLETHREAD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMainDlg::~CMainDlg()
{

#ifdef DEBUG_TRACE
	TRACE("CMainDlg Destructor");
#endif 	
	if (pStopRequestA != NULL) delete pStopRequestA;
	if (pStopRequestB != NULL) delete pStopRequestB;


	if (randomList != NULL)
	{
		randomList->clear();
		delete randomList;
	}
				
	if (agreggateSumList != NULL)
	{
		agreggateSumList->clear();
		delete agreggateSumList;
	}

 }



void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_START_THREAD_A_BUTTON, m_startThreadAButton);
	DDX_Control(pDX, IDC_START_THREAD_B_BUTTON, m_startThreadBButton);
	DDX_Control(pDX, IDC_STOP_THREAD_A_BUTTON, m_stopThreadAButton);
	DDX_Control(pDX, IDC_STOP_THREAD_B_BUTTON, m_stopThreadBButton);
	
	DDX_Control(pDX, IDC_LIST_SIZE_GRAPH, m_listSizeDraw);

	DDX_Text(pDX, IDC_THREAD_A_STATUS_LABEL, m_threadAStatusString);
	DDX_Text(pDX, IDC_THREAD_B_STATUS_LABEL, m_threadBStatusString);
	DDX_Text(pDX, IDC_LIST_SIZE_STATIC, m_listSizeString);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMainDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_START_THREAD_A_BUTTON, &CMainDlg::OnBnClickedStartThreadAButton)
	ON_BN_CLICKED(IDC_START_THREAD_B_BUTTON, &CMainDlg::OnBnClickedStartThreadBButton)
	ON_BN_CLICKED(IDC_STOP_THREAD_A_BUTTON, &CMainDlg::OnBnClickedStopThreadAButton)
	ON_BN_CLICKED(IDC_STOP_THREAD_B_BUTTON, &CMainDlg::OnBnClickedStopThreadBButton)

	ON_MESSAGE(WM_USER_THREAD_FINISHED, OnThreadFinished)
	ON_MESSAGE(WM_USER_THREAD_UPDATE_COUNT_DISPLAY, OnUpdateCountDisplay)
END_MESSAGE_MAP()


BOOL CMainDlg::DestroyWindow()
{
#ifdef DEBUG_TRACE
	TRACE("CMainDlg::DestroyWindow() will request the exit of the Threads A and B...");
#endif 	

	// Threads may still be running, so try to stop them...
	stopThread(THREAD_A);
	stopThread(THREAD_B);
	// And let some time to the thread to exit cleanly...
	Sleep(400);
	return CWnd::DestroyWindow();
}


BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	updateSupportThreadControls(FALSE,FALSE);


	CListSizeDraw* ctrl = (CListSizeDraw*)GetDlgItem(IDC_LIST_SIZE_GRAPH);


	randomList = new std::list<int>();
	agreggateSumList = new std::list<int>();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		//CPaintDC dc(this); // device context for painting

		//
		////Graphics graphics(pDC->m_hDC);
		//Graphics graphics(dc.m_hDC);

		//LinearGradientBrush linGrBrush(Point(0, 10),
		//	Point(200, 10),
		//	Color(255, 255, 0, 0),   // opaque red
		//	Color(255, 0, 0, 255));  // opaque blue

		//Pen pen(&linGrBrush);
		//graphics.DrawLine(&pen, 0, 110, 200, 110);
		//graphics.FillEllipse(&linGrBrush, 0, 130, 200, 100);
		//graphics.FillRectangle(&linGrBrush, 0, 255, 500, 30);



		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMainDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void CMainDlg::OnBnClickedStartThreadAButton()
{
	starThread(THREAD_A);
	updateSupportThreadControls(m_threadARunning, m_threadBRunning);
}

void CMainDlg::OnBnClickedStopThreadAButton()
{
	stopThread(THREAD_A);
	updateSupportThreadControls(m_threadARunning, m_threadBRunning);

}



void CMainDlg::OnBnClickedStartThreadBButton()
{
	starThread(THREAD_B);
	updateSupportThreadControls(m_threadARunning, m_threadBRunning);

}

void CMainDlg::OnBnClickedStopThreadBButton()
{
	stopThread(THREAD_B);
	updateSupportThreadControls(m_threadARunning, m_threadBRunning);
}



// This handler is common to both thread procedure, so a conditionnall processing is required
LONG CMainDlg::OnThreadFinished(WPARAM wParam, LPARAM lParam)
{

	
	if (wParam == THREAD_A)
	{
		m_threadARunning = FALSE;
		updateSupportThreadControls(m_threadARunning, m_threadBRunning);
#ifdef DEBUG_TRACE
		TRACE(" CMainDlg::OnThreadFinished Procedure thread completed\n");
#endif 		
	}
		
	else if (wParam == THREAD_B)
	{
		m_threadBRunning = FALSE;
		updateSupportThreadControls(m_threadARunning, m_threadBRunning);
#ifdef DEBUG_TRACE
		TRACE(" CMainDlg::OnThreadFinished Dialog Class Procedure thread completed\n");
#endif 
	}
		
	return 0;
}


LONG CMainDlg::OnUpdateCountDisplay(WPARAM wParam, LPARAM lParam)
{
	m_listSizeString.Format(_T("%d"), wParam);
	UpdateData(FALSE);

	return 0;
}




// Utility method, to keep the controls in sync in the GUI
void CMainDlg::updateSupportThreadControls(BOOL threadARunning, BOOL threadBRunning)
{
	if (threadARunning)  m_threadAStatusString.Format(_T("Running..."));
	else m_threadAStatusString.Format(_T("Stopped..."));
	
	m_startThreadAButton.EnableWindow(!threadARunning);
	m_stopThreadAButton.EnableWindow(threadARunning);
	
	if (threadBRunning)  m_threadBStatusString.Format(_T("Running..."));
	else m_threadBStatusString.Format(_T("Stopped..."));
	
	m_startThreadBButton.EnableWindow(!threadBRunning);
	m_stopThreadBButton.EnableWindow(threadBRunning);


	m_listSizeString.Format(_T("0"));

	UpdateData(FALSE);
}


////////////////////////////////////////////////////////////////////////////////////
//					 Thread related functions
////////////////////////////////////////////////////////////////////////////////////


// This method show how to use a plain C function has thread procedure.
// Currently, the method is in the same file than the Dialog class, 
// but it could be in any other file too...


void CMainDlg::starThread(int index)
{


	if (index == THREAD_A)
	{
		if (m_threadARunning)
		{
#ifdef DEBUG_TRACE
			TRACE(" CMainDlg::startThread(): Thread A is already runnning and can't start another one!\n");
#endif 
			return;
		}

		if (pStopRequestA != NULL) 	delete pStopRequestA;
		pStopRequestA = new CEvent(FALSE, TRUE);
		params = new CThreadParam(m_hWnd, pStopRequestA);


		TRACE("CMainDlg::starThread\n");
		TRACE("\t\t$$$ pStopRequestA : %p, pStopRequestA->m_hObject: %p \n", pStopRequestA, pStopRequestA->m_hObject);


		AfxBeginThread(WorkerThreadA, params);
		m_threadARunning = TRUE;
	}
	else if (index == THREAD_B)
	{
		if (m_threadBRunning)
		{
#ifdef DEBUG_TRACE
			TRACE(" CMainDlg::startThread(): Thread B is already runnning and can't start another one!\n");
#endif 
			return;
		}

		if (pStopRequestB != NULL)  delete pStopRequestB;
		pStopRequestB = new CEvent(FALSE, TRUE);
		params = new CThreadParam(m_hWnd, pStopRequestB);


		AfxBeginThread(WorkerThreadB, params);
		m_threadBRunning = TRUE;
	}

}


void CMainDlg::stopThread(int index)
{
	if (index == THREAD_A)
	{
		if (!m_threadARunning)
		{
#ifdef DEBUG_TRACE
			TRACE(" CMainDlg::stopThread(): Thread A is stopped already!\n");
#endif 
			return;
		}
		if (pStopRequestA == NULL)
		{
#ifdef DEBUG_TRACE
			TRACE(" CMainDlg::stopThread(): Threre is a problem with the Stop Event of Thread A!\n");
#endif 
			return;
		}

#ifdef DEBUG_TRACE
		TRACE(" CMainDlg::stopThread(): Setting Event A\n");
#endif 
		pStopRequestA->SetEvent();

		//pStopRequestA->SetEvent();
		//Since there is a polling on the Thread side, we need to wait a bit before assuming the thread
		//has caught the event and terminate...
		//Sleep(250);
		m_threadARunning = FALSE;
	}
	else if (index == THREAD_B)
	{
		if (!m_threadBRunning)
		{
#ifdef DEBUG_TRACE
			TRACE(" CMainDlg::stopThread(): Thread B is stopped already!\n");
#endif 
			return;
		}
		if (pStopRequestB == NULL)
		{
#ifdef DEBUG_TRACE
			TRACE(" CMainDlg::stopThread(): Threre is a problem with the Stop Event of Thread B!\n");
#endif 
			return;
		}

		pStopRequestB->SetEvent();
		//Since there is a polling on the Thread side, we need to wait a bit before assuming the thread
//has caught the event and terminate...
		//Sleep(250);
		m_threadBRunning = FALSE;

	}
}







void generate(std::list<int>* listTarget)
{

	// Create object for Single Lock
	CSingleLock lock(&c_s);
	int tmpSum = 0;
	lock.Lock();


	for (int i = 0; i < 10; i++)
	{
		int nextNum = rand() % 10;
		listTarget->push_back(nextNum);

		tmpSum += nextNum;
	}

	// Process

// Unlock
	lock.Unlock();

	//TRACE("$$$ Thread A generate added random values, tmp sum is %d\n", tmpSum);
	
}

void consume(std::list<int>* listTarget)
{
	CSingleLock lock(&c_s);
	int sum = 0;

	lock.Lock();

	for (std::list<int>::iterator it = listTarget->begin(); it != listTarget->end(); ++it)
	{
		sum+= *it;
	}

	listTarget->clear();
	lock.Unlock();

	//TRACE("### Thread B consume added value %d\n", sum);

	if (sum != 0)
		agreggateSumList->push_back(sum);



}



UINT WorkerThreadA(LPVOID pParam)
{
	CThreadParam* pObject = (CThreadParam*)pParam;

	if (pObject == NULL ||
		!pObject->IsKindOf(RUNTIME_CLASS(CThreadParam)))
		return 1;   // if pObject is not valid

	CEvent* pEvent = pObject->pStopRequest;
	VERIFY(pEvent != NULL);

#ifdef DEBUG_TRACE
	TRACE("$$$ Thread A --> Params received and and waiting for event...\n");
	TRACE("\t\t$$$ pEvent : %p, pEvent->m_hObject: %p \n", pEvent, pEvent->m_hObject);
#endif 

//
//
//	CSyncObject* pWaitObjects[] = { pEvent };
////	CMultiLock MultiLock(pWaitObjects, 1L);


	bool running = true;
	while (running)
	{ 
			switch (::WaitForSingleObject(pEvent->m_hObject, 5))
			{
				case WAIT_OBJECT_0:
					TRACE("\n$$$ Thread A --> event WAIT_OBJECT_0 received!\n");
					running = false;
					break;
				case WAIT_TIMEOUT:
					generate(randomList);
					break;
			}
			
			Sleep(THREAD_SLEEP_TIME);
	}


#ifdef DEBUG_TRACE
	TRACE("\n$$$ Thread A --> Will post an exit message and leave...\n");
#endif 

	// Let the CMainDlg class know that the Thread A is done...
	PostMessage(pObject->hWnd, WM_USER_THREAD_FINISHED, (WPARAM)THREAD_A, 0);

	delete pObject;

	return 0;   // thread completed successfully
}


UINT WorkerThreadB(LPVOID pParam)
{
	CThreadParam* pObject = (CThreadParam*)pParam;

	if (pObject == NULL ||
		!pObject->IsKindOf(RUNTIME_CLASS(CThreadParam)))
		return 1;   // if pObject is not valid

	CEvent* pEvent = pObject->pStopRequest;
	VERIFY(pEvent != NULL);


#ifdef DEBUG_TRACE
	TRACE("### Thread B --> Params received and and waiting for event...\n");
#endif 

	bool running = true;
	while (running)
	{
		switch (::WaitForSingleObject(pEvent->m_hObject, 5))
		{
			case WAIT_OBJECT_0:
				TRACE("\n### Thread B --> event WAIT_OBJECT_0 received!\n");
				running = false;
				break;
			case WAIT_TIMEOUT:
				consume(randomList);
				::PostMessage(pObject->hWnd, WM_USER_THREAD_UPDATE_COUNT_DISPLAY, (WPARAM)numberCount, 0);
				continue;
		}

		Sleep(THREAD_SLEEP_TIME);


	}

#ifdef DEBUG_TRACE
	TRACE("### Thread B --> Will post an exit message and leave...\n");
#endif 

	// Let the CMainDlg class know that the Thread B is done...
	::PostMessage(pObject->hWnd, WM_USER_THREAD_FINISHED, (WPARAM)THREAD_B, 0);
	delete pObject;

	return 0;   // thread completed successfully
}





