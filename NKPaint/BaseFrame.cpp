// BaseFrame.cpp : implementation file
//

#include "stdafx.h"
#include "BaseFrame.h"
#include "PaintView.h"
#include "TrueColorToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseFrame

IMPLEMENT_DYNCREATE(CBaseFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CBaseFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CBaseFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_NP_VIEW_TOOLBAR_TOOL, OnViewToolbarTool)
	ON_UPDATE_COMMAND_UI(ID_NP_VIEW_TOOLBAR_TOOL, OnUpdateViewToolbarTool)
	ON_COMMAND(ID_NP_VIEW_TOOLBAR_COLOR, OnViewToolbarColor)
	ON_UPDATE_COMMAND_UI(ID_NP_VIEW_TOOLBAR_COLOR, OnUpdateViewToolbarColor)
	ON_COMMAND_RANGE(ID_NP_TOOL_POLY_CLIP, ID_NP_TOOL_ROUND_RECT, OnSelectTool)
	ON_UPDATE_COMMAND_UI(ID_NP_VIEW_SHOW_MAP, OnUpdateViewShowMap)
	ON_COMMAND(ID_NP_VIEW_SHOW_MAP, OnViewShowMap)
	ON_MESSAGE(WM_USER_DRAW_DONE, OnBitmapUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // Default
	ID_SEPARATOR,           // Image process
	ID_SEPARATOR,           // Status
	ID_SEPARATOR,           // Mouse position
	ID_SEPARATOR,           // Zoom rate
};

static UINT BASED_CODE palette[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_NP_TOOL_POLY_CLIP,
	ID_NP_TOOL_SELECT,
	ID_NP_TOOL_ERASER,
	ID_NP_TOOL_FILL,
	ID_NP_TOOL_SUCKER,
	ID_NP_TOOL_ZOOM,
	ID_NP_TOOL_PEN,
	ID_NP_TOOL_BRUSH,
	ID_NP_TOOL_SPRAY,
	ID_NP_TOOL_TEXT,
	ID_NP_TOOL_LINE,
	ID_NP_TOOL_CURVE,
	ID_NP_TOOL_RECTANGLE,
	ID_NP_TOOL_POLYGON,
	ID_NP_TOOL_ELLIPSE,
	ID_NP_TOOL_ROUND_RECT,
	ID_SEPARATOR,          // for combo box (placeholder)
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
};

/////////////////////////////////////////////////////////////////////////////
// CBaseFrame construction/destruction

CBaseFrame::CBaseFrame()
{
	m_hAccel = NULL;
	m_bUsePaintMenu = TRUE;
}

CBaseFrame::~CBaseFrame()
{
}

BOOL CBaseFrame::CreateMenu(UINT nResourceID)
{
	if (m_bUsePaintMenu)
	{
		CMenu* pMenu = GetMenu();
		if (pMenu)
			pMenu->DestroyMenu();

		CMenu menu, * pNewMenu;
		HMENU hNewMenu = LoadMenu(AfxFindResourceHandle(MAKEINTRESOURCE(
			nResourceID), RT_MENU), MAKEINTRESOURCE(nResourceID));
		pNewMenu = menu.FromHandle(hNewMenu);

		// replace with our menu 
		BOOL bSetMenu = SetMenu(pNewMenu);
		ASSERT( GetMenu() == pNewMenu);
	}
	
	return TRUE;
}

BOOL CBaseFrame::PFMergeAccelerator(HACCEL& hDestination, HACCEL hToMerge)
{
	// this function merges the hToMerge accelerator table with that of hDestination
	ASSERT(hToMerge) ;

	int		original_count = 0 ;
	int		add_count ;
	if (hDestination != NULL)
	{
		// we have an existing table
		original_count = CopyAcceleratorTable(hDestination, NULL, 0) ;
 	}
	add_count = CopyAcceleratorTable(hToMerge, NULL, 0) ;
	if (add_count > 0)
	{
		ACCEL	*pElements = new ACCEL[original_count + add_count] ;
		// copy in the existing data if it exists
		if (hDestination != NULL)
			CopyAcceleratorTable(hDestination, pElements, original_count) ;
		// now add in the merge accelerator
		CopyAcceleratorTable(hToMerge, &pElements[original_count], add_count) ;
		HACCEL hNew = CreateAcceleratorTable(pElements, original_count + add_count) ;
		DestroyAcceleratorTable(hDestination) ;			// because it gets replaced
		hDestination = hNew ;
		delete []pElements ;
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseFrame::LoadAccelerators(UINT nResourceID)
{
	HACCEL AcclAppend = ::LoadAccelerators(AfxFindResourceHandle(MAKEINTRESOURCE(
		IDR_MAIN_FRAME), RT_ACCELERATOR), MAKEINTRESOURCE(IDR_MAIN_FRAME));
	PFMergeAccelerator(m_hAccelTable, AcclAppend);

	return m_hAccel != NULL;
}

BOOL CBaseFrame::UseAccelerators()
{
	if(!LoadAccelerators(IDR_MAIN_FRAME))
		return FALSE;
	return TRUE;
}

int CBaseFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!CreateMenu(IDR_MAIN_FRAME))
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_dlgPaintBox.Create(this, IDD_DLG_COLOR,
			CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
			IDC_DLG_BAR_PAINTBOX))
	{
		TRACE0("Failed to create dialog bar m_wndContentMenu\n");
		return -1;		// fail to create
	}
	m_dlgPaintBox.SetWindowText(_T("Color"));

	if (!m_barToolBox.Create(this, WS_CHILD | WS_VISIBLE | CBRS_SIZE_FIXED |
		CBRS_LEFT | CBRS_TOOLTIPS, IDC_DLG_BAR_PALETTE) ||
		!m_barToolBox.SetImageList(TB_SETIMAGELIST, IDB_NP_TOOL_BOX, CPoint(0,0), 16, 16) ||
		!m_barToolBox.SetButtons(palette,
		  sizeof(palette)/sizeof(UINT)))
	{
		TRACE0("Failed to create m_wndToolPaletteBar\n");
		return FALSE;       
	}

 	m_dlgZoomMap.Create(IDD_DLG_MAP);

	m_barToolBox.SetWindowText(_T("Tool"));
	m_barToolBox.SetColumns(2);

	m_barToolBox.EnableDocking(CBRS_ORIENT_VERT);
	m_dlgPaintBox.EnableDocking(CBRS_ORIENT_HORZ);

	EnableTool(ID_NP_TOOL_POLY_CLIP, FALSE);
	EnableTool(ID_NP_TOOL_SELECT, FALSE);
// 	EnableTool(ID_NP_TOOL_ERASER, FALSE);
	EnableTool(ID_NP_TOOL_FILL, FALSE);
	EnableTool(ID_NP_TOOL_SUCKER, FALSE);
// 	EnableTool(ID_NP_TOOL_PEN, FALSE);
// 	EnableTool(ID_NP_TOOL_BRUSH, FALSE);
	EnableTool(ID_NP_TOOL_SPRAY, FALSE);
	EnableTool(ID_NP_TOOL_TEXT, FALSE);
	EnableTool(ID_NP_TOOL_CURVE, FALSE);
	EnableTool(ID_NP_TOOL_POLYGON, FALSE);
// 	EnableTool(ID_NP_TOOL_ELLIPSE, FALSE);
// 	EnableTool(ID_NP_TOOL_ROUND_RECT, FALSE);

	return 0;
}

BOOL CBaseFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBaseFrame message handlers

void CBaseFrame::UpdateBar(CControlBar * pDockedBar)
{
	ASSERT(pDockedBar != NULL);
	CFrameWnd*pFrame=pDockedBar->GetDockingFrame();
	pFrame->RecalcLayout();
}

void CBaseFrame::OnViewToolbarTool()
{
	// TODO: Add your command handler code here
	if(m_barToolBox.IsWindowVisible())
        m_barToolBox.ShowWindow(SW_HIDE);
    else
        m_barToolBox.ShowWindow(SW_SHOW);
	
	UpdateBar(&m_barToolBox);
}

void CBaseFrame::OnUpdateViewToolbarTool(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
 	if (m_barToolBox.IsWindowVisible())
 		pCmdUI->SetCheck(1);
 	else
 		pCmdUI->SetCheck(0);
}

LRESULT CBaseFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CPaintView * pDrawView;
	if (message == WM_SYSCOMMAND)
	{
		switch(wParam)
		{
		case SC_MINIMIZE:
			// TODO: Add your specialized code here to dispatch before minimize
			break;
		case SC_CLOSE:
			// TODO: Add your specialized code here to dispatch before close
			break;
		case SC_MAXIMIZE:
		case 0xF032:
			pDrawView = (CPaintView *)this->GetActiveView();
//			pDrawView->ReSetStartPosition();
			break;
		case SC_RESTORE:
		case 0xF122:
			// TODO: Add your specialized code here to dispatch before restore
			break;
		case 0xF012:
			break;
		default:
			wParam = wParam;
			break;
		}
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CBaseFrame::OnViewToolbarColor() 
{
	if(m_dlgPaintBox.IsWindowVisible())
        m_dlgPaintBox.ShowWindow(SW_HIDE);
    else
        m_dlgPaintBox.ShowWindow(SW_SHOW);

	UpdateBar(&m_dlgPaintBox);
}

void CBaseFrame::OnUpdateViewToolbarColor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
 	if (m_dlgPaintBox.IsWindowVisible())
 		pCmdUI->SetCheck(1);
 	else
 		pCmdUI->SetCheck(0);
}

void CBaseFrame::OnSelectTool(UINT id)
{
	SetCurSelTool(id);
}

void CBaseFrame::SetCurSelTool(UINT uID, UINT uStyle)
{
	m_barToolBox.OnChangeTool(uID, uStyle);
	CPaintView * pDrawView = (CPaintView *)this->GetActiveView();
	ASSERT(pDrawView);
	pDrawView->OnToolChanged(uID);
}

void CBaseFrame::ShowMousePos(CPoint pt)
{
	CString strPos = _T("");
	if (pt.x >=0 && pt.y >= 0)
		strPos.Format(_T("%d,%d"), pt.x, pt.y);
	this->m_wndStatusBar.SetPaneText(2,strPos);
}

void CBaseFrame::OnUpdateViewShowMap(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
 	if (m_dlgZoomMap.IsWindowVisible())
 		pCmdUI->SetCheck(1);
 	else
 		pCmdUI->SetCheck(0);
}

void CBaseFrame::OnViewShowMap() 
{
	if(m_dlgZoomMap.IsWindowVisible())
        m_dlgZoomMap.ShowWindow(SW_HIDE);
    else
        m_dlgZoomMap.ShowWindow(SW_SHOW);
}

LRESULT CBaseFrame::OnBitmapUpdate(WPARAM wParam, LPARAM lParam)
{
	m_dlgZoomMap.Invalidate();
	return 0;
}
