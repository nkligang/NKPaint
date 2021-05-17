#include "StdAfx.h"
#include "DrawObject.h"

CDrawDC::CDrawDC() : CDC()
{

}

BOOL CDrawDC::BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop)
{
	CRect rect(x,y,nWidth,nHeight);
	CPoint pt(xSrc,ySrc);
	this->DPtoLP(&rect);
	pSrcDC->DPtoLP(&pt);
	return CDC::BitBlt(rect.left, rect.top, rect.right, rect.bottom, pSrcDC, pt.x, pt.y, dwRop);
}

BOOL CDrawDC::StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop)
{
	CRect rectDest(x,y,nWidth,nHeight),rectSrc(xSrc,ySrc,nSrcWidth,nSrcHeight);
	CRect rcDest(0,0,nWidth,nHeight);
	this->DPtoLP(&rcDest);
	this->DPtoLP(&rectDest);
	pSrcDC->DPtoLP(&rectSrc);
	return 	CDC::StretchBlt(
							rectDest.left - rcDest.left, 
							rectDest.top - rcDest.top, 
							rcDest.right - rcDest.left, 
							rcDest.bottom - rcDest.top, 
							pSrcDC, 
							rectSrc.left, 
							rectSrc.top, 
							rectSrc.right, 
							rectSrc.bottom, 
							dwRop
							);
}

void CDrawDC::FillRect(LPRECT lpRect, CBrush* pBrush)
{
	this->DPtoLP(lpRect);
	CDC::FillRect(lpRect, pBrush);
}

BOOL CDrawDC::PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop)
{
	CRect rect(x,y,nWidth,nHeight);
	CRect rcDest(0,0,nWidth,nHeight);
	this->DPtoLP(&rect);
	this->DPtoLP(&rcDest);
	return CDC::PatBlt(rect.left-rcDest.left, rect.top-rcDest.top, rcDest.right - rcDest.left, rcDest.bottom - rcDest.top, dwRop);
}

CPoint CDrawDC::OffsetViewportOrg(int nWidth, int nHeight)
{
	CSize sizeOffset(nWidth, nHeight);
	this->LPtoDP(&sizeOffset);
	return CDC::OffsetViewportOrg(sizeOffset.cx, sizeOffset.cy);
}

void CDrawDC::PrepareDC(float fZoomScale)
{
	::SetMapMode(this->m_hDC,MM_ISOTROPIC);
	int XLogMm	= ::GetDeviceCaps(this->m_hDC, HORZSIZE);
	int YLogMm	= ::GetDeviceCaps(this->m_hDC, VERTSIZE);
	int XLogPix = ::GetDeviceCaps(this->m_hDC, HORZRES);
	int YLogPix = ::GetDeviceCaps(this->m_hDC, VERTRES);
	::SetWindowExtEx(this->m_hDC, XLogMm * 100, YLogMm * 100, NULL);
	::SetViewportExtEx(this->m_hDC, (int)(XLogPix*fZoomScale), (int)(YLogPix*fZoomScale),NULL);	
}

inline HDC CDrawDC::GetSafeHdc() const
{
	return CDC::GetSafeHdc();
}

inline CPoint CDrawDC::MoveTo(int x, int y)
{
	CPoint pt(x,y);
	return MoveTo(pt);
}

CPoint CDrawDC::MoveTo(POINT point)
{
	this->DPtoLP(&point);
	return CDC::MoveTo(point);
}

inline BOOL CDrawDC::LineTo(int x, int y)
{
	CPoint pt(x,y);
	return LineTo(pt);
}

BOOL CDrawDC::LineTo(POINT point)
{
	this->DPtoLP(&point);
	return CDC::LineTo(point);
}

inline BOOL CDrawDC::Rectangle(int x1, int y1, int x2, int y2)
{
	CRect rect(x1,y1,x2,y2);
	return Rectangle(rect);
}

BOOL CDrawDC::Rectangle(LPRECT lpRect)
{
	this->DPtoLP(lpRect);
	return CDC::Rectangle(lpRect);
}

inline BOOL CDrawDC::Ellipse(int x1, int y1, int x2, int y2)
{
	CRect rect(x1,y1,x2,y2);
	return Ellipse(rect);
}

BOOL CDrawDC::Ellipse(LPRECT lpRect)
{
	this->DPtoLP(lpRect);
	return CDC::Ellipse(lpRect);
}

inline BOOL CDrawDC::RoundRect(int x1, int y1, int x2, int y2, int x3, int y3)
{
	CRect rect(x1,y1,x2,y2);
	CPoint pt(x3,y3);
	return RoundRect(rect, pt);
}

BOOL CDrawDC::RoundRect(LPRECT lpRect, POINT point)
{
	this->DPtoLP(lpRect);
	this->DPtoLP(&point);
	return CDC::RoundRect(lpRect, point);
}

inline COLORREF CDrawDC::SetPixel(int x, int y, COLORREF crColor)
{
	CPoint pt(x,y);
	return SetPixel(pt, crColor);
}

COLORREF CDrawDC::SetPixel(POINT point, COLORREF crColor)
{
	this->DPtoLP(&point);
	return CDC::SetPixel(point,crColor);
}

CDrawObject::CDrawObject()
{
	m_rectBound.SetRectEmpty();
}

CDrawObject::~CDrawObject(void)
{
	
}

void CDrawObject::SetBounds(const CPoint& ptTopLeft, const CPoint& ptBottomRigth)
{	
	m_rectBound.SetRect(ptTopLeft, ptBottomRigth);
	m_rectBound.NormalizeRect();
}

void CDrawObject::DrawTracker(CDrawDC* pDC, const CPoint& ptOffset)
{
	CPen		pen(PS_DOT,1,::GetSysColor(COLOR_HIGHLIGHT));
	LOGBRUSH	logBrush;
	CBrush		brushNull;
	CPen *		pOldPen;
	CBrush *	pOldBrush;
	CPoint		handle;

	logBrush.lbStyle = BS_NULL;	
	brushNull.CreateBrushIndirect(&logBrush);
	pOldPen		= pDC->SelectObject(&pen);
	pOldBrush	= pDC->SelectObject(&brushNull);
	pDC->Rectangle(m_rectBound + ptOffset);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	brushNull.DeleteObject();
}

EdgeHandle	CDrawObject::HitObjectTest(CPoint point, BOOL bSelected)
{
	CRect rtRect = GetBoundRect();
	return CDrawHandle::HitTest(rtRect, point, bSelected);
}

EdgeHandle CDrawHandle::HitTest(CRect rtRect, CPoint point, BOOL bSelected)
{
	EdgeHandle eHandle;
	for (INT nIndex = EH_FIRST + 1; nIndex < EH_LAST; nIndex++)
	{
		eHandle = (EdgeHandle)nIndex;
		CRect rc = GetHandleRect(rtRect, eHandle);
		if (rc.PtInRect(point))
			return eHandle;
	}
	return EH_NONE;
}

CRect CDrawHandle::GetHandleRect(CRect rtRect, EdgeHandle nHandleID)
{
	CPoint point = GetHandle(rtRect, nHandleID);
	CRect rect(point.x - 2, point.y - 2, point.x + 2, point.y + 2);
	rect.NormalizeRect();
	return rect;
}

CPoint CDrawHandle::GetHandle(CRect rtRect, EdgeHandle nHandle)
{
	int x, y, xCenter, yCenter;

	// this gets the center regardless of left/right and top/bottom ordering
	xCenter = rtRect.left + rtRect.Width() / 2;
	yCenter = rtRect.top + rtRect.Height() / 2;

	switch (nHandle)
	{
	case EH_TOP_LEFT:
		x = rtRect.left;
		y = rtRect.top;
		break;
		
	case EH_TOP_MIDDLE:
		x = xCenter;
		y = rtRect.top;
		break;

	case EH_TOP_RIGHT:
		x = rtRect.right;
		y = rtRect.top;
		break;

	case EH_MIDDLE_RIGHT:
		x = rtRect.right;
		y = yCenter;
		break;

	case EH_BOTTOM_RIGHT:
		x = rtRect.right;
		y = rtRect.bottom;
		break;

	case EH_BOTTOM_MIDDLE:
		x = xCenter;
		y = rtRect.bottom;
		break;

	case EH_BOTTOM_LEFT:
		x = rtRect.left;
		y = rtRect.bottom;
		break;

	case EH_MIDDLE_LEFT:
		x = rtRect.left;
		y = yCenter;
		break;
	}

	return CPoint(x, y);
}

HCURSOR CDrawHandle::GetHandleCursor(EdgeHandle nHandle)
{
	LPCTSTR id;
	switch (nHandle)
	{
	case EH_TOP_LEFT:
	case EH_BOTTOM_RIGHT:
		id = IDC_SIZENWSE;
		break;
	case EH_TOP_MIDDLE:
	case EH_BOTTOM_MIDDLE:
		id = IDC_SIZENS;
		break;
	case EH_TOP_RIGHT:
	case EH_BOTTOM_LEFT:
		id = IDC_SIZENESW;
		break;
	case EH_MIDDLE_RIGHT:
	case EH_MIDDLE_LEFT:
		id = IDC_SIZEWE;
		break;
	}
	return AfxGetApp()->LoadStandardCursor(id);
}

void CDrawObject::MoveHandleTo(int nHandle, CPoint point)
{
	CRect position = GetBoundRect();
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:	// left top
		if (point.x < position.right)
			position.left = point.x;

		if (point.y < position.bottom)
			position.top = point.y;			
		break;

	case 2:	// middle top
		if (point.y < position.bottom)
			position.top = point.y;
		break;

	case 3:	// right top
		if (point.x > position.left)
			position.right = point.x;

		if (point.y < position.bottom)
			position.top = point.y;
		break;

	case 4:	// right middle
		if (point.x > position.left)
			position.right = point.x;
		break;

	case 5:	// right bottom
		if (point.x > position.left)
			position.right = point.x;
		
		if (point.y > position.top)
			position.bottom = point.y;
		break;

	case 6:	// middle bottom
		if (point.y > position.top)
			position.bottom = point.y;
		break;

	case 7:	// left bottom
		if (point.x < position.right)
			position.left = point.x;
		if (point.y > position.top)
			position.bottom = point.y;
		break;

	case 8:	// left middle
		if (point.x < position.right)
			position.left = point.x;
		break;
	}

	m_rectBound = position;
}


CDrawPen::CDrawPen()
{
	m_uPenStyle = PenStyle::PS_DOT1;
	m_uPenType	= PenType::PT_LINE;
}

CDrawPen::~CDrawPen(void)
{
}

void CDrawPen::SetPenStyle(COLORREF color, UINT style,float fZoom)
{
	LOGBRUSH lb; 
	lb.lbColor = color;     
	HDC hDc= ::GetDC(NULL);
	CUtility::PrepareDC(hDc);
	CPoint pt(1,1);
	::DPtoLP(hDc,&pt,1);
	m_uPenStyle = style;
	m_colorFore = color;
	switch (style)
	{
	case PenStyle::PS_DOT1:
		{
			m_penCurrent.DeleteObject();
			lb.lbStyle = BS_SOLID; 
			lb.lbHatch = HS_BDIAGONAL; 
			m_penCurrent.CreatePen(PS_GEOMETRIC|PS_ENDCAP_SQUARE,1 , &lb, 0, NULL); 
			break;
		}
	case PenStyle::PS_DOT2:
		{
			m_penCurrent.DeleteObject();
			m_penCurrent.CreatePen(PS_SOLID, 2, color);
			break;
		}
	case PenStyle::PS_DOT3:
		{
			m_penCurrent.DeleteObject();
			m_penCurrent.CreatePen(PS_SOLID, 3, color);
			break;
		}
	default:
		{
			break;
		}
	}
	::ReleaseDC(NULL,hDc);
}

void CDrawPen::DrawSelf(CDrawDC* pDC, const CPoint& ptOffset)
{
	if (m_pointArray.GetSize() == 0) return;
		
	CPoint ptPoint;
	if (m_uPenStyle == PenStyle::PS_RECT1 || 
		m_uPenStyle == PenStyle::PS_RECT2 || 
		m_uPenStyle == PenStyle::PS_RECT3 || 
		m_uPenStyle == PenStyle::PS_RECT4)
	{
		CBrush brush (m_colorFore);
		CPoint ptUnit;
		switch (m_uPenStyle)
		{
		case PenStyle::PS_RECT1:
			ptUnit.x = ptUnit.y = 4;
			break;
		case PenStyle::PS_RECT2:
			ptUnit.x = ptUnit.y = 6;
			break;
		case PenStyle::PS_RECT3:
			ptUnit.x = ptUnit.y = 8;
			break;
		case PenStyle::PS_RECT4:
			ptUnit.x = ptUnit.y = 10;
			break;
		default:
			break;
		}
		for (int i=0; i < m_pointArray.GetSize() - 1; i++)
		{
			ptPoint.x = m_pointArray[i].x + ptOffset.x;
			ptPoint.y = m_pointArray[i].y + ptOffset.y;
			pDC->FillRect(CRect(ptPoint.x, ptPoint.y, ptPoint.x + ptUnit.x, ptPoint.y + ptUnit.y), &brush);
		}
	}
	else
	{
		CPen* pOldPen = pDC->SelectObject(&m_penCurrent);
		ptPoint.x = m_pointArray[0].x + ptOffset.x;
		ptPoint.y = m_pointArray[0].y + ptOffset.y;
		if (m_pointArray.GetSize() == 2 && m_pointArray[0] == m_pointArray[1])
		{
			pDC->SetPixel(ptPoint, m_colorFore);
		}
		else
		{
			pDC->MoveTo(ptPoint);
			for (int i=0; i < m_pointArray.GetSize(); i++)
			{
				ptPoint.x = m_pointArray[i].x + ptOffset.x;
				ptPoint.y = m_pointArray[i].y + ptOffset.y;
				pDC->LineTo(ptPoint);
			}
		}
		pDC->SelectObject(pOldPen);
	}
}

void CDrawPen::AdjustBounds()
{
	if (m_pointArray.GetSize()==0)
	{
		m_rectBound.SetRectEmpty();
		return;
	}
	CPoint pt = m_pointArray[0];
	m_rectBound = CRect(pt.x, pt.y, pt.x, pt.y);

	for (int i=1; i < m_pointArray.GetSize(); i++)
	{
		pt = m_pointArray[i];
		m_rectBound.left     = min(m_rectBound.left, pt.x);
		m_rectBound.right    = max(m_rectBound.right, pt.x);
		m_rectBound.top      = min(m_rectBound.top, pt.y);
		m_rectBound.bottom   = max(m_rectBound.bottom, pt.y);
	}

	m_rectBound.InflateRect(2,2);
	return;
}

void CDrawPen::AddPoint(CPoint pt)
{
	m_pointArray.Add(pt);
}

int CDrawPen::GetPointCount()
{
	return m_pointArray.GetSize();
}


CDrawShape::CDrawShape(UINT type,const CRect& rect)
{	
	m_nShapeType = type;
	m_rectBound = rect;
	if (m_nShapeType != ShapeType::ST_LINE)
		m_rectBound.NormalizeRect();
}

void CDrawShape::SetBounds(const CPoint& ptTopLeft, const CPoint& ptBottomRigth)
{	
	m_rectBound.SetRect(ptTopLeft,ptBottomRigth);
	if (m_nShapeType != ShapeType::ST_LINE)
		m_rectBound.NormalizeRect();
}

CDrawShape::~CDrawShape(void)
{
}

void CDrawShape::DrawSelf(CDrawDC* pDC, const CPoint& ptOffset)
{
	CPen* pOldPen = pDC->SelectObject(&m_penCurrent);
	CBrush* pOldBrush = pDC->SelectObject(&m_brushCurrent);
	CRect rcBound(m_rectBound);
	rcBound += ptOffset;
	switch (m_nShapeType)
	{
	case ShapeType::ST_LINE:
		{
			pDC->MoveTo(rcBound.TopLeft());
			pDC->LineTo(rcBound.BottomRight());
			break;
		}
	case ShapeType::ST_RECT:
		{
			pDC->Rectangle(rcBound);
			break;
		}
	case ShapeType::ST_ELLIPSE:
		{
			pDC->Ellipse(rcBound);			
			break;
		}
	case ShapeType::ST_ROUND_RECT:
		{			
			CPoint pt(rcBound.Width()/3, rcBound.Height()/3);
			pDC->RoundRect(rcBound,pt);
			break;
		}
	default:
		break;
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CDrawShape::SetShapeStyle(COLORREF colorFore,COLORREF colorBack,UINT lineWidth,UINT shapeStyle,float fZoom)
{
	m_penCurrent.DeleteObject();
	m_brushCurrent.DeleteObject();

	HDC hDc= ::GetDC(NULL);
	CUtility::PrepareDC(hDc);
	CPoint pt(lineWidth,1);
	::DPtoLP(hDc,&pt,1);
	lineWidth = pt.x;

	switch (shapeStyle)
	{
	case ShapeStyle::SS_FRAME:
		{
			m_penCurrent.CreatePen(PS_SOLID,lineWidth,colorFore);

			LOGBRUSH lb;
			lb.lbStyle = BS_NULL;
			m_brushCurrent.CreateBrushIndirect(&lb);
			break;
		}
	case ShapeStyle::SS_FRAME_FILL:
		{
			m_penCurrent.CreatePen(PS_SOLID,lineWidth,colorFore);
			m_brushCurrent.CreateSolidBrush(colorBack);
			break;
		}
	case ShapeStyle::SS_FILL:
		{
			m_penCurrent.CreatePen(PS_SOLID,lineWidth,colorFore);
			m_brushCurrent.CreateSolidBrush(colorFore);
			break;
		}
	default:
		break;
	}
}

//------------------------------------------------------------------------------------
CDrawRegion::CDrawRegion(COLORREF colorBack)
{
	m_colorBk = colorBack;
	m_rectOrig.SetRectEmpty();
	m_bHasMoved = FALSE;
}

CDrawRegion::~CDrawRegion(void)
{
	m_imgArea.Destroy();
}

void CDrawRegion::DrawSelf(CDrawDC* pDC, const CPoint& ptOffset)
{
	//erase background
	CBrush brushBack(m_colorBk);
	pDC->FillRect(m_rectOrig + ptOffset,&brushBack);

	//clip region redraw
	CPoint ptPoint(m_rectBound.TopLeft() + ptOffset);
	if (!m_imgArea.IsNull())
	{
		CDC * pImgDC = CDC::FromHandle(m_imgArea.GetDC());
		pDC->StretchBlt(ptPoint.x, ptPoint.y, m_rectBound.Width(), m_rectBound.Height(),pImgDC,0,0,m_imgArea.GetWidth(),m_imgArea.GetHeight(),
							   SRCCOPY);
		m_imgArea.ReleaseDC();
		pImgDC->DeleteDC();
	}
}

void CDrawRegion::SetOrigArea(const CPoint& ptTopLeft, const CPoint& ptBottomRigth, HBITMAP hBMP)
{
	m_rectOrig.SetRect(ptTopLeft,ptBottomRigth);
	m_rectOrig.NormalizeRect();
	m_rectBound.CopyRect(m_rectOrig);

	m_imgArea.Destroy();
	m_imgArea.Attach(hBMP);
}

void CDrawRegion::SetBounds(const CPoint& ptTopLeft, const CPoint& ptBottomRigth)
{
	m_bHasMoved = TRUE;
	m_rectBound.SetRect(ptTopLeft,ptBottomRigth);
	m_rectBound.NormalizeRect();
}

//----------------------------------------------------------------
CDrawText::CDrawText()
{
	m_strText = _T("");
}

CDrawText::~CDrawText(void)
{
}

void CDrawText::DrawSelf(CDrawDC* pDC, const CPoint& ptOffset)
{
	CFont* pOldFont = (CFont*) pDC->SelectObject(&m_fontText);
	pDC->SetTextColor(m_colorText);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_strText,m_rectBound,DT_LEFT|DT_WORDBREAK);
	pDC->SelectObject(pOldFont);
}

void CDrawText::SetTextStyle(const LOGFONT& lf, COLORREF colorText)
{
	m_fontText.DeleteObject();
	m_colorText = colorText;
}

void CDrawText::SetText(const CString& str)
{
	m_strText = str;
}

CString CDrawText::GetText()
{
	return m_strText;
}
