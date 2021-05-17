// ColorBoxStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ColorBoxStatic.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBoxStatic

CColorBoxStatic::CColorBoxStatic()
{
	m_bColorSet = FALSE;
	m_pParent = NULL;
	m_bBitmapCreated = FALSE;
}

CColorBoxStatic::~CColorBoxStatic()
{
}


BEGIN_MESSAGE_MAP(CColorBoxStatic, CStatic)
	//{{AFX_MSG_MAP(CColorBoxStatic)
	ON_WM_RBUTTONDOWN()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBoxStatic message handlers

void CColorBoxStatic::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ASSERT(m_pParent);
	m_pParent->SendMessage(WM_SET_BACKGROUND,(LPARAM)m_colorCur);
	CStatic::OnRButtonDown(nFlags, point);
}

void CColorBoxStatic::OnClicked() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_pParent);
	m_pParent->SendMessage(WM_SET_FOREGROUND,(LPARAM)m_colorCur);
}

void CColorBoxStatic::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ASSERT(m_bColorSet);//current color should be initialized
	CColorDialog colorDialog(m_colorCur);
	if (colorDialog.DoModal() == IDOK)
	{
		SetCurrentColor(colorDialog.GetColor());
	}

	CStatic::OnLButtonDblClk(nFlags, point);
}

void CreatePaintColor(CDC * pDC, CBitmap * pBitmap, COLORREF color, int nWidth, int nHeight)
{
	ASSERT(pDC != NULL && pBitmap != NULL);
	CBrush bursh(color);
	CDC dc;
	CRect rect(1,1,nWidth-1,nHeight-1);
	dc.CreateCompatibleDC(pDC);
	dc.SelectObject(pBitmap);
	dc.FillRect(rect,&bursh);
}

void CColorBoxStatic::SetCurrentColor(COLORREF color)
{
	m_colorCur  = color;
	m_bColorSet = TRUE;
	CPaintDC paintDC(this);
	if (!m_bBitmapCreated)
	{
		CDC dc;
		CRect rect;
		m_bitmapCur.CreateCompatibleBitmap(&paintDC,COLOR_BOX_WIDTH,COLOR_BOX_HEIGHT);
		m_bBitmapCreated = TRUE;
		dc.CreateCompatibleDC(&paintDC);
		dc.SelectObject(m_bitmapCur);
		rect.SetRect(0,COLOR_BOX_WIDTH-1,COLOR_BOX_WIDTH,COLOR_BOX_HEIGHT);
		dc.FillRect(rect,&g_cbSystemPlan);
		rect.SetRect(COLOR_BOX_WIDTH-1,0,COLOR_BOX_WIDTH,COLOR_BOX_HEIGHT);
		dc.FillRect(rect,&g_cbSystemPlan);
	}
	CreatePaintColor(&paintDC,&m_bitmapCur,m_colorCur,COLOR_BOX_WIDTH,COLOR_BOX_HEIGHT);
	this->SetBitmap(m_bitmapCur);
}

void CColorBoxStatic::SetCurrentMonoMode(int index)
{
	this->SetBitmap(NULL);
}

void CColorBoxStatic::SetParent(CWnd * pParent)
{
	m_pParent = pParent;
}
