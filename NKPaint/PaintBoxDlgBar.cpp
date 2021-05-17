// PaintBoxDlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "NKPaint.h"
#include "PaintBoxDlgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COLORREF g_PaintBoxColors[] = 
{
	RGB(  0,  0,  0),RGB(255,255,255),
	RGB(128,128,128),RGB(192,192,192),
	RGB(128,  0,  0),RGB(255,  0,  0),
	RGB(128,128,  0),RGB(255,255,  0),
	RGB(  0,128,  0),RGB(  0,255,  0),
	RGB(  0,128,128),RGB(  0,255,255),
	RGB(  0,  0,128),RGB(  0,  0,255),
	RGB(128,  0,128),RGB(255,  0,255),
	RGB(128,128, 64),RGB(255,255,128),
	RGB(  0,  0, 64),RGB(  0,255,128),
	RGB(  0,128,255),RGB(128,255,255),
	RGB(  0, 64,128),RGB(128,128,255),
	RGB(128,  0,255),RGB(255,  0,128),
	RGB(128, 64,  0),RGB(255,128, 64),
};
/////////////////////////////////////////////////////////////////////////////
// CPaintBoxDlgBar dialog


CPaintBoxDlgBar::CPaintBoxDlgBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CPaintBoxDlgBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bBitmapCreated = FALSE;
	m_uBitCount = 24;
}


void CPaintBoxDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaintBoxDlgBar)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaintBoxDlgBar, CDialogBar)
	//{{AFX_MSG_MAP(CPaintBoxDlgBar)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
	ON_MESSAGE(WM_SET_BACKGROUND, OnSetBackground)
	ON_MESSAGE(WM_SET_FOREGROUND, OnSetForeground)
	ON_MESSAGE(WM_MODIFY_BITCOUNTTYPE, OnModifyBitCountType)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaintBoxDlgBar message handlers
LONG CPaintBoxDlgBar::OnInitDialog ( UINT wParam, LONG lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);
	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
	}
	return bRet;
}

void CPaintBoxDlgBar::SetForeBackColor(DWORD dwFlag, COLORREF fg, COLORREF bg)
{
	if (!m_bBitmapCreated)
	{
		m_bitmapForeBack.LoadBitmap(IDB_NP_COLOR_BF);
		m_bBitmapCreated = TRUE;
	}
	{
		CPaintDC paintDC(this);
		CDC dc;
		CRect rect;
		dc.CreateCompatibleDC(&paintDC);
		dc.SelectObject(m_bitmapForeBack);
		if (dwFlag & SET_BACK_GROUND_COLOR)
		{
			CBrush brush(bg);
			rect.SetRect(18,13,23,24);
			dc.FillRect(rect,&brush);
			rect.SetRect(12,19,18,24);
			dc.FillRect(rect,&brush);
			m_colorBackground = bg;
		}
		if (dwFlag & SET_FORE_GROUND_COLOR)
		{
			CBrush brush(fg);
			rect.SetRect(5,6,16,17);
			dc.FillRect(rect,&brush);
			m_colorForeground = fg;
		}
	}
	m_staticFG.SetBitmap(m_bitmapForeBack);
}

LRESULT CPaintBoxDlgBar::OnSetBackground(WPARAM wParam, LPARAM lParam)
{
	SetForeBackColor(SET_BACK_GROUND_COLOR,0,(COLORREF)wParam);
	return 0;
}

LRESULT CPaintBoxDlgBar::OnSetForeground(WPARAM wParam, LPARAM lParam)
{
	SetForeBackColor(SET_FORE_GROUND_COLOR,(COLORREF)wParam,0);
	return 0;
}

void CPaintBoxDlgBar::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialogBar::OnRButtonDown(nFlags, point);
}

void CPaintBoxDlgBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialogBar::OnLButtonDblClk(nFlags, point);
}

int CPaintBoxDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_staticFG.Create(NULL,WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY | SS_SUNKEN,CRect(5,8,37,40),this))
	{
		TRACE0("Failed to create static\n");
		return -1;      // fail to create		
	}
	SetForeBackColor(SET_BACK_GROUND_COLOR | SET_FORE_GROUND_COLOR, RGB(0,0,0), RGB(255,255,255));

	for (int i = 0,h = 0; i < 28; i++)
	{
		h = i % 2 == 0 ? 8 : 24;
		if (!m_staticBoxes[i].Create(NULL,WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY | SS_SUNKEN,CRect(36 + (i/2)*16,h,11,11),this))
		{
			TRACE0("Failed to create static\n");
			return -1;      // fail to create		
		}
		m_staticBoxes[i].SetCurrentColor(g_PaintBoxColors[i]);
		m_staticBoxes[i].SetParent(this);
	}
	SendMessage(WM_MODIFY_BITCOUNTTYPE,(WPARAM)m_uBitCount,TYPE_SET);
	return 0;
}

LRESULT CPaintBoxDlgBar::OnModifyBitCountType(WPARAM wParam, LPARAM lParam)
{
	UINT uBitCount = (UINT)wParam;
	int i;
	if (lParam == TYPE_CHANGE && m_uBitCount == uBitCount) return -1;
	switch(uBitCount)
	{
	case 1:
		for (i = 0; i < 28; i++)
		{
			m_staticBoxes[i].SetCurrentMonoMode(0);
			m_staticBoxes[i].ShowWindow(SW_SHOW);
		}
		break;
	case 4:
		for (i = 0; i < 28; i++)
		{
			m_staticBoxes[i].SetCurrentColor(g_PaintBoxColors[i]);
			i > 15 ? m_staticBoxes[i].ShowWindow(SW_HIDE) : m_staticBoxes[i].ShowWindow(SW_SHOW);
		}
		break;
	case 8:
	case 24:
		for (i = 0; i < 28; i++)
		{
			m_staticBoxes[i].SetCurrentColor(g_PaintBoxColors[i]);
			m_staticBoxes[i].ShowWindow(SW_SHOW);
		}
		break;
	}
	m_uBitCount = uBitCount;
	return 0;
}

void CPaintBoxDlgBar::SetBoxColor(int index, COLORREF color)
{
	ASSERT(index >= 0);
	switch(m_uBitCount)
	{
	case 1:
		if (index < 2)
		{
			m_staticBoxes[index].SetCurrentColor(color);
		}
		break;
	case 4:
		if (index < 16)
		{
			m_staticBoxes[index].SetCurrentColor(color);
		}
		break;
	case 8:
	case 24:
		if (index < 28)
		{
			m_staticBoxes[index].SetCurrentColor(color);
		}
		break;
	}
}
