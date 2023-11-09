// MyCustomControl.cpp : implementation file
//

#include "stdafx.h"
#include "CustomControl.h"
#include "MyCustomControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MyCustomControl

MyCustomControl::MyCustomControl()
{
	//Register My window class
	RegisterWndClass();

	flag=FALSE;//Sets the drawing flag off
}

MyCustomControl::~MyCustomControl()
{
}


BEGIN_MESSAGE_MAP(MyCustomControl, CWnd)
	//{{AFX_MSG_MAP(MyCustomControl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MyCustomControl message handlers

BOOL MyCustomControl::RegisterWndClass()
{
	WNDCLASS windowclass;
	HINSTANCE hInst = AfxGetInstanceHandle();

	//Check weather the class is registerd already
	if (!(::GetClassInfo(hInst, MYWNDCLASS, &windowclass)))
	{
		//If not then we have to register the new class
		windowclass.style = CS_DBLCLKS;// | CS_HREDRAW | CS_VREDRAW;
		windowclass.lpfnWndProc = ::DefWindowProc;
		windowclass.cbClsExtra = windowclass.cbWndExtra = 0;
		windowclass.hInstance = hInst;
		windowclass.hIcon = NULL;
		windowclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		windowclass.hbrBackground = ::GetSysColorBrush(COLOR_WINDOW);
		windowclass.lpszMenuName = NULL;
		windowclass.lpszClassName = MYWNDCLASS;


		if (!AfxRegisterClass(&windowclass))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;

}

int MyCustomControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	
	return 0;
}


void MyCustomControl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(flag==FALSE)
	{
		oldpt=point;
		flag=TRUE;
	}
	
	//CWnd::OnLButtonDown(nFlags, point);
}

void MyCustomControl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(flag==TRUE)
	{
	
		CDC *d=GetDC();

		d->MoveTo(oldpt);
		d->LineTo(point);
	

		oldpt=point;
	
		ReleaseDC(d);
	}
	
	//CWnd::OnMouseMove(nFlags, point);
}

void MyCustomControl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	flag=FALSE;

	//CWnd::OnLButtonUp(nFlags, point);
}
