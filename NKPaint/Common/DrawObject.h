#ifndef __CDRAWOBJECT__
#define __CDRAWOBJECT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CImage.h"
#include <afxtempl.h>
#include "Utility.H"

// Hot points are the eight points around the canvas
#define HOTPOINT_WIDTH		3
#define HOTPOINT_HEIGHT		3

class CDrawDC : public CDC
{
public:
	CDrawDC();
	BOOL    BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop);
	BOOL    StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
	void    FillRect(LPRECT lpRect, CBrush* pBrush);
	BOOL    PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop);
	virtual CPoint OffsetViewportOrg(int nWidth, int nHeight);
	void    PrepareDC(float fZoomScale = 1.0f);
	HDC     GetSafeHdc() const;
	CPoint  MoveTo(int x, int y);
	CPoint  MoveTo(POINT point);

	BOOL LineTo(int x, int y);
	BOOL LineTo(POINT point);
	BOOL Rectangle(int x1, int y1, int x2, int y2);
	BOOL Rectangle(LPRECT lpRect);
	BOOL Ellipse(int x1, int y1, int x2, int y2);
	BOOL Ellipse(LPRECT lpRect);
	BOOL RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
	BOOL RoundRect(LPRECT lpRect, POINT point);

	COLORREF SetPixel(int x, int y, COLORREF crColor);
	COLORREF SetPixel(POINT point, COLORREF crColor);
};

typedef enum {
	EH_FIRST = 0,

	EH_NONE = 0,
	EH_TOP_LEFT,
	EH_TOP_MIDDLE,
	EH_TOP_RIGHT,
	EH_MIDDLE_RIGHT,
	EH_BOTTOM_RIGHT,
	EH_BOTTOM_MIDDLE,
	EH_BOTTOM_LEFT,
	EH_MIDDLE_LEFT,

	EH_LAST,
} EdgeHandle;

class AFX_EXT_CLASS CDrawHandle
{
public:
	EdgeHandle		  HitTest(CRect rtRect, CPoint point, BOOL bSelected = FALSE);
	static HCURSOR	GetHandleCursor(EdgeHandle nHandle);

protected:
	CPoint			GetHandle(CRect rtRect, EdgeHandle nHandle);
	CRect			  GetHandleRect(CRect rtRect, EdgeHandle nHandleID);	
};

class CDrawObject : public CDrawHandle
{
public:
	enum PenType
	{
		PT_LINE,
		PT_ERASER,
		PT_BRUSH,
	};
	enum PenStyle
	{
		PS_DOT1,
		PS_DOT2,
		PS_DOT3,

		PS_RECT1,
		PS_RECT2,
		PS_RECT3,
		PS_RECT4,
	};
	enum ShapeType
	{
		ST_LINE,
		ST_RECT,
		ST_ELLIPSE,
		ST_ROUND_RECT
	};
	enum ShapeStyle
	{
		SS_FRAME,
		SS_FRAME_FILL,
		SS_FILL
	};

	CDrawObject();
	virtual			~CDrawObject(void);

	virtual void	DrawSelf(CDrawDC * pDC, const CPoint & ptOffset) = 0;
	CRect&			  GetBoundRect() { return m_rectBound; }
	virtual void	OffsetBounds(CPoint ptDelta) { m_rectBound.OffsetRect(ptDelta); };
	virtual void	SetBounds(const CPoint & ptTopLeft, const CPoint & ptBottomRigth);
	void		    	DrawTracker(CDrawDC* pDC, const CPoint & ptOffset);
	void			    MoveHandleTo(int nHandle, CPoint point);
	EdgeHandle		HitObjectTest(CPoint point, BOOL bSelected = FALSE);
protected:
	CRect         m_rectBound;
};


// used for pen,brush,eraser
class CDrawPen : public CDrawObject
{
public:
	CDrawPen();
	~CDrawPen(void);

	void			DrawSelf(CDrawDC * pDC, const CPoint & ptOffset);
	void			SetPenStyle(COLORREF color, UINT style, float fZoom = 1);
	CPen *		GetCurrentPen() { return &m_penCurrent; }
	void			AdjustBounds();
	void			AddPoint(CPoint pt);
	int				GetPointCount();

private:
	UINT			m_uPenType;
	UINT			m_uPenStyle;
	CArray<CPoint,CPoint>  m_pointArray;
	CPen			m_penCurrent;
	COLORREF	m_colorFore;	
};


// used for line rectangle ellipse round-rectangle
class CDrawShape :public CDrawObject
{
public:
	CDrawShape(UINT type, const CRect & rect = CRect(0,0,0,0));
	~CDrawShape(void);

	void			DrawSelf(CDrawDC * pDC, const CPoint & ptOffset);
	void			SetBounds(const CPoint & ptTopLeft, const CPoint & ptBottomRigth);
	void			SetShapeStyle(COLORREF colorFore, COLORREF colorBack, UINT lineWidth, UINT shapeStyle, float fZoom = 1);
	CPen *		GetCurrentPen() { return &m_penCurrent; }
	CBrush *	GetCurrentBrush() { return &m_brushCurrent; }

private:
	int				m_nShapeType;
	CPen			m_penCurrent;
	CBrush		m_brushCurrent;
};


// used for select-region
class CDrawRegion : public CDrawObject
{
public:
	CDrawRegion(COLORREF colorBack);
	~CDrawRegion(void);

	void			    DrawSelf(CDrawDC* pDC, const CPoint& ptOffset);
	virtual void	OffsetBounds(CPoint ptDelta){m_rectBound.OffsetRect(ptDelta); m_bHasMoved = TRUE;}
	void		    	SetOrigArea(const CPoint& ptTopLeft, const CPoint& ptBottomRigth, HBITMAP hBMP);
	void			    SetBounds(const CPoint& ptTopLeft, const CPoint& ptBottomRigth);
	BOOL			    IsMoved(){return m_bHasMoved;}
	void		    	DestroyImage(){m_imgArea.Destroy();}

private:
	CRect			  m_rectOrig;
	COLORREF		m_colorBk;
	CImage			m_imgArea;
	BOOL		  	m_bHasMoved;
};


//used for text-region
class CDrawText : public CDrawObject
{
public:
	CDrawText();	
	~CDrawText(void);

	void			DrawSelf(CDrawDC * pDC, const CPoint & ptOffset);
	void			SetTextStyle(const LOGFONT& lf, COLORREF colorText);
	void			SetText(const CString& str);
	CString		GetText();

private:
	CFont			m_fontText;
	CString		m_strText;
	COLORREF	m_colorText;
};

#endif