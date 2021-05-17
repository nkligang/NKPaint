// StyleListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "NKPaint.h"
#include "StyleListCtrl.h"
#include "AdvanceDraw.h"
#include "BaseFrame.h"
#include "PaintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


enum ImageStyleList{
	ISL_FIRST = -1,

	ISL_TRANSPARENT,
	ISL_ERASER,
	ISL_ZOOM,
	ISL_BRUSH,
	ISL_SPRAY,
	ISL_LINE,
	ISL_RECT,

	ISL_LAST
};//Add items between BSL_FIRST and BSL_LAST

CImageList	g_imageStyleList[ISL_LAST];
UINT		g_uCurSelStyle[16];


UINT * GetCurSelStyle(UINT index)
{
	switch(index)
	{
	case ID_NP_TOOL_POLY_CLIP:
		return &g_uCurSelStyle[0];
	case ID_NP_TOOL_SELECT:
		return &g_uCurSelStyle[0];
	case ID_NP_TOOL_ERASER:
		return &g_uCurSelStyle[2];
	case ID_NP_TOOL_FILL:
		return &g_uCurSelStyle[3];
	case ID_NP_TOOL_SUCKER:
		return &g_uCurSelStyle[4];
	case ID_NP_TOOL_ZOOM:
		return &g_uCurSelStyle[5];
	case ID_NP_TOOL_PEN:
		return &g_uCurSelStyle[6];
	case ID_NP_TOOL_BRUSH:
		return &g_uCurSelStyle[7];
	case ID_NP_TOOL_SPRAY:
		return &g_uCurSelStyle[8];
	case ID_NP_TOOL_TEXT:
		return &g_uCurSelStyle[0];
	case ID_NP_TOOL_LINE:
		return &g_uCurSelStyle[10];
	case ID_NP_TOOL_CURVE:
		return &g_uCurSelStyle[10];
	case ID_NP_TOOL_RECTANGLE:
		return &g_uCurSelStyle[12];
	case ID_NP_TOOL_POLYGON:
		return &g_uCurSelStyle[13];
	case ID_NP_TOOL_ELLIPSE:
		return &g_uCurSelStyle[14];
	case ID_NP_TOOL_ROUND_RECT:
		return &g_uCurSelStyle[15];
	default:
		return NULL;
	}
}

void LoadImageResource()
{
	static BOOL bLoaded = FALSE;
	if (!bLoaded)
	{
		LoadImageList(&g_imageStyleList[ISL_TRANSPARENT], IDB_NP_TRANSPARENT, CPoint(0,0), 36, 2);
		LoadImageList(&g_imageStyleList[ISL_ZOOM], IDB_NP_ZOOM_RATE, CPoint(0,0), 23, 4);
		LoadImageList(&g_imageStyleList[ISL_SPRAY], IDB_NP_SPRAY, CPoint(0,0), 24, 3);
		LoadImageList(&g_imageStyleList[ISL_LINE], IDB_NP_LINE, CPoint(0,0), 28, 5);
		LoadImageList(&g_imageStyleList[ISL_ERASER], IDB_NP_ERASER, CPoint(0,0), 10, 4);
		LoadImageList(&g_imageStyleList[ISL_RECT], IDB_NP_RECTANGLE, CPoint(1,1), 28, 3);
		LoadImageList(&g_imageStyleList[ISL_BRUSH], IDB_NP_BRUSH, CPoint(0,0), 8, 6);
		bLoaded = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CStyleListCtrl

CStyleListCtrl::CStyleListCtrl()
{
	m_nItemHeight = 0;
	LoadImageResource();
	m_nCurSelToolID = -1;
}

CImageList * CStyleListCtrl::GetSpecImageList(int index)
{
	switch (index)
	{
	case ID_NP_TOOL_POLY_CLIP:
	case ID_NP_TOOL_SELECT:
	case ID_NP_TOOL_TEXT:
		return &g_imageStyleList[ISL_TRANSPARENT];
	case ID_NP_TOOL_ZOOM:
		return &g_imageStyleList[ISL_ZOOM];
	case ID_NP_TOOL_BRUSH:
		return &g_imageStyleList[ISL_BRUSH];
	case ID_NP_TOOL_ERASER:
		return &g_imageStyleList[ISL_ERASER];
	case ID_NP_TOOL_LINE:
	case ID_NP_TOOL_CURVE:
		return &g_imageStyleList[ISL_LINE];
	case ID_NP_TOOL_SPRAY:
		return &g_imageStyleList[ISL_SPRAY];
	case ID_NP_TOOL_RECTANGLE:
	case ID_NP_TOOL_POLYGON:
	case ID_NP_TOOL_ELLIPSE:
	case ID_NP_TOOL_ROUND_RECT:
		return &g_imageStyleList[ISL_RECT];
	default:
		return NULL;
	}
}

CStyleListCtrl::~CStyleListCtrl()
{
}


BEGIN_MESSAGE_MAP(CStyleListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CStyleListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleListCtrl message handlers

BOOL CStyleListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_OWNERDRAWFIXED;

	return CListCtrl::PreCreateWindow(cs);
}

void CStyleListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if(m_nItemHeight > 0)
	   lpMeasureItemStruct->itemHeight = m_nItemHeight;
}

void CStyleListCtrl::SetItemHeight(UINT nHeight)
{
	m_nItemHeight = nHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;

	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

void CStyleListCtrl::SetCurSelToolStyle(UINT index)
{
	UINT * pStyle = GetCurSelStyle(m_nCurSelToolID);
	if (pStyle != NULL)
	{
		*pStyle = index;
		CBaseFrame * pFrame = (CBaseFrame*)AfxGetApp()->m_pMainWnd;
		ASSERT(pFrame);
		CPaintView * pView = (CPaintView *)pFrame->GetActiveView();
		ASSERT(pView);
		pView->OnToolStyleChanged(m_nCurSelToolID,index);
	}
}

UINT CStyleListCtrl::GetCurSelToolStyle()
{
	UINT * pStyle = GetCurSelStyle(m_nCurSelToolID);
	if (pStyle == NULL) return (UINT)-1;
	return *pStyle;
}

void CStyleListCtrl::SetCurSelTool(UINT nToolID, UINT uStyle)
{
	m_nCurSelToolID = nToolID;
	if (uStyle == INVALID_STYLE)
	{
		SetCurSelToolStyle(GetCurSelToolStyle());
	}
	else
	{
		UINT uOldSelStyle = GetCurSelToolStyle();
		if (uOldSelStyle != uStyle)
		{
			ASSERT(uStyle < (UINT)GetItemCount());
			SetItemState(uStyle, LVIS_SELECTED, 0);
			SetCurSelToolStyle(uStyle);
		}
	}
}

void CStyleListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	TCHAR lpBuffer[256];

	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
	lvi.iItem = lpDrawItemStruct->itemID ; 
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer ;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));

	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		if ( nCol > 0 ) 
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol-1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left ; 
		}

		// Get the text 
		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));

		CDC* pDC;
		pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

		CImageList * pImageList = this->GetImageList(LVSIL_SMALL);
		ASSERT(pImageList != NULL);

		int iItem = lvi.iItem;

		CRect rcItem;
		this->GetItemRect(iItem, &rcItem, LVIR_BOUNDS);

		IMAGEINFO imageInfo;
		pImageList->GetImageInfo(iItem,&imageInfo);
		CRect rcImage(imageInfo.rcImage);
		BITMAP bitmap;
		::GetObject(imageInfo.hbmImage, sizeof(BITMAP), &bitmap);

		CSize sizeSpace;
		sizeSpace.cx = (rcItem.Width() - rcImage.Width())/2;
		sizeSpace.cy = (rcItem.Height() - rcImage.Height())/2;

		CPoint ptStart(rcItem.left + sizeSpace.cx,rcItem.top + sizeSpace.cy);
		COLORREF HighLightInvert = RGB(255,255,255) - ::GetSysColor(COLOR_HIGHLIGHT);

		if ( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			if (bitmap.bmBitsPixel == 8 || bitmap.bmBitsPixel == 24)
			{
				pDC->FillSolidRect(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(), ::GetSysColor(COLOR_HIGHLIGHT));
				pImageList->DrawIndirect(pDC,iItem,ptStart,CSize(rcImage.Width(),rcImage.Height()),
					CPoint(0,0),ILD_TRANSPARENT);
			}
			else
			{
				pDC->FillSolidRect(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(), HighLightInvert);
				pImageList->DrawIndirect(pDC,iItem,ptStart,CSize(rcImage.Width(),rcImage.Height()),
					CPoint(0,0),ILD_TRANSPARENT);
				pDC->PatBlt(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),PATINVERT);
			}
		}
		else
		{
			pDC->FillSolidRect(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(), ::GetSysColor(COLOR_BTNFACE));
			pImageList->DrawIndirect(pDC,iItem,ptStart,CSize(rcImage.Width(),rcImage.Height()),CPoint(0,0));
		}

		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

		UINT   uFormat    = DT_LEFT ;
		::DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer), 
		&lpDrawItemStruct->rcItem, uFormat);

		pDC->SelectStockObject(SYSTEM_FONT);
	}

}

void CStyleListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if (pNMListView->uNewState & LVIS_SELECTED != 0)
	{
		SetCurSelToolStyle(pNMListView->iItem);
	}
	*pResult = 0;
}
