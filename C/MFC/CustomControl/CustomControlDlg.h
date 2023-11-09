// CustomControlDlg.h : header file
//

#include "MyCustomControl.h"

#if !defined(AFX_CUSTOMCONTROLDLG_H__12EFE1C7_EC18_11D7_8254_B156B2239326__INCLUDED_)
#define AFX_CUSTOMCONTROLDLG_H__12EFE1C7_EC18_11D7_8254_B156B2239326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCustomControlDlg dialog

class CCustomControlDlg : public CDialog
{
// Construction
public:
	CCustomControlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCustomControlDlg)
	enum { IDD = IDD_CUSTOMCONTROL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	MyCustomControl m_drawpad;//This is our custom control

	// Generated message map functions
	//{{AFX_MSG(CCustomControlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMCONTROLDLG_H__12EFE1C7_EC18_11D7_8254_B156B2239326__INCLUDED_)
