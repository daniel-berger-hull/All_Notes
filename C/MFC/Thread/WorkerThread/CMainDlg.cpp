
// CMainDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SimpleThread.h"
#include "CMainDlg.h"
#include "afxdialogex.h"

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
	
 }



void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_START_PROC_THREAD_BUTTON, m_startProcThreadButton);
	DDX_Control(pDX, IDC_START_CLASS_PROC_THREAD_BUTTON, m_startClassProcThreadButton);

	DDX_Text(pDX, IDC_PROC_THREAD_STATUS_LABEL, m_procThreadStatusString);
	DDX_Text(pDX, IDC_CLASS_PROC_THREAD_STATUS_LABEL, m_classProcThreadStatusString);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMainDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_START_PROC_THREAD_BUTTON, &CMainDlg::OnBnClickedStartProcThreadButton)
	ON_BN_CLICKED(IDC_START_CLASS_PROC_THREAD_BUTTON, &CMainDlg::OnBnClickedStartClassProcThreadButton)

	ON_MESSAGE(WM_USER_THREAD_FINISHED, OnThreadFinished)
END_MESSAGE_MAP()



BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	updateSupportThreadControls(FALSE,FALSE);

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


void CMainDlg::OnBnClickedStartProcThreadButton()
{
	startProcThread();
	updateSupportThreadControls(m_procThreadRunning, m_classProcThreadRunning);
}


void CMainDlg::OnBnClickedStartClassProcThreadButton()
{
	startClassProcThread();
	updateSupportThreadControls(m_procThreadRunning, m_classProcThreadRunning);
}



// This handler is common to both thread procedure, so a conditionnall processing is required
LONG CMainDlg::OnThreadFinished(WPARAM wParam, LPARAM lParam)
{

	if (wParam == PROC_THREAD_COMPLETED)
	{
		m_procThreadRunning = FALSE;
		updateSupportThreadControls(m_procThreadRunning, m_classProcThreadRunning);
#ifdef DEBUG_TRACE
		TRACE(" CMainDlg::OnThreadFinished Procedure thread completed\n");
#endif 		
	}
		
	else if (wParam == CLASS_PROC_THREAD_COMPLETED)
	{
		m_classProcThreadRunning = FALSE;
		updateSupportThreadControls(m_procThreadRunning, m_classProcThreadRunning);
#ifdef DEBUG_TRACE
		TRACE(" CMainDlg::OnThreadFinished Dialog Class Procedure thread completed\n");
#endif 
	}
		
	return 0;
}

// Utility method, to keep the controls in sync in the GUI
void CMainDlg::updateSupportThreadControls(BOOL procThreadRunning, BOOL classProcThreadRunning)
{

	if (procThreadRunning)  m_procThreadStatusString.Format(_T("Running..."));
	else m_procThreadStatusString.Format(_T("Stopped..."));
	
	m_startProcThreadButton.EnableWindow(!procThreadRunning);
	
	

	if (classProcThreadRunning)  m_classProcThreadStatusString.Format(_T("Running..."));
	else m_classProcThreadStatusString.Format(_T("Stopped..."));
	
	m_startClassProcThreadButton.EnableWindow(!classProcThreadRunning);


	UpdateData(FALSE);
}


////////////////////////////////////////////////////////////////////////////////////
//					 Thread related functions
////////////////////////////////////////////////////////////////////////////////////


// This method show how to use a plain C function has thread procedure.
// Currently, the method is in the same file than the Dialog class, 
// but it could be in any other file too...
void CMainDlg::startProcThread()
{

	if (m_procThreadRunning)
	{
#ifdef DEBUG_TRACE
		TRACE(" CMainDlg::startThread(): Thread is already runnning and can't start another one!\n");
#endif 
		return;
	}

	params = new CThreadParam(m_hWnd,1,3);
	AfxBeginThread(WorkerThreadProc, params);
	
	m_procThreadRunning = TRUE;
}

// This is the second type of method, which is using a member function a class
// Here, the member function in on the dialog class, but it could be on any other class
// Note: The member function used as procedure for the thread MUST be 'static'
void CMainDlg::startClassProcThread() 
{
	if (m_classProcThreadRunning)
	{
#ifdef DEBUG_TRACE
		TRACE(" CMainDlg::startClassProcThread(): Thread is already runnning and can't start another one!\n");
#endif 
		return;
	}

	params = new CThreadParam(m_hWnd, 1, 3);
	AfxBeginThread(run, params);

	m_classProcThreadRunning = TRUE;
}




UINT CMainDlg::run(LPVOID pParam)
{

	CThreadParam* pObject = (CThreadParam*)pParam;

	if (pObject == NULL ||
		!pObject->IsKindOf(RUNTIME_CLASS(CThreadParam)))
		return 1;   // if pObject is not valid


#ifdef DEBUG_TRACE
	TRACE("CMainDlg::run() --> Params received: param1 = %d, param2 = %d\n", pObject->param1, pObject->param2);
#endif 

	Sleep(1000);

	::PostMessage(pObject->hWnd, WM_USER_THREAD_FINISHED, (WPARAM)CLASS_PROC_THREAD_COMPLETED, 0);

	delete pObject;

	return 0;   
}


UINT WorkerThreadProc(LPVOID pParam)
{
	CThreadParam* pObject = (CThreadParam*)pParam;

	if (pObject == NULL ||
		!pObject->IsKindOf(RUNTIME_CLASS(CThreadParam)))
		return 1;   // if pObject is not valid

#ifdef DEBUG_TRACE
	TRACE("MyThreadProc() --> Params received: param1 = %d, param2 = %d\n", pObject->param1, pObject->param2);
#endif 

	Sleep(1000);

	int nCount = 1000;
	::PostMessage(pObject->hWnd, WM_USER_THREAD_FINISHED, (WPARAM)PROC_THREAD_COMPLETED, 0);


	delete pObject;

	return 0;   // thread completed successfully
}




