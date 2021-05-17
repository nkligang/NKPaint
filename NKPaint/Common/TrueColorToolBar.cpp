// TrueColorToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "TrueColorToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrueColorToolBar

CTrueColorToolBar::CTrueColorToolBar()
{
}

CTrueColorToolBar::~CTrueColorToolBar()
{
}


BEGIN_MESSAGE_MAP(CTrueColorToolBar, CToolBar)
	//{{AFX_MSG_MAP(CTrueColorToolBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrueColorToolBar message handlers
BOOL CTrueColorToolBar::LoadTrueColorToolBar(int nBtnWidth,
											 UINT uToolBar,
											 UINT uToolBarHot,
											 UINT uToolBarDisabled)
{
	if (!SetTrueColorToolBar(TB_SETIMAGELIST, uToolBar, nBtnWidth))
		return FALSE;
	
	if (!SetTrueColorToolBar(TB_SETHOTIMAGELIST, uToolBarHot, nBtnWidth))
		return FALSE;

	if (uToolBarDisabled) {
	if (!SetTrueColorToolBar(TB_SETDISABLEDIMAGELIST, uToolBarDisabled, nBtnWidth))
		return FALSE;
	}

	return TRUE;
}


BOOL CTrueColorToolBar::SetTrueColorToolBar(UINT uToolBarType,
							     	        UINT uToolBar,
										    int nBtnWidth)
{
	CImageList	cImageList;
	CBitmap		cBitmap;
	BITMAP		bmBitmap;
	CSize		cSize;
	int			nNbBtn;
	
	if (!cBitmap.Attach(LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(uToolBar), RT_BITMAP), MAKEINTRESOURCE(uToolBar),
								  IMAGE_BITMAP, 0, 0,
		                          LR_DEFAULTSIZE|LR_CREATEDIBSECTION)) ||
	    !cBitmap.GetBitmap(&bmBitmap))
		return FALSE;

	cSize  = CSize(bmBitmap.bmWidth, bmBitmap.bmHeight); 
	nNbBtn = cSize.cx/nBtnWidth;
	RGBTRIPLE* rgb		= (RGBTRIPLE*)(bmBitmap.bmBits);
	COLORREF   rgbMask	= RGB(rgb[0].rgbtRed,
		                      rgb[0].rgbtGreen,
							  rgb[0].rgbtBlue);
	
	if (!cImageList.Create(nBtnWidth, cSize.cy,
		                   ILC_COLOR24|ILC_MASK,
						   nNbBtn, 0))
		return FALSE;
	
	if (cImageList.Add(&cBitmap, rgbMask) == -1)
		return FALSE;

	SendMessage(uToolBarType, 0, (LPARAM)cImageList.m_hImageList);
	cImageList.Detach(); 
	cBitmap.Detach();
	
	return TRUE;
}

BOOL CTrueColorToolBar::SetMutiColumns(UINT nColumns)
{
	DWORD dwStyle = GetBarStyle();
	//CBRS_SIZE_FIXED must be defined in bar style
	ASSERT(dwStyle & CBRS_SIZE_FIXED);
	int nCount = GetToolBarCtrl().GetButtonCount();
	for(int i = 0; i < nCount; i++)
	{
		UINT nStyle = GetButtonStyle(i);
		nStyle |= TBSTYLE_CHECK;
		if ((i % nColumns) == nColumns-1)
			nStyle |= TBBS_WRAPPED;
		else
			nStyle &= ~TBBS_WRAPPED;
		SetButtonStyle(i, nStyle);
	}
	return TRUE;
}