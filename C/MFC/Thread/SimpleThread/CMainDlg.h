
// CMainDlg.h : header file
//

#pragma once


#include "CMathThread.h"

UINT MyThreadProc(LPVOID pParam);



class CThreadParam : public CObject
{
	
	public:

		CThreadParam() : param1(0), param2(0)					{}
		CThreadParam(int p1, int p2) : param1(p1), param2(p2)   {}

		int param1;
		int param2;
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


	CButton m_startThreadButton;
	CButton m_stopThreadButton;
	CString m_threadStatusString;
	HICON m_hIcon;

	//CWinThread* m_threadHandle = NULL;
	CMathThread* m_threadHandle = NULL;

	

	CThreadParam* params = NULL;
	BOOL m_threadRunning = FALSE;

	void startThread();
	void pauseThread();
	void terminateThread();


	
 
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedStartThreadButton();
	afx_msg void OnBnClickedStopThreadButton();
	
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedValidateButton();
};
