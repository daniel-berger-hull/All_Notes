// CustomControl.h : main header file for the CUSTOMCONTROL application
//

#if !defined(AFX_CUSTOMCONTROL_H__12EFE1C5_EC18_11D7_8254_B156B2239326__INCLUDED_)
#define AFX_CUSTOMCONTROL_H__12EFE1C5_EC18_11D7_8254_B156B2239326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCustomControlApp:
// See CustomControl.cpp for the implementation of this class
//

class CCustomControlApp : public CWinApp
{
public:
	CCustomControlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomControlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCustomControlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMCONTROL_H__12EFE1C5_EC18_11D7_8254_B156B2239326__INCLUDED_)
