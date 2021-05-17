// [INDEX]
// scaling			[n]		缩放比例
// partial refresh	[n]		局部刷新
// client region	[n]		客户区域
// zoom in			[adv]	放大
// zoom out			[adv]	缩小
// map mode			[n]		映射模式
// twinkling		[adj]	闪烁
// double-buffering [n]		双缓冲
// object			[n]		绘制对象
// graphic element	[n]		图元
// hot point		[n]		热点-画布边缘的标记画布边界的高亮点
// client background[n]		客户区背景(区别于画布的背景)
//
/////////////////////////////////////////////////////////////////////////////

// PaintView.cpp : implementation of the CProjectFrameworkView class
//

#include "stdafx.h"
#include "NKPaint.h"
#include "DrawObject.h"
#include "PaintView.h"
#include <math.h>
#include <atlbase.h>
#include <comdef.h>
#include "structure.h"
#include "Utils.h"
#include "AdvanceDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaintView

IMPLEMENT_DYNCREATE(CPaintView, CView)

BEGIN_MESSAGE_MAP(CPaintView, CADMMScrollView)
	//{{AFX_MSG_MAP(CPaintView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_USER_DRAW_DONE, OnDrawDone)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CADMMScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CADMMScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CADMMScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaintView construction/destruction

CPaintView::CPaintView()
{	
	m_sizeCanvas.cx = 384;
	m_sizeCanvas.cy = 320;

	m_fZoomScale = 1.0f;
	
	m_pCurObj = NULL;
	m_curMode = SM_NONE;
	m_bCurObjectIsActive = FALSE;

	m_uLineWidth = 1;
	m_emBrushStyle = CDrawObject::PenStyle::PS_DOT2;
	m_emShapeStyle = CDrawObject::ShapeStyle::SS_FRAME;
	
	m_nDragHandle = EH_NONE;
	m_eCanvasHandle = EH_NONE;
	m_pEditBox = NULL;

	m_bRecord = TRUE;

	m_pHistoryStack =new CHistoryStack(5);

	m_pClientBkBrush = &g_cbSystemBackground;

	m_strCurFile = _T("untitled");
}

CPaintView::~CPaintView()
{
	if (m_pCurObj)
	{
		delete m_pCurObj;
		m_pCurObj =NULL;
	}
	if (m_pEditBox)
	{
		delete m_pEditBox;
		m_pEditBox = NULL;
	}
	if(m_pHistoryStack)
	{
		m_pHistoryStack->Clear();
		delete m_pHistoryStack;
		m_pHistoryStack = NULL;
	}
}

HBITMAP CPaintView::CanvasToBitmap()
{
	CClientDC dc(this);
	CDrawDC   memDC;
	CBitmap   bitmap;
	CBitmap * pOldBitmap;

	// the size of bitmap to create(not changed when zooming)
	// attention : this size is not the size of the current bitmap,
	//			   but the size of canvas without zooming.
	//			   the difference between them is when draging the edge of
	//			   canvas.
	CSize size = GetBitmapSize();

	// whether the size of the current canvas is changed
	// take care : avoid (image size != (image size * scaling) / scaling) [float operation]
	if ((m_sizeCanvas.cx != (LONG)(size.cx * m_fZoomScale)) || (m_sizeCanvas.cy != (LONG)(size.cy * m_fZoomScale)))
	{
		size.cx = (LONG)(m_sizeCanvas.cx / m_fZoomScale);
		size.cy = (LONG)(m_sizeCanvas.cy / m_fZoomScale);
	}

	// create device(for operating bitmap) and bitmap
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, size.cx,size.cy);

	// draw backgroud bitmap and current dynamic object to bitmap 
	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PrepareDC();
	DoDrawToBitmap(&memDC);
	memDC.SelectObject(pOldBitmap);
	::DeleteDC(memDC);

	// get handle of bitmap
	return (HBITMAP)bitmap.Detach();
}

BOOL CPaintView::SetZoomScale(FLOAT fValue)
{
	// scaling is between 1/PAINT_MAX_ZOOM_SCALE and PAINT_MAX_ZOOM_SCALE
	// we needn't to update when a sequence of the same scaling value is set
	if ((fValue <= PAINT_MAX_ZOOM_SCALE) &&
		(fValue >= (1/PAINT_MAX_ZOOM_SCALE)) &&
		(fabs(m_fZoomScale - fValue) > INFINITESIMAL))
	{
		m_fZoomScale = fValue;
		// update image size and scrollbar
		UpdateViewSize();
		return TRUE;
	}
	return FALSE;
}

SIZE CPaintView::GetBitmapSize()
{
	HBITMAP hBitmap = GetBitmap();
	ASSERT(hBitmap);

	// get bitmap head info
	SIZE sizeBitmap;
	BITMAP bitmap;
	::GetObject(hBitmap,sizeof(bitmap),&bitmap);
	sizeBitmap.cx = bitmap.bmWidth;
	sizeBitmap.cy = bitmap.bmHeight;

	return sizeBitmap;
}

HBITMAP CPaintView::GetBitmap()
{
	// get the handle of the current bitmap in the bitmap list
	// but not a copy of the bitmap
	return m_pHistoryStack->GetCurrent();
}


BOOL CPaintView::SetBitmap(HBITMAP hBitmap)
{
	// valid handle needed
	if (hBitmap)
	{
		// store this bitmap to history and update view
		RecordHistory(hBitmap);
		return TRUE;
	}
	return FALSE;
}

void CPaintView::InitHistory(HBITMAP hBitmap)
{
	ASSERT(hBitmap);
	// clear all bitmap(release all bitmap memories)
	m_pHistoryStack->Clear();
	// add one basic bitmap
	m_pHistoryStack->Add(hBitmap);
	// update wnds
	BitmapChanged(BCT_UPDATE);
}

void CPaintView::RecordHistory(HBITMAP hBitmap)
{
	// valid handle
 	if (hBitmap != NULL)
 	{
 		m_pHistoryStack->Add(hBitmap);
	}
	// update anyway
	BitmapChanged(BCT_UPDATE);
}

void CPaintView::DoDrawToScreen(CDrawDC * pDC)
{
	HDC hMainDC = ::GetDC(NULL);
	HDC hTempDC = ::CreateCompatibleDC(hMainDC);

	CPoint ptLeftTop(0,0), ptOffset(0,0);
	HBITMAP hBitmap		= ::CopyBitmap(GetBitmap());// new bitmap needed
	HBITMAP hOldBitmap	= (HBITMAP)::SelectObject(hTempDC,hBitmap);

	BITMAP bitmap;
	::GetObject(hBitmap,sizeof(bitmap),&bitmap);

	if (hBitmap)
	{
		CSize sizeImage;
		sizeImage.cx = bitmap.bmWidth;
		sizeImage.cy = bitmap.bmHeight;

		// the start of the current bitmap is started from the size of hot point 
		ptLeftTop.x += HOTPOINT_WIDTH;
		ptLeftTop.y += HOTPOINT_HEIGHT;
		// the offset is changed when the scroll bar's position is changed.
		ptOffset = this->GetScrollPosition();
		ptOffset = ptLeftTop - ptOffset;

		// repaint step 1 : erase background
		// filled with background color. screen twinkling problem is avoid
		// by erase background ourself. 
		// buffering technique is used, firstly, draw all object to the temperary
		// bitmap and finally draw the temperary bitmap to the screen device
		//
		DrawBackGround(pDC);
		
		// repaint step 2: draw the current object the current bitmap
		// the oject is must be draw to the current bitmap, otherwise,
		// the oject is hard to suit to the size of the current bitmap when zoomed [in/out]
		CDC * pImgDC = CDC::FromHandle(hTempDC);
		// the scaling is must be set to 1 for the current bitmap
		CUtility::PrepareDC(pImgDC->m_hDC, 1);
		if (m_pCurObj)
		{
			// no offset used
			ptOffset.x = 0;
			ptOffset.y = 0;
			m_pCurObj->DrawSelf((CDrawDC *)pImgDC,ptOffset);
 			if (m_bCurObjectIsActive)
 				m_pCurObj->DrawTracker(pDC, ptOffset);
		}

		// when scaling is larger than 1, advance stretch mode is not needed.
		if (m_fZoomScale < 1.0)
			pDC->SetStretchBltMode(HALFTONE);

		// repaint step 3: draw the current bitmap to screen device
		pDC->StretchBlt(ptLeftTop.x,ptLeftTop.y,m_sizeCanvas.cx,m_sizeCanvas.cy,
			pImgDC, 0, 0, sizeImage.cx, sizeImage.cy, SRCCOPY);

		// FromHanlde create handle, we must delete here, otherwise, GDI handle leaks.
		pImgDC->DeleteDC();
	}
	
	::SelectObject(hTempDC,hOldBitmap);
	::DeleteObject(hBitmap);

	::DeleteDC(hTempDC);
	::ReleaseDC(NULL,hMainDC);
}

void CPaintView::DoDrawToBitmap(CDrawDC *pDC)
{
	//
	// paint mode:
	//   draw object to bitmap - painting when the operation 
	//							 of current dynamic oject is finished.
	//
	HDC hMainDC = ::GetDC(NULL);
	HDC hTempDC = ::CreateCompatibleDC(hMainDC);

	HBITMAP hCurBitmap = GetBitmap();
	HBITMAP hBitmap	   = CopyBitmap(hCurBitmap);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hTempDC,hBitmap);

	BITMAP bitmap;
	::GetObject(hBitmap,sizeof(bitmap),&bitmap);
	CSize sizeBitmap(bitmap.bmWidth, bitmap.bmHeight);

	if (hBitmap)
	{
		CSize sizeCanvas(m_sizeCanvas);
		sizeCanvas.cx = (LONG)(sizeCanvas.cx / m_fZoomScale);
		sizeCanvas.cy = (LONG)(sizeCanvas.cy / m_fZoomScale);

		// filled with color of back ground when canvas is expanded
		if (sizeBitmap.cx < sizeCanvas.cx || sizeBitmap.cy < sizeCanvas.cy)
		{
			CSize sizeReal(sizeBitmap.cx, sizeBitmap.cy);

			CBrush brushBackGround(GetBkColor());
			CRect  rect1(sizeReal.cx, 0, sizeCanvas.cx, sizeCanvas.cy);
			pDC->FillRect(&rect1, &brushBackGround);

			CRect  rect2(0, sizeReal.cy, sizeCanvas.cx, sizeCanvas.cy);
			pDC->FillRect(&rect2, &brushBackGround);
		}
		
		// keep original bitmap
		CDC * pImgDC = CDC::FromHandle(hTempDC);
		pDC->StretchBlt(0,0,sizeBitmap.cx,sizeBitmap.cy,
			pImgDC, 0, 0, sizeBitmap.cx, sizeBitmap.cy, SRCCOPY);
		pImgDC->DeleteDC();
	}

	// draw new object to current canvas
	// attention : real-time drawing object[OnDraw] to screen is different from
	//			   record drawing object[RecordHistory]
	if (m_pCurObj)
	{
		CPoint ptOffset(0,0);
		m_pCurObj->DrawSelf(pDC, ptOffset);
		if (m_bCurObjectIsActive)
			m_pCurObj->DrawTracker(pDC, ptOffset);
	}

	::SelectObject(hTempDC, hOldBitmap);

	::DeleteDC(hTempDC);
	::DeleteObject(hBitmap);
	::ReleaseDC(NULL, hMainDC);
}

void CPaintView::UpdateViewSize()
{
	CSize sizeNew(m_totalLog);
	CSize sizeImage = GetBitmapSize();

	// the size of canvas is changed with scaling 
	// and the real size of the current bitmap
	m_sizeCanvas.cx = (LONG)(sizeImage.cx * m_fZoomScale);
	m_sizeCanvas.cy = (LONG)(sizeImage.cy * m_fZoomScale);

	// the size of scrolling is include the size of canvas and hot points
	sizeNew.cx = (LONG)(m_sizeCanvas.cx + 2 * HOTPOINT_WIDTH);
	sizeNew.cy = (LONG)(m_sizeCanvas.cy + 2 * HOTPOINT_HEIGHT);

	// resize scroll size
	if (sizeNew != m_totalLog)
		this->SetScrollSizes(MM_ISOTROPIC, sizeNew);

	// global update
	this->Invalidate();
}

void CPaintView::ClientToBitmap(LPPOINT pt)
{
	CPoint ptOffset = this->GetScrollPosition();
		
	// convert client coordinate to canvas coordinate
	pt->x -= HOTPOINT_WIDTH;
	pt->y -= HOTPOINT_HEIGHT;

	// convert canvas coorinate to the real bitmap coorinate
	pt->x = (LONG)((pt->x + ptOffset.x) / m_fZoomScale);
	pt->y = (LONG)((pt->y + ptOffset.y) / m_fZoomScale);
}

void CPaintView::BitmapToClient(LPPOINT pt)
{
	CPoint ptOffset = this->GetScrollPosition();
	
	// convert canvas coordinate to client coordinate
	pt->x += HOTPOINT_WIDTH;
	pt->y += HOTPOINT_HEIGHT;
	
	// convert the real bitmap coorinate to canvas coorinate
	pt->x = (LONG)((pt->x - ptOffset.x) * m_fZoomScale);
	pt->y = (LONG)((pt->y - ptOffset.y) * m_fZoomScale);
}

void CPaintView::DrawBackGround(CDrawDC * pDC)
{
	ASSERT(pDC);

	CRect rcClient;
	this->GetClientRect(rcClient);
	if (m_pClientBkBrush == NULL)
	{
		m_pClientBkBrush = &g_cbSystemBackground;
	}

	// just erase client region which need to be erased.
	ASSERT(m_pClientBkBrush);
	pDC->FillRect(&rcClient, m_pClientBkBrush);
}

BOOL CPaintView::IsPointInCanvas(const CPoint & point)
{
	CSize sizeCanvas(m_sizeCanvas);

	// the size of canvas is not changing when zooming
	sizeCanvas.cx = (LONG)(sizeCanvas.cx / m_fZoomScale);
	sizeCanvas.cy = (LONG)(sizeCanvas.cy / m_fZoomScale);

	return (point.x <= sizeCanvas.cx && point.y <= sizeCanvas.cy && point.x >= 0 && point.y >= 0);
}

void CPaintView::SetCurrentObject(CDrawObject * pNewObj)
{
	// delete former object
	if (m_pCurObj != NULL)
	{
		delete m_pCurObj;
	}
	// set the current object
	m_pCurObj = pNewObj;
	if (pNewObj == NULL)
		m_bCurObjectIsActive = FALSE;
}

void CPaintView::InvalidateObject(CDrawObject * pObj)
{
	// valid check
	if (pObj == NULL) return;

	// get the bounding box of this object
	CRect rectBound = pObj->GetBoundRect();
	CSize sizeOffset;
	// get the relative position
	sizeOffset = this->GetScrollPosition();

	// the orininal size of offset
	sizeOffset.cx = (LONG)(sizeOffset.cx / m_fZoomScale);
	sizeOffset.cy = (LONG)(sizeOffset.cy / m_fZoomScale);

	// calculate the actual bounding box
	rectBound -= sizeOffset;
 	rectBound.left	 = (LONG)(rectBound.left   * m_fZoomScale);
 	rectBound.top	 = (LONG)(rectBound.top    * m_fZoomScale);
 	rectBound.right	 = (LONG)(rectBound.right  * m_fZoomScale);
 	rectBound.bottom = (LONG)(rectBound.bottom * m_fZoomScale);

	// the bounding box of canvas
	CRect rectCanvas;
	rectCanvas.left	  = HOTPOINT_WIDTH;
	rectCanvas.top    = HOTPOINT_HEIGHT;
	rectCanvas.right  = rectCanvas.left + m_sizeCanvas.cx;
	rectCanvas.bottom = rectCanvas.top  + m_sizeCanvas.cy;

	// put RECT right
	rectBound.NormalizeRect();
	// enlarge a bit invalidate area
	rectBound.InflateRect(HOTPOINT_WIDTH * 4, HOTPOINT_HEIGHT * 4);
	if (rectBound.IntersectRect(rectBound,rectCanvas))
	{		
		this->InvalidateRect(rectBound,FALSE);
	}
}

EdgeHandle CPaintView::HitCanvasTest(CPoint point)
{
	// get the runtime rectangle of canvas
	CSize sizeCanvas(m_sizeCanvas);
	sizeCanvas.cx = (LONG)(sizeCanvas.cx / m_fZoomScale);
	sizeCanvas.cy = (LONG)(sizeCanvas.cy / m_fZoomScale);
	CRect rtRect(0, 0, sizeCanvas.cx, sizeCanvas.cy);

	// hit test
	return CDrawHandle::HitTest(rtRect, point);
}

void CPaintView::DrawFocusRect(const CPoint& ptLeftTop, const CPoint& ptBottomRight)
{
	CRect rect(ptLeftTop,ptBottomRight);
	rect.NormalizeRect();

	CClientDC dc(this);
	dc.DrawFocusRect(rect);
}

void CPaintView::OnToolChanged(UINT uCurSelTool)
{
	// initialize paint mode
	m_curMode = SM_NONE;
	// edit box should be hiden
	HidenEditBox();
	// no object is active when tool is changed
	m_bCurObjectIsActive = FALSE;
	if (m_pCurObj != NULL)
	{
		this->InvalidateObject(m_pCurObj);
	}
}

void CPaintView::OnToolStyleChanged(UINT uCurSelTool, UINT uCurSelToolStyle)
{
	// the current selected tool must be the same
 	CBaseFrame * pFrame = (CBaseFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	ASSERT(pFrame->GetCurSelTool() == uCurSelTool);

	switch (uCurSelTool)
	{
		case ID_NP_TOOL_ZOOM:
		{			
			switch (uCurSelToolStyle)
			{
			case 0:
				SetZoomScale(1);
				break;
			case 1:
				SetZoomScale(2);
				break;
			case 2:
				SetZoomScale(6);
				break;
			case 3:
				SetZoomScale(8);
				break;
			default:
				break;
			}
			break;
		}
		case ID_NP_TOOL_BRUSH:
		{
			switch (uCurSelToolStyle)
			{
			case 0:
				m_emBrushStyle = CDrawObject::PenStyle::PS_RECT1;
				break;
			case 1:
				m_emBrushStyle = CDrawObject::PenStyle::PS_RECT2;
				break;
			case 2:
				m_emBrushStyle = CDrawObject::PenStyle::PS_RECT3;
				break;
			case 3:
				m_emBrushStyle = CDrawObject::PenStyle::PS_DOT2;
				break;
			case 4:
				m_emBrushStyle = CDrawObject::PenStyle::PS_DOT3;
				break;
			default:
				m_emBrushStyle = CDrawObject::PenStyle::PS_DOT1;
				break;
			}
			break;
		}
		case ID_NP_TOOL_ERASER:
		{
			switch (uCurSelToolStyle)
			{
			case 0:
				m_emBrushStyle = CDrawObject::PenStyle::PS_RECT1;
				break;
			case 1:
				m_emBrushStyle = CDrawObject::PenStyle::PS_RECT2;
				break;
			case 2:
				m_emBrushStyle = CDrawObject::PenStyle::PS_RECT3;
				break;
			case 3:
				m_emBrushStyle = CDrawObject::PenStyle::PS_RECT4;
				break;
			default:
				m_emBrushStyle = CDrawObject::PenStyle::PS_RECT1;
				break;
			}
			break;
		}
		case ID_NP_TOOL_LINE:
		{
			switch (uCurSelToolStyle)
			{
			case 0:
				m_uLineWidth = 1;
				break;
			case 1:
				m_uLineWidth = 2;
				break;
			case 2:
				m_uLineWidth = 3;
				break;
			case 3:
				m_uLineWidth = 4;
				break;
			case 4:
				m_uLineWidth = 5;
				break;
			default:
				break;
			}
			break;
		}
		case ID_NP_TOOL_RECTANGLE:
		case ID_NP_TOOL_POLYGON:
		case ID_NP_TOOL_ELLIPSE:
		case ID_NP_TOOL_ROUND_RECT:
		{
			switch (uCurSelToolStyle)
			{
			case 0:
				m_emShapeStyle = CDrawObject::ShapeStyle::SS_FRAME;
				break;
			case 1:
				m_emShapeStyle = CDrawObject::ShapeStyle::SS_FRAME_FILL;
				break;
			case 2:
				m_emShapeStyle = CDrawObject::ShapeStyle::SS_FILL;
				break;
			default:
				break;
			}
			break;
		}
		default:
			break;
	}

}

BOOL CPaintView::SaveImage(LPCTSTR lpFileName)
{
	HBITMAP hBitmap = GetBitmap();

	GetDocument()->SetModifiedFlag(FALSE);
	// set the current bitmap to file
	CImage imgCurrent;
	imgCurrent.CreateFromHBITMAP(hBitmap);
	HRESULT hResult = imgCurrent.Save(lpFileName);
	if (FAILED(hResult)) 
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CPaintView::OpenImage(LPCTSTR lpFileName)
{
	CImage imgCurrent;

	// load image by file name
	HRESULT hResult = imgCurrent.Load(lpFileName);
	if (FAILED(hResult))
	{
		return FALSE;
	}

	m_strCurFile = lpFileName;
	// initialize history stack
	InitHistory(imgCurrent.Detach());

	return TRUE;
}

LRESULT CPaintView::OnDrawDone(WPARAM wParam, LPARAM lParam)
{
	// get new bitmap which contains the backgroud bitmap 
	// and the current active object
	HBITMAP hBitmap = CanvasToBitmap();
	// whether record or not
	if (m_bRecord)
	{
		RecordHistory(hBitmap);
	}
	else
	{
		RecordHistory();
	}
	// the current object is not needed anymore
	if (m_pCurObj != NULL)
	{
 		delete m_pCurObj;
		m_pCurObj = NULL;
	}
	GetDocument()->SetModifiedFlag(TRUE);
	return 0;
}

void CPaintView::ShowEditBox(const CRect& rect, const CString& strText)
{
	CRect rectClient(rect);
//	this->DocToClient(rectClient);
	if (m_pEditBox==NULL)
	{
		m_pEditBox = new CEdit;
		m_pEditBox->Create(ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER,rectClient, this, 1);
		m_fontEditBox.CreateFont(-1 * 12, 0, 0, 0, 20, 
						FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS, _T("Arial")); 
		m_pEditBox->SetFont(&m_fontEditBox);
	}
	else
	{
//		m_pEditBox->SetFont(&GetColorBar()->m_font);
		m_pEditBox->MoveWindow(rectClient);						
		m_pEditBox->ModifyStyle(NULL,WS_VISIBLE);
	}
	m_pEditBox->SetWindowText(strText);	
	int len = strText.GetLength();
	m_pEditBox->SetSel(len, len);
}

void CPaintView::OnDraw(CDC * pDC)
{
	//
	// the core of partial refresh technology is getting the partial
	// region which need to be update, but not the whole canvas.
	// this may save a lot of time.
	// pay attention to the logic coordinate and device coordinate, that
	// because of mapping mode.
	// 

	// real-time draw step 1: get region which need to be update
	CRect rcClientDevice, rcClientLogic;
	this->GetClientRect(rcClientDevice);
	rcClientLogic = rcClientDevice;
	// pDC has been set map mode on initialization
	pDC->DPtoLP(&rcClientLogic);

	// real-time draw step 2: create device and set map mode
	CDrawDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	dcMem.PrepareDC(m_fZoomScale);

	// real-time draw step 3: create one bitmap as a buffer
	HBITMAP hBitmap, hOldBitmap;
	hBitmap = (HBITMAP)::CreateCompatibleBitmap(pDC->m_hDC, 
		rcClientDevice.Width(), rcClientDevice.Height());
	hOldBitmap = (HBITMAP)::SelectObject(dcMem.m_hDC, hBitmap);
	
	// real-time draw step 4: set viewport's offset
	// when scroll bar is moved, the viewport is changed. but device
	// coordinate is not changed. so we should use logic coordinate
	// to set viewport's offset
	CPoint ptTopLeft(rcClientLogic.left,rcClientLogic.top);
	dcMem.OffsetViewportOrg(-ptTopLeft.x,-ptTopLeft.y);

	// real-time draw step 5: draw the current bitmap and the active 
	// object to the bitmap buffer
	DoDrawToScreen(&dcMem);

	// real-time draw step 6: draw the buffer to the screen device
	DrawHotPoint(&dcMem,rcClientDevice.left,rcClientDevice.top,m_sizeCanvas.cx,m_sizeCanvas.cy,HOTPOINT_WIDTH,HOTPOINT_HEIGHT,m_pClientBkBrush,&g_cbSystemWhite,FALSE);
	pDC->BitBlt(rcClientLogic.left,rcClientLogic.top,rcClientLogic.Width(),rcClientLogic.Height(),&dcMem,rcClientLogic.left,rcClientLogic.top,SRCCOPY);
	
	::SelectObject(dcMem.m_hDC, hOldBitmap);
	::DeleteObject(hBitmap);
	dcMem.DeleteDC();
}

void CPaintView::OnInitialUpdate()
{
	CADMMScrollView::OnInitialUpdate();

	// create default bitmap and initialize history bitmap stack
	HBITMAP hBitmap;
	hBitmap = CreatBitmap(NULL, m_sizeCanvas.cx, m_sizeCanvas.cy, COLOR_SYSTEM_WHITE);
	InitHistory(hBitmap);
}

BOOL CPaintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CPaintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CPaintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
CPaintDoc* CPaintView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPaintDoc)));
	return (CPaintDoc*)m_pDocument;
}
#endif

void CPaintView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CADMMScrollView::OnPrepareDC(pDC, pInfo);
	// set viewport
	pDC->SetViewportExt((int)(m_XLogPix*m_fZoomScale),(int)(m_YLogPix*m_fZoomScale));	
}

void CPaintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// convert coordinate from client to real bitmap
	ClientToBitmap(&point);

	// SetCapture and ReleaseCapture make the cursor is not inside 
	// this program's window(the other program's window), the OnMouseMove 
	// is still to respond
	this->SetCapture();

	// get device and set map mode
	CClientDC dc(this);
	OnPrepareDC(&dc);

	// hit canvas test for drag the hot point of canvas
	m_eCanvasHandle = HitCanvasTest(point);
	if (((m_eCanvasHandle == EH_MIDDLE_RIGHT) ||
		 (m_eCanvasHandle == EH_BOTTOM_RIGHT) ||
		 (m_eCanvasHandle == EH_BOTTOM_MIDDLE))	&& 
		 m_nDragHandle <= 6)
	{
		m_curMode = SM_SIZE_CANVAS;						
		SetCursor(GetHandleCursor(m_eCanvasHandle));
		return;
	}

	// don't handle anything when cursor is not in canvas
	if (!IsPointInCanvas(point)) return;

	// take different action according to the current selected tool
	CBaseFrame * pFrame = (CBaseFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	switch (pFrame->GetCurSelTool())
	{
		case ID_NP_TOOL_PEN:
		{
			CDrawPen * pDrawObj = new CDrawPen();
			pDrawObj->SetPenStyle(GetFgColor(),CDrawPen::PenStyle::PS_DOT1,m_fZoomScale);
			pDrawObj->AddPoint(point);			
			this->SetCurrentObject(pDrawObj);
			m_curMode = SM_DRAW_PEN;
			m_ptMove = point;   
			break;
		}
		case ID_NP_TOOL_BRUSH:
		{
			CDrawPen* pDrawObj = new CDrawPen();			
			pDrawObj->SetPenStyle(GetFgColor(),m_emBrushStyle,m_fZoomScale);
			pDrawObj->AddPoint(point);			
			SetCurrentObject(pDrawObj);
			m_curMode = SM_DRAW_PEN;
			m_ptMove = point;   
			break;
		}
		case ID_NP_TOOL_ERASER:
		{
			CDrawPen* pDrawObj = new CDrawPen();
			pDrawObj->SetPenStyle(GetBkColor(),m_emBrushStyle,m_fZoomScale);
			pDrawObj->AddPoint(point);			
			SetCurrentObject(pDrawObj);
			m_curMode = SM_DRAW_PEN;
			m_ptMove = point;   
			break;
		}
		case ID_NP_TOOL_LINE:
		{
			CDrawShape* pDrawObj = new CDrawShape(CDrawShape::ShapeType::ST_LINE);
			pDrawObj->SetShapeStyle(GetFgColor(),GetBkColor(), m_uLineWidth, m_emShapeStyle, m_fZoomScale);
			SetCurrentObject(pDrawObj);
			m_curMode = SM_DRAW_SHAPE;
			break;
		}
		case ID_NP_TOOL_RECTANGLE:
		{
			CDrawShape* pDrawObj = new CDrawShape(CDrawShape::ShapeType::ST_RECT);
			pDrawObj->SetShapeStyle(GetFgColor(),GetBkColor(), m_uLineWidth, m_emShapeStyle, m_fZoomScale);
			SetCurrentObject(pDrawObj);
			m_curMode = SM_DRAW_SHAPE;
			break;
		}
		case ID_NP_TOOL_ELLIPSE:
		{
			CDrawShape* pDrawObj = new CDrawShape(CDrawShape::ShapeType::ST_ELLIPSE);
			pDrawObj->SetShapeStyle(GetFgColor(),GetBkColor(), m_uLineWidth, m_emShapeStyle, m_fZoomScale);
			SetCurrentObject(pDrawObj);
			m_curMode = SM_DRAW_SHAPE;
			break;
		}
		case ID_NP_TOOL_ROUND_RECT:
		{
			CDrawShape* pDrawObj = new CDrawShape(CDrawShape::ShapeType::ST_ROUND_RECT);
			pDrawObj->SetShapeStyle(GetFgColor(),GetBkColor(),m_uLineWidth, m_emShapeStyle, m_fZoomScale);
			SetCurrentObject(pDrawObj);
			m_curMode = SM_DRAW_SHAPE;
			break;
		}
		case ID_NP_TOOL_SELECT:
		{
			if(m_curMode == SM_NONE)
			{
				CDrawRegion* pDrawObj = new CDrawRegion(GetBkColor());
				SetCurrentObject(pDrawObj);
				m_curMode = SM_DRAW_REGION;
			}
			else if (m_curMode == SM_SELECT)
			{
				CDrawRegion* pAreaObj = reinterpret_cast<CDrawRegion*>(m_pCurObj);
				if (pAreaObj)
				{
					m_nDragHandle = pAreaObj->HitObjectTest(point);
					if (m_nDragHandle>0)
					{
						m_curMode = SM_SIZE;						
						SetCursor(pAreaObj->GetHandleCursor(m_nDragHandle));
					}
					else if (pAreaObj->GetBoundRect().PtInRect(point))
					{
						m_curMode = SM_MOVE;
					}
					else
					{
						m_bCurObjectIsActive = FALSE;
						this->InvalidateObject(pAreaObj);
						if (pAreaObj->IsMoved())
						{
							SendMessage(WM_USER_DRAW_DONE);
						}

						//m_curMode = SM_NONE;
						CDrawRegion* pDrawObj = new CDrawRegion(GetBkColor());
						SetCurrentObject(pDrawObj);
						m_curMode = SM_DRAW_REGION;
					}
				}
			}
			break;
		}
		case ID_NP_TOOL_TEXT:
		{
			
			if(m_curMode == SM_NONE)
			{

				CDrawText* pDrawObj = new CDrawText();
				//LOGFONT lf;
				//GetColorBar()->m_font.GetLogFont(&lf);
				CSize size(1,1);
				//lf.lfHeight = lf.lfHeight*size.cx;
				//pDrawObj->SetTextStyle(lf,GetColorBar()->m_colorText);
				SetCurrentObject(pDrawObj);
				m_curMode = SM_DRAW_TEXT;
			}
			else if(m_curMode == SM_EDIT)
			{
				CDrawText* pTextObj = reinterpret_cast<CDrawText*>(m_pCurObj);
				if (!m_pCurObj->GetBoundRect().PtInRect(point))
				{
					HidenEditBox();
					m_bCurObjectIsActive = TRUE;
					this->InvalidateObject(pTextObj);					
					m_curMode = SM_SELECT;
				}

			}
			else if (m_curMode == SM_SELECT)
			{
				m_nDragHandle = m_pCurObj->HitObjectTest(point);
				if (m_nDragHandle>0)
				{
					m_curMode = SM_SIZE;						
					SetCursor(m_pCurObj->GetHandleCursor(m_nDragHandle));
				}
				else if (m_pCurObj->GetBoundRect().PtInRect(point))
				{
					m_curMode = SM_MOVE;
				}
				else
				{
					m_bCurObjectIsActive = FALSE;
					this->InvalidateObject(m_pCurObj);
					SendMessage(WM_USER_DRAW_DONE);
					m_curMode = SM_NONE;
				}
			}
			break;
		}
		case ID_NP_TOOL_FILL:
		{
			CBrush brush;
			CBrush *pOldBrush = dc.SelectObject(&brush);
			dc.FloodFill(point.x, point.y, GetFgColor());
			dc.SelectObject(pOldBrush);
			SendMessage(WM_USER_DRAW_DONE);
			m_curMode = SM_NONE;
			break;
		}
		default:
		{
			m_curMode = SM_NONE;
			break;
		}
	}

	// store Left-Top coordinate and Moving coordinate
	m_ptMove = point; 
	m_ptStart = point;
}

void CPaintView::OnMouseMove(UINT nFlags, CPoint point)
{
	// convert coordinate from client to real bitmap
	ClientToBitmap(&point);

	// get device and set map mode
	CClientDC dc(this);
	OnPrepareDC(&dc);

	// hit canvas test for drag the hot point of canvas
	EdgeHandle eHandle = HitCanvasTest(point);
	if (eHandle == EH_MIDDLE_RIGHT ||
		eHandle == EH_BOTTOM_RIGHT ||
		eHandle == EH_BOTTOM_MIDDLE)
	{
		if ((nFlags & MK_LBUTTON) == 0)
		{
			SetCursor(GetHandleCursor(eHandle));
			if (SM_NONE == m_curMode)
				return;
		}
	}

	// show the mouse position when moving
	if (!IsPointInCanvas(point))
	{
		if ((nFlags & MK_LBUTTON) == 0)
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		// show nothing(erase the last coordinate)
		((CBaseFrame*)this->GetParentFrame())->ShowMousePos(CPoint(-1,-1));	
	}
	else
	{
		((CBaseFrame*)this->GetParentFrame())->ShowMousePos(point);	
	}

	// when the current object is active, hit test needed
	if (m_bCurObjectIsActive)
	{
		EdgeHandle nDragHandle = m_pCurObj->HitObjectTest(point);
		if (nDragHandle > 0)// hit the hot points
		{
			SetCursor(m_pCurObj->GetHandleCursor(nDragHandle));
		}
		else if (m_pCurObj->GetBoundRect().PtInRect(point))// hit the active object
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		}
	}

	// just return when nothing to be draw
	if (SM_NONE == m_curMode) return;
	
	switch (m_curMode)
	{
		case SM_DRAW_PEN:
		{
			CDrawPen* pPenObj = reinterpret_cast<CDrawPen*>(m_pCurObj);
			if (pPenObj != NULL)
			{
				this->InvalidateObject(pPenObj);
				pPenObj->AddPoint(point);			
				pPenObj->AdjustBounds();
				this->InvalidateObject(pPenObj);
			}
			break;
		}	
		case SM_DRAW_SHAPE:
		{
			CDrawShape* pShapeObj = reinterpret_cast<CDrawShape*>(m_pCurObj);
			if (pShapeObj)
			{
				this->InvalidateObject(pShapeObj);
				pShapeObj->SetBounds(m_ptStart,point);
				this->InvalidateObject(pShapeObj);
			}
			break;
		}
		case SM_DRAW_REGION:
		{
			DrawFocusRect(m_ptStart,m_ptMove);
			DrawFocusRect(m_ptStart,point);
			break;
		}
		case SM_DRAW_TEXT:
		{
			DrawFocusRect(m_ptStart,m_ptMove);
			DrawFocusRect(m_ptStart,point);
			break;
		}
		case SM_MOVE:
		{
			if (m_bCurObjectIsActive)
			{
				this->InvalidateObject(m_pCurObj);
				CPoint delta = point - m_ptMove;
				m_pCurObj->OffsetBounds(delta);
				this->InvalidateObject(m_pCurObj);
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			}
			break;
		}
		case SM_SIZE:
		{
			if (m_bCurObjectIsActive)
			{
				this->InvalidateObject(m_pCurObj);
				m_pCurObj->MoveHandleTo(m_nDragHandle, point);
				this->InvalidateObject(m_pCurObj);
				SetCursor(m_pCurObj->GetHandleCursor(m_nDragHandle));
			}
			break;
		}
		case SM_SIZE_CANVAS:
		{
			SetCursor(GetHandleCursor(m_eCanvasHandle));
			switch (m_eCanvasHandle)
			{
				case EH_MIDDLE_RIGHT:
				{
					DrawFocusRect(CPoint(0,0),CPoint(m_ptMove.x, m_sizeCanvas.cy));
					DrawFocusRect(CPoint(0,0),CPoint(point.x, m_sizeCanvas.cy));
					break;
				}
				case EH_BOTTOM_RIGHT:
				{
					DrawFocusRect(CPoint(0,0),m_ptMove);
					DrawFocusRect(CPoint(0,0),point);
					break;
				}
				case EH_BOTTOM_MIDDLE:
				{
					DrawFocusRect(CPoint(0,0),CPoint(m_sizeCanvas.cx, m_ptMove.y));
					DrawFocusRect(CPoint(0,0),CPoint(m_sizeCanvas.cx, point.y));
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}
	
	// store moving coordinate
	m_ptMove = point;
}

void CPaintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// convert coordinate from client to real bitmap
	ClientToBitmap(&point);

	// SetCapture and ReleaseCapture make the cursor is not inside 
	// this program's window(the other program's window), the OnMouseMove 
	// is still to respond
	ReleaseCapture();

	// get device and set map mode
	CClientDC dc(this);	
	OnPrepareDC(&dc);

	switch (m_curMode)
	{
		case SM_DRAW_PEN:
		{
			CDrawPen* pPenObj = reinterpret_cast<CDrawPen*>(m_pCurObj);
			if (IsPointInCanvas(m_ptMove))
			{
				pPenObj->AddPoint(point);
			}

			if (pPenObj->GetPointCount() == 0)
			{
				delete m_pCurObj;
				m_pCurObj = NULL;
			}
			else
			{
				pPenObj->AdjustBounds();
				this->InvalidateObject(pPenObj);
				SendMessage(WM_USER_DRAW_DONE);
			}
			m_curMode = SM_NONE;
			break;
		}
		case SM_DRAW_SHAPE:
		{
			CDrawShape* pShapeObj = reinterpret_cast<CDrawShape*>(m_pCurObj);
			pShapeObj->SetBounds(m_ptStart,point);
			this->InvalidateObject(pShapeObj);
			SendMessage(WM_USER_DRAW_DONE);
			m_curMode = SM_NONE;	
			break;
		}
		case SM_DRAW_REGION:
		{
			CDrawRegion * pAreaObj = reinterpret_cast<CDrawRegion*>(m_pCurObj);
			
			CPoint ptOffset(HOTPOINT_WIDTH, HOTPOINT_HEIGHT);
			m_ptStart -= ptOffset;
			point    -= ptOffset;
			CRect rect(m_ptStart,point);
			rect.NormalizeRect();		
			CSize size(int(rect.Width()/m_fZoomScale), int(rect.Height()/m_fZoomScale));
			CPoint ptTopLeft(int(rect.left/m_fZoomScale), int(rect.top/m_fZoomScale));
			
			CPoint ptScroll = this->GetScrollPosition();
			ptTopLeft += ptScroll;

			HBITMAP      hBmp;
			CClientDC	dc(this);
			CDrawDC		memDC;
			CBitmap     bitmap;

			memDC.CreateCompatibleDC(&dc);
			bitmap.CreateCompatibleBitmap(&dc, size.cx, size.cy);
			CBitmap *pOldBitmap = memDC.SelectObject(&bitmap);

			CImage imgCurrent;
			imgCurrent.CreateFromHBITMAP(GetBitmap());
			CDC * pImgDC = CDC::FromHandle(imgCurrent.GetDC());
			memDC.StretchBlt(0,0,size.cx, size.cy,pImgDC,
				ptTopLeft.x,ptTopLeft.y, size.cx, size.cy, SRCCOPY);
			imgCurrent.ReleaseDC();
			
			hBmp=(HBITMAP)bitmap.GetSafeHandle();
			memDC.SelectObject(pOldBitmap);
			memDC.DeleteDC();
			bitmap.Detach();

			if (hBmp)
			{
				pAreaObj->SetOrigArea(m_ptStart,point,hBmp);
			}

			m_bCurObjectIsActive = TRUE;
			this->InvalidateObject(pAreaObj);
			m_curMode = SM_SELECT;
			break;
		}
		case SM_DRAW_TEXT:
		{
			CRect rect(m_ptStart,point);
			rect.NormalizeRect();
			m_pCurObj->SetBounds(m_ptStart,point);
			this->ShowEditBox(rect);
			m_curMode = SM_EDIT;
			break;
		}
		case SM_MOVE:
		case SM_SIZE:
		{
			m_curMode = SM_SELECT;
			break;
		}
		case SM_SIZE_CANVAS:
		{
			switch (m_eCanvasHandle)
			{
				case EH_MIDDLE_RIGHT:
				{
					m_sizeCanvas.cx = point.x;
					break;
				}
				case EH_BOTTOM_RIGHT:
				{
					m_sizeCanvas.cx = point.x;
					m_sizeCanvas.cy = point.y;
					break;
				}
				case EH_BOTTOM_MIDDLE:
				{
					m_sizeCanvas.cy = point.y;
					break;
				}
				default:
					break;
			}
			this->Invalidate();
			SendMessage(WM_USER_DRAW_DONE);
			m_curMode = SM_NONE;
			break;
		}
	}
}

void CPaintView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// convert coordinate from client to real bitmap
	ClientToBitmap(&point);
	
	CDrawText* pTextObj = reinterpret_cast<CDrawText*>(m_pCurObj);
	if (pTextObj != NULL && pTextObj->GetBoundRect().PtInRect(point))
	{
		this->ShowEditBox(pTextObj->GetBoundRect(),pTextObj->GetText());
		m_bCurObjectIsActive = FALSE;
		m_curMode = SM_EDIT;
	}
}

void CPaintView::HidenEditBox(void)
{
	if (m_pEditBox &&(m_pEditBox->GetStyle() & WS_VISIBLE) != 0) 
	{
		m_pEditBox->ModifyStyle(WS_VISIBLE,NULL);
		CString strTemp;
		m_pEditBox->GetWindowText(strTemp);
		CDrawText* pTextObj = reinterpret_cast<CDrawText*>(m_pCurObj);
		if (pTextObj!=NULL)
		{
			pTextObj->SetText(strTemp);
		}
	}
}

void CPaintView::OnDelete()
{
	if (m_bCurObjectIsActive)
	{
		CDrawRegion* pAreaObj = reinterpret_cast<CDrawRegion*>(m_pCurObj);
		if(pAreaObj)
		{
			pAreaObj->DestroyImage();
			m_bCurObjectIsActive = FALSE;
			this->InvalidateObject(pAreaObj);
			RecordHistory();
			m_curMode = SM_NONE;
		}
	}
}

BOOL CPaintView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest != HTCLIENT)
	{
		return CADMMScrollView::OnSetCursor(pWnd, nHitTest, message);
	}
	CBaseFrame * pFrame = (CBaseFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	UINT uCurSelToolStyle = pFrame->GetCurSelToolStyle();
	switch (pFrame->GetCurSelTool())
	{
		case ID_NP_TOOL_SELECT:
		case ID_NP_TOOL_POLY_CLIP:
		case ID_NP_TOOL_TEXT:
		case ID_NP_TOOL_LINE:
		case ID_NP_TOOL_CURVE:
		case ID_NP_TOOL_RECTANGLE:
		case ID_NP_TOOL_POLYGON:
		case ID_NP_TOOL_ELLIPSE:
		case ID_NP_TOOL_ROUND_RECT:
			SetCursor(AfxGetApp()->LoadCursor(IDC_NP_CURSOR_CROSS_PEN));
			break;
		case ID_NP_TOOL_ERASER:
			{
				HCURSOR hCursor = NULL;
				COLORREF colorBk = GetBkColor();
				switch (uCurSelToolStyle)
				{
					case 0:
						hCursor = CreateEraserCursor(0,0,4,4,32,32,colorBk);
						break;
					case 1:
						hCursor = CreateEraserCursor(0,0,6,6,32,32,colorBk);
						break;
					case 2:
						hCursor = CreateEraserCursor(0,0,8,8,32,32,colorBk);
						break;
					case 3:
						hCursor = CreateEraserCursor(0,0,10,10,32,32,colorBk);
						break;
					default:
						break;
				}
				SetCursor(hCursor);
				break;
			}
		case ID_NP_TOOL_FILL:
			SetCursor(AfxGetApp()->LoadCursor(IDC_NP_CURSOR_FILL));
			break;
		case ID_NP_TOOL_SUCKER:
			SetCursor(AfxGetApp()->LoadCursor(IDC_NP_CURSOR_SUCKER));
			break;
		case ID_NP_TOOL_ZOOM:
			SetCursor(AfxGetApp()->LoadCursor(IDC_NP_CURSOR_ZOOM));
			break;
		case ID_NP_TOOL_PEN:
			SetCursor(AfxGetApp()->LoadCursor(IDC_NP_CURSOR_PEN));
			break;
		case ID_NP_TOOL_BRUSH:
			{
				HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_NP_CURSOR_CROSS_BRUSH);
				switch (uCurSelToolStyle)
				{
					case 0:
						break;
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						break;
					default:
						break;
				}
				SetCursor(hCursor);	
				break;
			}
		case ID_NP_TOOL_SPRAY:
			SetCursor(AfxGetApp()->LoadCursor(IDC_NP_CURSOR_SPRAY));
			break;
		default:
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			break;
	}
	return TRUE;
}

BOOL CPaintView::OnEraseBkgnd(CDC* pDC) 
{
	// Attention : directly return TRUE means background erasing is not need anymore
	// and erase background ourself.
	// if we call the OnEraseBkgnd of the base class, the screen will be twinkling
	return TRUE;
}

void CPaintView::BitmapChanged(BitmapChangedType eType)
{
	// update this view
	switch (eType)
	{
	case BCT_UPDATE:
	case BCT_INIT:
		{
			HBITMAP hBitmap = GetBitmap();
			if (hBitmap != NULL)	// update view
			{
				BITMAP bitmap;
				::GetObject(hBitmap,sizeof(bitmap),&bitmap);
				m_sizeCanvas.cx = bitmap.bmWidth;
				m_sizeCanvas.cy = bitmap.bmHeight;
				UpdateViewSize();
				Invalidate();
			}
			if (eType == BCT_INIT)	// hide current object
			{
				m_curMode = SM_NONE;
				HidenEditBox();
				m_bCurObjectIsActive = FALSE;
				SetCurrentObject(NULL);
			}
		}
		break;
	default:
		break;
	}

	// update for other wnd
	((CBaseFrame*)AfxGetMainWnd())->SendMessage(WM_USER_DRAW_DONE, 0, 0);
}

void CPaintView::OnEditUndo()
{
	if (m_pHistoryStack->CanUndo())
	{
		m_pHistoryStack->Undo();
		BitmapChanged(BCT_INIT);
	}
}

void CPaintView::OnEditRedo()
{
	if (m_pHistoryStack->CanRedo())
	{
		m_pHistoryStack->Redo();
		BitmapChanged(BCT_INIT);
	}

}

void CPaintView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pHistoryStack->CanUndo());
}

void CPaintView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pHistoryStack->CanRedo());
}

#define GET_INT_VALUE(fValue) (int)((fValue)>0.0 ? floor((fValue) + 0.5) : ceill((fValue)-0.5))
void CPaintView::AutoZoom()
{
	CSize szImage = GetBitmapSize();
	CRect rcClient;
	this->GetClientRect(&rcClient);
	double fZoom = 1.0;

	if (szImage.cx > rcClient.Width() || szImage.cy > rcClient.Height())
	{
		double fRateX = (double)szImage.cx / rcClient.Width();
		double fRateY = (double)szImage.cy / rcClient.Height();
		int nX = GET_INT_VALUE(fRateX);
		int nY = GET_INT_VALUE(fRateY);
		if (nX % 2 == 1) nX = nX + 1;
		if (nY % 2 == 1) nY = nY + 1;
		fZoom = (nX > nY) ? nX : nY;
		if (fZoom > 0)
			SetZoomScale((float)(1/fZoom));
		else
			SetZoomScale(1);
	}
	else
	{
		SetZoomScale(1);
	}
}
