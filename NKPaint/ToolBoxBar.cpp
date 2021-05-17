
#include "stdafx.h"
#include "NKPaint.h"
#include "ToolBoxBar.h"
#include "AdvanceDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBoxBar

BEGIN_MESSAGE_MAP(CToolBoxBar, CToolBar)
	//{{AFX_MSG_MAP(CToolBoxBar)
	ON_WM_CREATE()
    ON_UPDATE_COMMAND_UI_RANGE(ID_NP_TOOL_POLY_CLIP, ID_NP_TOOL_ROUND_RECT, OnUpdateTools)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBoxBar construction/destruction

CToolBoxBar::CToolBoxBar()
{
	m_nColumns = 2;
	m_cxLeftBorder = 5;
	m_cyTopBorder = 5;
	m_cxRightBorder = 5;
	m_cyBottomBorder = 5;
	m_nCurSelToolID = ID_NP_TOOL_NONE;
	for (UINT uToolID = ID_NP_TOOL_POLY_CLIP; uToolID <= ID_NP_TOOL_ROUND_RECT; uToolID++)
	{
		m_uToolState[uToolID - ID_NP_TOOL_POLY_CLIP] = TBS_ENABLE;
	}
}

CToolBoxBar::~CToolBoxBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxBar diagnostics

#ifdef _DEBUG
void CToolBoxBar::AssertValid() const
{
	CToolBar::AssertValid();
}

void CToolBoxBar::Dump(CDumpContext& dc) const
{
	CToolBar::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolBoxBar message handlers

void CToolBoxBar::SetColumns(UINT nColumns)
{
	m_nColumns = nColumns;
	DWORD dwStyle = GetBarStyle();
	//CBRS_SIZE_FIXED must be defined in bar style
	ASSERT(dwStyle & CBRS_SIZE_FIXED);
	int nCount = GetToolBarCtrl().GetButtonCount();
	for (int i = 0; i < nCount; i++)
	{
		UINT nStyle = GetButtonStyle(i);
		BOOL bWrap = (((i + 1) % nColumns) == 0);
		if (bWrap)
			nStyle |= TBBS_WRAPPED;
		else
			nStyle &= ~TBBS_WRAPPED;
		SetButtonStyle(i, nStyle);
	}

	Invalidate();
	GetParentFrame()->RecalcLayout();
}

BOOL CToolBoxBar::SetImageList(UINT uImageListType, UINT uBitmapResource, CPoint ptMask, int nImageWidth, int nImageCount)
{
	CImageList	cImageList;
	LoadImageList(&cImageList,uBitmapResource,ptMask,nImageWidth,nImageCount);
	SendMessage(uImageListType, 0, (LPARAM)cImageList.m_hImageList);
	cImageList.Detach(); 	
	return TRUE;
}

int CToolBoxBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rect(2, 176+5, 2+42, 176+5+66);
	CListCtrl* pListCtrl =  &m_listStyle;
	if (!pListCtrl->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSCROLL,
		 rect, this, IDC_LIST_TOOLBOX_STYLE))
	{
		TRACE0("Failed to create style list\n");
		return FALSE;
	}
	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl->SetBkColor(::GetSysColor(COLOR_BTNFACE));
	pListCtrl->InsertColumn(0,_T(""),LVCFMT_CENTER,40);
	pListCtrl->ModifyStyleEx(0,WS_EX_STATICEDGE);

	return 0;
}

void CToolBoxBar::OnUpdateTools(CCmdUI* pCmdUI)
{
	if (pCmdUI->m_nID < ID_NP_TOOL_POLY_CLIP || pCmdUI->m_nID > ID_NP_TOOL_ROUND_RECT)
		return;
	if ((m_uToolState[pCmdUI->m_nID - ID_NP_TOOL_POLY_CLIP] & TBS_ENABLE) != 0)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurSelToolID);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CToolBoxBar::EnableTool(UINT uID, BOOL bEnable)
{
	if (uID < ID_NP_TOOL_POLY_CLIP || uID > ID_NP_TOOL_ROUND_RECT)
		return;
	if (bEnable)
		m_uToolState[uID - ID_NP_TOOL_POLY_CLIP] |= TBS_ENABLE;
	else
		m_uToolState[uID - ID_NP_TOOL_POLY_CLIP] &= ~TBS_ENABLE;
	Invalidate();
}

BOOL CToolBoxBar::IsToolEnabled(UINT uID)
{
	ASSERT(uID >= ID_NP_TOOL_POLY_CLIP && uID <= ID_NP_TOOL_ROUND_RECT);
	return (m_uToolState[uID - ID_NP_TOOL_POLY_CLIP] & TBS_ENABLE) != 0;
}


void CToolBoxBar::OnChangeTool(UINT index, UINT uStyle)
{	
	m_nCurSelToolID = index;
	CListCtrl * pListCtrl =  &m_listStyle;
	pListCtrl->DeleteAllItems();
	CImageList * pImageList = CStyleListCtrl::GetSpecImageList(index);
	if(pImageList != NULL)
	{
		pListCtrl->SetImageList(pImageList,LVSIL_SMALL);
		for (int iItem = 0; iItem < pImageList->GetImageCount(); iItem++)
			pListCtrl->InsertItem(iItem,_T(""),iItem);
		
		((CStyleListCtrl *)pListCtrl)->SetCurSelTool(m_nCurSelToolID, uStyle);
		UINT style = ((CStyleListCtrl *)pListCtrl)->GetCurSelToolStyle();
		pListCtrl->SetItemState(style, LVNI_SELECTED,LVNI_SELECTED);

		//TODO:Set height of item and multi-column
		IMAGEINFO imageInfo;
		pImageList->GetImageInfo(0,&imageInfo);
		UINT uItemHeight = 64 / pImageList->GetImageCount();
		if ((uItemHeight) >= (UINT)(imageInfo.rcImage.bottom - imageInfo.rcImage.top))
		{
			((CStyleListCtrl *)pListCtrl)->SetItemHeight(uItemHeight);
		}
	}
}
