#if !defined(AFX_MYCUSTOMCONTROL_H__7A2B7621_EC26_11D7_8254_B961E1E45826__INCLUDED_)
#define AFX_MYCUSTOMCONTROL_H__7A2B7621_EC26_11D7_8254_B961E1E45826__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCustomControl.h : header file
//

#define MYWNDCLASS "MyDrawPad"

/////////////////////////////////////////////////////////////////////////////
// MyCustomControl window

class MyCustomControl : public CWnd
{
// Construction
public:
	MyCustomControl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyCustomControl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MyCustomControl();

	// Generated message map functions
protected:
	//{{AFX_MSG(MyCustomControl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDC cDC;
	BOOL RegisterWndClass();
	CPoint oldpt;
	BOOL flag;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCUSTOMCONTROL_H__7A2B7621_EC26_11D7_8254_B961E1E45826__INCLUDED_)
