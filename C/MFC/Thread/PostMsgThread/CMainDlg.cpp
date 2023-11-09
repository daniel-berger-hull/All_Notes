
// CMainDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CPostMsgThreadApp.h"
#include "CMainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG_TRACE
#define new DEBUG_TRACE_NEW
#endif


// CMainDlg dialog



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


	DDX_Control(pDX, IDC_START_THREAD_BUTTON, m_startThreadButton);
	DDX_Control(pDX, IDC_STOP_THREAD_BUTTON, m_stopThreadButton);
	
	DDX_Text(pDX, IDC_THREAD_STATUS_LABEL, m_threadStatusString);
}



BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_START_THREAD_BUTTON, &CMainDlg::OnBnClickedStartThreadButton)
	ON_BN_CLICKED(IDC_STOP_THREAD_BUTTON, &CMainDlg::OnBnClickedStopThreadButton)
	ON_BN_CLICKED(IDC_POST_MESSAGE_BUTTON, &CMainDlg::OnBnClickedPostMessageButton)
	ON_BN_CLICKED(IDCANCEL, &CMainDlg::OnBnClickedCancel)

	ON_MESSAGE(WM_THREAD_COMPLETED, &CMainDlg::OnThreadCompletedMessage)

END_MESSAGE_MAP()





// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_redBrush.CreateSolidBrush(RGB(255,0,0));
	m_GreenBrush.CreateSolidBrush(RGB(0, 255, 0));



	m_threadStatusString.Format(_T("Not Started..."));

#ifdef DEBUG_TRACE
	TRACE("CMainDlg::OnInitDialog()  Thread ID #%d\n", GetCurrentThreadId());
#endif 

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


HBRUSH CMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Are we painting the IDC_MYSTATIC control? We can use
	// CWnd::GetDlgCtrlID() to perform the most efficient test.
	if (pWnd->GetDlgCtrlID() == IDC_THREAD_STATUS_LABEL)
	{
		// Set the text color to red
		pDC->SetTextColor(RGB(255, 255, 255));

		// Set the background mode for text to transparent 
		// so background will show thru.
		pDC->SetBkMode(TRANSPARENT);

		// Return handle to our CBrush object
		
		if (m_threadRunning)
			hbr = m_redBrush;
		else
			hbr = m_GreenBrush;
	}

	return hbr;
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMainDlg::OnBnClickedStartThreadButton()
{
	startThread();
	updateSupportThreadControls(TRUE);
	
}


void CMainDlg::OnBnClickedStopThreadButton()
{
	stopThread();
	updateSupportThreadControls(FALSE);
}


void CMainDlg::OnBnClickedCancel()
{
	if (m_threadRunning)
	{
		stopThread();
		Sleep(500);
	}
	
	CDialogEx::OnCancel();
}


void CMainDlg::OnBnClickedPostMessageButton()
{

#ifdef DEBUG_TRACE
	TRACE("\tPosting WM_JOB_DONE to CMathThread...\n");
#endif 

	m_threadHandle->PostThreadMessageW(WM_SEND_INFO_TO_JOB, 10, 0);
}




// This is the handler of the message from the Thread when it is completed
// the thread has called PostMessage(WM_THREAD_COMPLETED, ....)
LRESULT CMainDlg::OnThreadCompletedMessage(WPARAM wParam, LPARAM lParam)
{
	m_threadRunning = FALSE;
	updateSupportThreadControls(FALSE);

	return NULL;
}


void CMainDlg::updateSupportThreadControls(BOOL threadRunning)
{

	if (threadRunning)  m_threadStatusString.Format(_T("Running..."));
	else m_threadStatusString.Format(_T("Stopped..."));
	m_startThreadButton.EnableWindow(!threadRunning);
	m_stopThreadButton.EnableWindow(threadRunning);

	UpdateData(FALSE);

	CStatic* ctrl = (CStatic*)GetDlgItem(IDC_THREAD_STATUS_LABEL);

	ctrl->Invalidate();
}



////////////////////////////////////////////////////////////////////////////////////
//					 Thread related functions
////////////////////////////////////////////////////////////////////////////////////


void CMainDlg::startThread()
{

	if (m_threadRunning)
	{
#ifdef DEBUG_TRACE
		TRACE("Thread is already runnning and can't start another one!\n");
#endif 
		return;
	}


	// The MFC framework is handling the instantiation of the CWinThread object here...	
	m_threadHandle = (CDemoThread*)AfxBeginThread(RUNTIME_CLASS(CDemoThread));

	// This is a trick found at: https://stackoverflow.com/questions/28430714/cwinthread-message-maps
	// There should be alread something available in the MFC framework
	// instead of using a pointer to its owner in the work thread...
	// More research may be required
	m_threadHandle->setStayAliveDuration(4000);
	m_threadHandle->setOwner(this);			
	m_threadRunning = TRUE;


}

void CMainDlg::stopThread()
{
	if (!m_threadRunning)
	{
#ifdef DEBUG_TRACE
		TRACE("CMainDlg::stopThread(): Thread is not runnning, so there is nothing to stop!\n");
#endif 
		return;
	}

	if (m_threadHandle == NULL)
	{
#ifdef DEBUG_TRACE
		TRACE("CMainDlg::stopThread(): There is be a non NULL handle to the running Thread, but there is NULL pointer found here!\n");
#endif 
		return;
	}


	m_threadHandle->PostThreadMessage(WM_STOP_JOB, 0, 0);

#ifdef DEBUG_TRACE
	TRACE("CMainDlg::stopThread() Posted message WM_STOP_JOB\n");
#endif 


	m_threadRunning = FALSE;
}

void CMainDlg::terminateThread()
{

}





