
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


// CMainDlg dialog



CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIMPLETHREAD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMainDlg::~CMainDlg()
{
	if (params != NULL) delete params;
 }



void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_START_THREAD_BUTTON, m_startThreadButton);
	DDX_Control(pDX, IDC_STOP_THREAD_BUTTON, m_stopThreadButton);

	DDX_Text(pDX, IDC_THREAD_STATUS_LABEL, m_threadStatusString);





}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_THREAD_BUTTON, &CMainDlg::OnBnClickedStartThreadButton)
	ON_BN_CLICKED(IDC_STOP_THREAD_BUTTON, &CMainDlg::OnBnClickedStopThreadButton)
	ON_BN_CLICKED(IDCANCEL, &CMainDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_VALIDATE_BUTTON, &CMainDlg::OnBnClickedValidateButton)
END_MESSAGE_MAP()


// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	m_threadStatusString.Format(_T("Not Started..."));



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



void CMainDlg::OnBnClickedStartThreadButton()
{

	m_threadStatusString.Format(_T("Running..."));

	m_startThreadButton.EnableWindow(FALSE);
	m_stopThreadButton.EnableWindow(TRUE);

	UpdateData(FALSE);

	startThread();
}


void CMainDlg::OnBnClickedStopThreadButton()
{
	m_startThreadButton.EnableWindow(TRUE);
	m_stopThreadButton.EnableWindow(FALSE);


	m_threadStatusString.Format(_T("Stopped"));
	UpdateData(FALSE);

	
	pauseThread();

}


void CMainDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}



////////////////////////////////////////////////////////////////////////////////////
//					 Thread related functions
////////////////////////////////////////////////////////////////////////////////////


void CMainDlg::startThread()
{

	if (m_threadRunning)
	{
		TRACE("Thread is already runnning and can't start another one!\n");
		return;
	}

	

	params = new CThreadParam(1,3);


	


	
	// This is the first version
	//m_threadHandle = (CMathThread*)AfxBeginThread(MyThreadProc, params);

		
		
	m_threadHandle = (CMathThread*)AfxBeginThread(RUNTIME_CLASS(CMathThread));
	m_threadHandle->m_bAutoDelete = FALSE;
	m_threadRunning = TRUE;


	

	DWORD exitCode;
	//if (GetExitCodeThread(m_threadHandle, &exitCode))
	if (GetExitCodeThread(m_threadHandle->m_hThread, &exitCode))
	{
		TRACE("\tMain Thread:GetExitCodeThread: exit code = %d\n", exitCode);
	}
	else
	{
		TRACE("\tMain Thread:GetExitCodeThread: Failed!!!, error code is %d\n", GetLastError());
	}


}

void CMainDlg::pauseThread()
{
	TRACE("CMainDlg::pauseThread()\n");

	m_threadRunning = FALSE;
}

void CMainDlg::terminateThread()
{

}

UINT MyThreadProc(LPVOID pParam)
{
	CThreadParam* pObject = (CThreadParam*)pParam;


	TRACE("MyThreadProc()\n");

	if (pObject == NULL ||
		!pObject->IsKindOf(RUNTIME_CLASS(CThreadParam)))
		return 1;   // if pObject is not valid

	TRACE("\tParams received: param1 = %d, param2 = %d\n", pObject->param1, pObject->param2);

	return 0;   // thread completed successfully
}


void CMainDlg::OnBnClickedValidateButton()
{
	if (m_threadHandle != NULL)
	{
		DWORD exitCode;


		if (GetExitCodeThread(m_threadHandle->m_hThread, &exitCode))
		{
			TRACE("\tButton Event:GetExitCodeThread: exit code = %d\n", exitCode);
		}
		else
		{

			//DWORD lastErrorCode = GetLastError();
			TRACE("\tButton Event:GetExitCodeThread: Failed!!!, error code is %d\n", GetLastError());

		}


	}
	else
	{
		int a = 2;
	}
}
