#pragma once

#include <list>


#define DRAW_ZONE_MARGIN		  10
#define	MAX_BAR_DISPLAY			  14

#define DISPLAY_REFRESH_PERIOD   20

// CListSizeDraw

class CListSizeDraw : public CStatic
{
	DECLARE_DYNAMIC(CListSizeDraw)

public:
	CListSizeDraw();
	virtual ~CListSizeDraw();

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);



protected:
	//COLORREF m_cl;
	CBrush m_backgroundBrush;
	CBrush m_graphBarBrush;

	CPen m_graphGreenPen;
	CPen m_axisWhitePen;


	bool runDisplayThread = false;
	
	
	void drawAxis(CPaintDC* dc, CRect* rect);
	void drawData(CPaintDC* dc, CRect* rect);


	static UINT run(LPVOID p);
	void runThread();

	

	DECLARE_MESSAGE_MAP()

public:
	
	afx_msg void OnNcDestroy();
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


