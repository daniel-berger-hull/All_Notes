
// CMainDlg.h : header file
//

#pragma once


#include "CDemoThread.h"


#define WM_USER_THREAD_FINISHED (WM_USER+0x100)

#define PROC_THREAD_COMPLETED		1000
#define CLASS_PROC_THREAD_COMPLETED	1001


UINT WorkerThreadProc(LPVOID pParam);



class CThreadParam : public CObject
{
	
	public:

		CThreadParam() : param1(0), param2(0)					{}
		CThreadParam(HWND ptr, int p1, int p2) : hWnd(ptr), param1(p1), param2(p2)   {}

		int param1;
		int param2;
		HWND hWnd;
};

// CMainDlg dialog
class CMainDlg : public CDialogEx
{
// Construction
public:
	CMainDlg(CWnd* pParent = nullptr);	// standard constructor
	~CMainDlg();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMPLETHREAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	void updateSupportThreadControls(BOOL procThreadRunning, BOOL classProcThreadRunning);
	static UINT run(LPVOID pParam);


	CButton m_startProcThreadButton;
	CButton m_startClassProcThreadButton;
	CString m_procThreadStatusString;
	CString m_classProcThreadStatusString;

	HICON m_hIcon;

	
	CThreadParam* params = NULL;
	BOOL m_procThreadRunning = FALSE;
	BOOL m_classProcThreadRunning = FALSE;


	void startProcThread();
	void startClassProcThread();

	
	DECLARE_MESSAGE_MAP()

public:
	//afx_msg void OnBnClickedStartThreadButton();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStartProcThreadButton();
	afx_msg void OnBnClickedStartClassProcThreadButton();

	//afx_msg void OnBnClickedValidateButton();


	afx_msg LONG OnThreadFinished(WPARAM wParam, LPARAM lParam);
	
	
	
};
