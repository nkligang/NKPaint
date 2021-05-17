// MapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NKPaint.h"
#include "MapDlg.h"
#include "PaintDoc.h"
#include "PaintView.h"
#include "BaseFrame.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapDlg dialog


CDlgMap::CDlgMap(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMap, CDialog)
	//{{AFX_MSG_MAP(CMapDlg)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapDlg message handlers

void CDlgMap::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient, rcPaint;
	this->GetClientRect(rcClient);
	rcPaint = rcClient;

	CPaintView * pDrawView;
	pDrawView = (CPaintView *)((CBaseFrame*)AfxGetMainWnd())->GetActiveView();
	CPaintDoc * pDoc = pDrawView->GetDocument();
	HBITMAP hBitmap = pDrawView->GetBitmap();
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(&dc);
	HBITMAP hOldBitmap = (HBITMAP)dcTemp.SelectObject(hBitmap);
	BITMAP bitmap;
	::GetObject(hBitmap,sizeof(bitmap),&bitmap);
	
	if (((float)rcPaint.Width()) / bitmap.bmWidth > ((float)rcPaint.Height()) / bitmap.bmHeight)
	{
		rcPaint.right = (LONG)(bitmap.bmWidth * ((float)rcPaint.Height()) / bitmap.bmHeight);
		rcClient.left = rcPaint.right;
	} 
	else
	{
		rcPaint.bottom = (LONG)(bitmap.bmHeight * ((float)rcPaint.Width()) / bitmap.bmWidth);
		rcClient.top = rcPaint.bottom;
	}

	dc.SetStretchBltMode(HALFTONE);
	dc.StretchBlt(rcPaint.left,rcPaint.top, rcPaint.Width(), rcPaint.Height(), &dcTemp, rcPaint.left,rcPaint.top, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	dc.FillRect(&rcClient, &g_cbSystemBackground);

	dcTemp.SelectObject(hOldBitmap);
	dcTemp.DeleteDC();
}

void CDlgMap::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	this->Invalidate();
}

BOOL CDlgMap::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;	
//	return CDialog::OnEraseBkgnd(pDC);
}
