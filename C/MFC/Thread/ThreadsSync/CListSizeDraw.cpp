// CListSizeDraw.cpp : implementation file
//

#include "pch.h"
#include "CListSizeDraw.h"


#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")



// CListSizeDraw

extern std::list<int>* agreggateSumList;



IMPLEMENT_DYNAMIC(CListSizeDraw, CStatic)

CListSizeDraw::CListSizeDraw()
{
	//m_cl = RGB(255, 0, 0);

	m_backgroundBrush.CreateSolidBrush(RGB(0, 0, 0));
	m_graphBarBrush.CreateSolidBrush(RGB(0, 255, 0));

	m_graphGreenPen.CreatePen(PS_DASH, 1, RGB(0, 255, 00));
	m_axisWhitePen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
}

CListSizeDraw::~CListSizeDraw()
{
}


BEGIN_MESSAGE_MAP(CListSizeDraw, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



void CListSizeDraw::PreSubclassWindow()
{
	
	CStatic::PreSubclassWindow();

	// Will start the thread that is updating the drawing of the graph
	AfxBeginThread(run, this);
}



void CListSizeDraw::OnNcDestroy()
{
	CStatic::OnNcDestroy();


	runDisplayThread = false;
	// Will wait a bit as the loop in the runThread method
	// is polling every 200 mSec, so putting a bit more delay
	// just to be sure that the Thread exists

	Sleep(250);

	TRACE("CListSizeDraw::OnNcDestroy()\n");

}


void CListSizeDraw::OnPaint()
{
	CPaintDC dc(this);


	CRect rect;
	GetClientRect(&rect);


	

	

	dc.FillRect(&rect, &m_backgroundBrush);

	drawAxis(&dc,&rect);


	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);


	drawData(&dc, &rect);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

}



void CListSizeDraw::drawAxis(CPaintDC* dc, CRect* rect)
{
	dc->SelectObject(&m_axisWhitePen);
	dc->MoveTo(DRAW_ZONE_MARGIN, DRAW_ZONE_MARGIN);
	dc->LineTo(DRAW_ZONE_MARGIN, rect->bottom - DRAW_ZONE_MARGIN);

	dc->MoveTo(DRAW_ZONE_MARGIN, rect->bottom - DRAW_ZONE_MARGIN);
	dc->LineTo(rect->right - DRAW_ZONE_MARGIN, rect->bottom - DRAW_ZONE_MARGIN);
}






void CListSizeDraw::drawData(CPaintDC* dc, CRect* rect)
{
	
	int numPointsToPlot = agreggateSumList->size();
	// Make sure that there is a least some data to plot...
	if (numPointsToPlot == 0)  return;
	
	

	// Adding one to the startX, as the vertical axis is drawn already on the margin
	int startX = DRAW_ZONE_MARGIN + 1;
	int baseY = rect->bottom - DRAW_ZONE_MARGIN;
	int centerY = rect->bottom / 2;
	int dX = 10;  
	
	dc->SelectObject(&m_graphGreenPen);


	CRect nextBarRect;
		
	// Will display the aggregate sums calculated by the Tread B
	// in bar graph.
	std::list<int>::iterator it = agreggateSumList->begin();

	// But obviously, over time, the number of  aggregate sums 
	// will get very large, so this code below make sure pick up
	// the MAX_BAR_DISPLAY items of the list..
	if (numPointsToPlot > MAX_BAR_DISPLAY)
		std::advance(it, numPointsToPlot - MAX_BAR_DISPLAY);
	

	for ( ; it != agreggateSumList->end(); ++it)
	{
		int endX = startX + dX - 1;
		int startY = baseY - *it;
		// If the consume thread is stopped while the generator continues
		// it can happen that the number of items in the list of random number can be VERY large!
		// so we make sure to clip this value in the code below
		if (startY < DRAW_ZONE_MARGIN)  startY = DRAW_ZONE_MARGIN;


		nextBarRect.left = startX;
		nextBarRect.top = startY;
		nextBarRect.right = endX;
		nextBarRect.bottom = baseY;

		dc->FillRect(&nextBarRect, &m_graphBarBrush);

		startX += dX;
	}
}





void CListSizeDraw::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDown(nFlags, point);
}


//////////////////////////////////////////////////
//		 Section of Thread related functions
//////////////////////////////////////////////////

UINT CListSizeDraw::run(LPVOID p)
{
		
	TRACE(" CListSizeDraw::run(LPVOID p) static version...\n");
	CListSizeDraw* me = (CListSizeDraw*)p;
	me->runThread();

	return 0;
}


void CListSizeDraw::runThread()
{


	TRACE(" CListSizeDraw::runThread() started..\n");

	runDisplayThread = true;
	int count = 0;
	while (runDisplayThread)
	{

		if (++count > 2500)
		{
			runDisplayThread = false;
		}

		Invalidate();
		Sleep(DISPLAY_REFRESH_PERIOD);
	}

	

	TRACE(" CListSizeDraw::runThread() terminated..\n");
}


BOOL CListSizeDraw::OnEraseBkgnd(CDC* pDC)
{
	return 0;
}
