#include "stdafx.h"
#include "AdvanceDraw.h"
#include "ColorCursor.h"
#include "Utils.h"
#include <math.h>
#include "CImage.h"

static CBrush g_cbSystemHotPoint(::GetSysColor(COLOR_HIGHLIGHT));

BOOL DrawHotPoint(CDrawDC * pDC, 
				  int x, int y, 
				  int nWidth, int nHeight, 
				  int nHotWidth, int nHotHeight,
				  CBrush * pBrushBG,
				  CBrush * pBrushWhite,
				  BOOL bSelectedRegion)
{
	CBrush * pOldBrush;
	const int halfHPW = nHotWidth / 2;
	const int halfHPH = nHotHeight / 2;
	const int thirdHPW = nHotWidth / 3;
	const int thirdHPH = nHotHeight / 3;
	pOldBrush = pDC->SelectObject(pBrushBG);
	pDC->PatBlt(x,y,nWidth + nHotWidth,nHotHeight,PATCOPY);
	pDC->PatBlt(x,y+nHeight+nHotHeight,nWidth + nHotWidth,nHotHeight,PATCOPY);
	pDC->PatBlt(x,y,nHotWidth,nHeight + nHotHeight,PATCOPY);
	pDC->PatBlt(x+nWidth+nHotWidth,y,nHotWidth,nHeight + nHotHeight*2,PATCOPY);
	if (bSelectedRegion)
	{
		//white
		pDC->SelectObject(pBrushWhite);
		pDC->PatBlt(x+halfHPW,y+halfHPH,halfHPW,nHotHeight+nHeight+halfHPH,PATCOPY);
		pDC->PatBlt(x+halfHPW,y+halfHPH,nHotWidth+nWidth+halfHPW,halfHPH,PATCOPY);
		pDC->PatBlt(nWidth+x+nHotWidth+halfHPW,y+halfHPH,halfHPW,nHotHeight+nHeight+halfHPH,PATCOPY);
		pDC->PatBlt(x+halfHPW,nHeight+y+nHotHeight+halfHPH,nHotWidth+nWidth+halfHPW,halfHPH,PATCOPY);
		pDC->SelectObject(&g_cbSystemHotPoint);
		pDC->PatBlt(nWidth+x+nHotWidth+halfHPW,y+halfHPH,halfHPW,3,PATCOPY);
		for (int i = y+halfHPH + 3; i <= y+nHotHeight+nHeight+halfHPH; i+=8)
		{
			if ((i + 4) > (y+nHotHeight+nHeight+halfHPH))
			{
				pDC->PatBlt(x+halfHPW,i,halfHPW,y+nHotHeight+nHeight+halfHPH-i+1,PATCOPY);
				break;
			}
			else
				pDC->PatBlt(x+halfHPW,i,halfHPW,4,PATCOPY);
			if ((i + 8) > (y+nHotHeight+nHeight+halfHPH))
			{
				pDC->PatBlt(nWidth+x+nHotWidth+halfHPW,i+4,halfHPW,y+nHotHeight+nHeight+halfHPH-i-4+1,PATCOPY);
			}
			else
				pDC->PatBlt(nWidth+x+nHotWidth+halfHPW,i+4,halfHPW,4,PATCOPY);
		}
		pDC->PatBlt(x+halfHPW,nHeight+y+nHotHeight+halfHPH,3,halfHPH,PATCOPY);
		for (int j = x+halfHPW + 3; j <= x+nHotWidth+nWidth+halfHPW; j+=8)
		{
			if ((j + 4) > (x+nHotWidth+nWidth+halfHPW))
			{
				pDC->PatBlt(j,y+halfHPH,x+nHotWidth+nWidth+halfHPW-j+1,halfHPH,PATCOPY);
				break;
			}
			else
				pDC->PatBlt(j,y+halfHPH,4,halfHPH,PATCOPY);
			if ((j + 8) > (x+nHotWidth+nWidth+halfHPW))
			{
				pDC->PatBlt(j+4,nHeight+y+nHotHeight+halfHPH,x+nHotWidth+nWidth+halfHPW-j-4+1,halfHPH,PATCOPY);
			}
			else
				pDC->PatBlt(j+4,nHeight+y+nHotHeight+halfHPH,4,halfHPH,PATCOPY);
		}
	}
	pDC->SelectObject(&g_cbSystemHotPoint);
	//left		top
	pDC->PatBlt(x,y,nHotWidth,nHotHeight,PATCOPY);
	//left		middle
	pDC->PatBlt(x,y+(nHeight-nHotHeight+thirdHPW)/2+nHotHeight,nHotWidth,nHotHeight,PATCOPY);
	//left		right
	pDC->PatBlt(x,nHeight+y+nHotHeight,nHotWidth,nHotHeight,PATCOPY);
	//middle	top
	pDC->PatBlt(x+(nWidth-nHotWidth+thirdHPW)/2+nHotWidth,y,nHotWidth,nHotHeight,PATCOPY);
	//right		top
	pDC->PatBlt(nWidth+x+nHotWidth,y,nHotWidth,nHotHeight,PATCOPY);
	//right		middle
	pDC->PatBlt(nWidth+x+nHotWidth,y+(nHeight-nHotHeight+thirdHPH)/2+nHotHeight,nHotWidth,nHotHeight,PATCOPY);
	//middle	bottom
	pDC->PatBlt(x+(nWidth-nHotWidth+thirdHPW)/2+nHotWidth,nHeight+y+nHotHeight,nHotWidth,nHotHeight,PATCOPY);
	//right		bottom
	pDC->PatBlt(nWidth+x+nHotWidth,nHeight+y+nHotHeight,nHotWidth,nHotHeight,PATCOPY);
	if (!bSelectedRegion)
	{
		pDC->SelectObject(pBrushWhite);
		//left top
		pDC->PatBlt(x+thirdHPW,y+thirdHPH,nHotWidth-thirdHPW,nHotHeight-thirdHPH,PATCOPY);
		//left middle
		pDC->PatBlt(x+thirdHPW,y+(nHeight-nHotHeight+thirdHPH)/2+nHotHeight+thirdHPH,nHotWidth-thirdHPW,nHotHeight-thirdHPH,PATCOPY);
		//middle top
		pDC->PatBlt(x+(nWidth-nHotWidth+thirdHPW)/2+nHotWidth+thirdHPW,y+thirdHPH,nHotWidth-thirdHPW,nHotHeight-thirdHPH,PATCOPY);
		//right top
		pDC->PatBlt(nWidth+x+nHotWidth+thirdHPW,y+thirdHPH,nHotWidth-thirdHPW,nHotHeight-thirdHPH,PATCOPY);
		//left bottom
		pDC->PatBlt(x+thirdHPW,nHeight+y+nHotHeight+thirdHPH,nHotWidth-thirdHPW,nHotHeight-thirdHPH,PATCOPY);
	}
	pDC->SelectObject(&pOldBrush);
	return TRUE;
}

BOOL GetBimapPixel(HBITMAP hBitmap, int x, int y, COLORREF & color)
{
	HDC hMainDC = ::GetDC(NULL);
	HDC hTempDC = ::CreateCompatibleDC(hMainDC); 
	HBITMAP hOldBitmap	= (HBITMAP)::SelectObject(hTempDC,hBitmap);
	BITMAP bmBitmap;
	::GetObject(hBitmap, sizeof(BITMAP), &bmBitmap);

	if (x >= bmBitmap.bmWidth || x < 0 || y >= bmBitmap.bmHeight || y < 0)
		return FALSE;
	color = ::GetPixel(hTempDC, x, y);

	::SelectObject(hTempDC,hOldBitmap);
	::DeleteDC(hTempDC);
	return TRUE;
}

BOOL LoadImageList(CImageList * pImageList, UINT uBitmapResource, CPoint ptMask, int nImageWidth, int nImageCount)
{
	CBitmap	cBitmap;
	BITMAP	bmBitmap;
	CSize	cSize;
	UINT	uBitCount;
	COLORREF rgbMask = RGB(0,0,0);
	ASSERT(pImageList);
	if (!cBitmap.Attach(LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(uBitmapResource), RT_BITMAP), MAKEINTRESOURCE(uBitmapResource),
								  IMAGE_BITMAP, 0, 0,
		                          LR_DEFAULTSIZE|LR_CREATEDIBSECTION)) ||
	    !cBitmap.GetBitmap(&bmBitmap))
		return FALSE;

	GetBimapPixel(cBitmap.operator HBITMAP(),ptMask.x,ptMask.y, rgbMask);

	switch(bmBitmap.bmBitsPixel)
	{
	case 1:
		uBitCount = ILC_MASK;
		break;
	case 2:
		uBitCount = ILC_COLOR4;
		break;
	case 8:
		uBitCount = ILC_COLOR8;
		break;
	case 24:
		uBitCount = ILC_COLOR24;
		break;
	case 32:
		uBitCount = ILC_COLOR32;
		break;
	}
	cSize  = CSize(bmBitmap.bmWidth, bmBitmap.bmHeight);
	
	if (!pImageList->Create(nImageWidth, cSize.cy,
		                   uBitCount|ILC_MASK,
						   nImageCount, 0))
		return FALSE;
	
	if (pImageList->Add(&cBitmap, rgbMask) == -1)
		return FALSE;

	cBitmap.Detach();
	return TRUE;
}


float ConventUnit(UNIT_TYPE oldType, UNIT_TYPE newType, float value)
{
	switch(oldType)
	{
	case UT_INCH:
		switch(newType)
		{
		case UT_INCH:
			break;
		case UT_CM:
			return value * 2.54f;
		case UT_PIXEL:
			return value * 96.52f;
		default:
			break;
		}
		break;
	case UT_CM:
		switch(newType)
		{
		case UT_INCH:
			return value * 0.393701f;
		case UT_CM:
			break;
		case UT_PIXEL:
			return value * 38.f;
		default:
			break;
		}
		break;
	case UT_PIXEL:
		switch(newType)
		{
		case UT_INCH:
			return value * 0.010361f;
		case UT_CM:
			return value * 0.026316f;
		case UT_PIXEL:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return value;
}

#define BITMAP_OPERATION_BEGIN(hSrcDC, hDstDC, hBitmapSrc, hBitmapDst, bitmap)			\
{																						\
	HDC hDstDC;																			\
	HDC hSrcDC;																			\
	BITMAP bitmap;																		\
	HBITMAP hOldBitmap,hOldBitmap2;														\
	hDstDC = ::CreateCompatibleDC(NULL);												\
	hSrcDC = ::CreateCompatibleDC(NULL);												\
	::GetObject(hBitmapSrc,sizeof(bitmap),&bitmap);										\
	hOldBitmap2 = (HBITMAP)::SelectObject(hSrcDC, hBitmapSrc);							\
	hBitmapDst  = ::CreateCompatibleBitmap(hSrcDC, bitmap.bmWidth, bitmap.bmHeight);	\
	hOldBitmap	= (HBITMAP)::SelectObject(hDstDC, hBitmapDst);							\

#define BITMAP_OPERATION_END(hSrcDC, hDstDC, hBitmapSrc, hBitmapDst, bitmap)			\
	::SelectObject(hDstDC,hOldBitmap);													\
	::SelectObject(hSrcDC ,hOldBitmap2);												\
	::DeleteDC(hDstDC);																	\
	::DeleteDC(hSrcDC);																	\
}																						\

#define BITMAP_OPERATION_BEGIN_EX(hSrcDC, hDstDC, hBitmapSrc, hBitmapDst, nWidth, nHeight)\
{																						\
	HDC hDstDC;																			\
	HDC hSrcDC;																			\
	HBITMAP hOldBitmap,hOldBitmap2;														\
	hDstDC = ::CreateCompatibleDC(NULL);												\
	hSrcDC = ::CreateCompatibleDC(NULL);												\
	hOldBitmap2 = (HBITMAP)::SelectObject(hSrcDC, hBitmapSrc);							\
	hBitmapDst  = ::CreateCompatibleBitmap(hSrcDC, nWidth, nHeight);					\
	hOldBitmap	= (HBITMAP)::SelectObject(hDstDC, hBitmapDst);							\

#define BITMAP_OPERATION_END_EX(hSrcDC, hDstDC, hBitmapSrc, hBitmapDst, nWidth, nHeight)\
	::SelectObject(hDstDC,hOldBitmap);													\
	::SelectObject(hSrcDC ,hOldBitmap2);												\
	::DeleteDC(hDstDC);																	\
	::DeleteDC(hSrcDC);																	\
}																						\

HBITMAP CreatBitmap(HBITMAP hBitmapRef, int nWidth, int nHeight, COLORREF colorDef)
{
	HBITMAP hBitmapDst;
	BITMAP_OPERATION_BEGIN_EX(hSrcDC, hDstDC, hBitmapRef, hBitmapDst, nWidth, nHeight);
	{
		CBrush	brushDef(colorDef);
		::FillRect(hDstDC, &CRect(0, 0, nWidth, nHeight), brushDef);
	}
	BITMAP_OPERATION_END_EX(hSrcDC, hDstDC, hBitmapRef, hBitmapDst, nWidth, nHeight);
	return hBitmapDst;
}

HBITMAP CopyBitmap(HBITMAP hBitmapSrc)
{
	HBITMAP hBitmapDst;
	BITMAP_OPERATION_BEGIN(hSrcDC, hDstDC, hBitmapSrc, hBitmapDst, bitmap);
	{
		::BitBlt(hDstDC, 0, 0, bitmap.bmWidth,bitmap.bmHeight, hSrcDC, 0, 0, SRCCOPY);
	}
	BITMAP_OPERATION_END(hSrcDC, hDstDC, hBitmapSrc, hBitmapDst, bitmap);
	return hBitmapDst;
}

HBITMAP ClipBitmap(HBITMAP hBitmapSrc, INT nFromX, INT nFromY, INT nWidth, INT nHeight)
{
	HBITMAP hBitmapDst;
	BITMAP_OPERATION_BEGIN_EX(hSrcDC, hDstDC, hBitmapSrc, hBitmapDst, nWidth, nHeight);
	{
		::BitBlt(hDstDC, 0, 0, nWidth, nHeight, hSrcDC, nFromX, nFromY, SRCCOPY);
	}
	BITMAP_OPERATION_END_EX(hSrcDC, hDstDC, hBitmapSrc, hBitmapDst, nWidth, nHeight);
	return hBitmapDst;
}

HBITMAP ChangeBitmap(HBITMAP hBitmapSrc, int nWidth, int nHeight, COLORREF bk)
{
	HBITMAP hBitmapDst;
	BITMAP_OPERATION_BEGIN_EX(hSrcDC, hDstDC, hBitmapSrc, hBitmapDst, nWidth, nHeight);
	{
		HBRUSH  hBrush;
		HBRUSH  hOldBrush;
		hBrush		= ::CreateSolidBrush(bk);
		hOldBrush	= (HBRUSH)::SelectObject(hDstDC,hBrush);

		::FillRect(hDstDC, CRect(0,0,nWidth,nHeight), hBrush);
		::BitBlt(hDstDC, 0, 0, nWidth, nHeight, hSrcDC, 0, 0, SRCCOPY);

		::SelectObject(hDstDC,hOldBrush);
		::DeleteObject(hBrush);
	}
	BITMAP_OPERATION_END_EX(hSrcDC, hDstDC, hBitmapSrc, hBitmapDst, nWidth, nHeight);
	return hBitmapDst;
}

BOOL CopyBitmapEx(HBITMAP hBitmapDst, INT nToX, INT nToY, INT nToWidth, INT nToHeight, HBITMAP hBitmapSrc, INT nFromX, INT nFromY)
{
	HDC hDstDC;
	HDC hSrcDC;
	HBITMAP hOldBitmap,hOldBitmap2;

	hDstDC = ::CreateCompatibleDC(NULL);
	hSrcDC = ::CreateCompatibleDC(NULL);
	hOldBitmap2 = (HBITMAP)::SelectObject(hSrcDC, hBitmapSrc);
	hOldBitmap	= (HBITMAP)::SelectObject(hDstDC, hBitmapDst);

	::BitBlt(hDstDC, nToX, nToY, nToWidth, nToHeight, hSrcDC, nFromX, nFromY, SRCCOPY);

	::SelectObject(hDstDC,hOldBitmap);
	::SelectObject(hSrcDC ,hOldBitmap2);

	::DeleteDC(hDstDC);
	::DeleteDC(hSrcDC);
	
	return TRUE;
}

HBITMAP StretchBitmap(HBITMAP hBitmapSrc, int nWStretch, int nHStretch, BOOL bRate)
{
	HDC hDstDC;
	HDC hSrcDC;
	BITMAP bitmap;
	HBITMAP hBitmapDst;
	HBITMAP hOldBitmap,hOldBitmap2;
	int nDstWidth, nDstHeight;

	hDstDC = ::CreateCompatibleDC(NULL);
	hSrcDC = ::CreateCompatibleDC(NULL);

	::GetObject(hBitmapSrc,sizeof(bitmap),&bitmap);
	if (bRate)
	{
		nDstWidth  = bitmap.bmWidth  * nWStretch / 100;
		nDstHeight = bitmap.bmHeight * nHStretch / 100;
	}
	else
	{
		nDstWidth  = nWStretch;
		nDstHeight = nHStretch;
	}
	hOldBitmap2 = (HBITMAP)::SelectObject(hSrcDC, hBitmapSrc);
	hBitmapDst = ::CreateCompatibleBitmap(hSrcDC, nDstWidth, nDstHeight);
	hOldBitmap	= (HBITMAP)::SelectObject(hDstDC, hBitmapDst);
	
	::SetStretchBltMode(hDstDC, HALFTONE);
	::StretchBlt(hDstDC, 0, 0, nDstWidth, nDstHeight, hSrcDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	::SelectObject(hDstDC, hOldBitmap);
	::SelectObject(hSrcDC, hOldBitmap2);

	::DeleteDC(hDstDC);
	::DeleteDC(hSrcDC);
	
	return hBitmapDst;
}

#define WIDTH 32
#define HEIGHT 32

HBITMAP GetCursorBitmap()
{
	HDC hMainDC = ::GetDC(NULL);
	HDC hTempDC = ::CreateCompatibleDC(hMainDC); 
	HBITMAP hTempBitmap = ::CreateCompatibleBitmap(hMainDC,WIDTH,HEIGHT);
	HBITMAP hOldBitmap	= (HBITMAP)::SelectObject(hTempDC,hTempBitmap);
	HPEN    hPen		= ::CreatePen(PS_SOLID,1,RGB(255,0,0));
	HPEN    hOldPen		= (HPEN)::SelectObject(hTempDC,hPen);
	HBRUSH  hBrush		= ::CreateSolidBrush(RGB(0,255,0));
	HBRUSH  hOldBrush	= (HBRUSH)::SelectObject(hTempDC,hBrush);

	::Rectangle(hTempDC,2,2,10,10);

	::SetTextColor(hTempDC,RGB(0,0,255));
	::SetBkMode(hTempDC,TRANSPARENT);
	::DrawText(hTempDC,"Hi",2,CRect(0,11,WIDTH,HEIGHT),DT_CENTER);
	::MoveToEx(hTempDC,0,0,NULL);
	::LineTo(hTempDC,WIDTH-1,0);
	::LineTo(hTempDC,WIDTH-1,HEIGHT-1);
	::LineTo(hTempDC,0,HEIGHT-1);
	::LineTo(hTempDC,0,0);
	
	::SelectObject(hTempDC,hOldBrush);
	::SelectObject(hTempDC,hOldPen);
	::SelectObject(hTempDC,hOldBitmap);

	::DeleteObject(hBrush);
	::DeleteObject(hPen);
	::DeleteDC(hTempDC);
	::ReleaseDC(NULL,hMainDC);

	return hTempBitmap;
}

HBITMAP GetEraserCursorBitmap(int xHotspot, int yHotspot, int xTo, int yTo, int nWidth, int nHeight, COLORREF colorBk)
{
	HDC hMainDC = ::GetDC(NULL);
	HDC hTempDC = ::CreateCompatibleDC(hMainDC); 
	HBITMAP hTempBitmap = ::CreateCompatibleBitmap(hMainDC,WIDTH,HEIGHT);
	HBITMAP hOldBitmap	= (HBITMAP)::SelectObject(hTempDC,hTempBitmap);
	HPEN    hPen		= ::CreatePen(PS_SOLID,1,RGB(0,0,0));
	HPEN    hOldPen		= (HPEN)::SelectObject(hTempDC,hPen);
	HBRUSH  hBrush		= ::CreateSolidBrush(COLOR_SYSTEM_CURSOR);
	HBRUSH  hForeBrush	= ::CreateSolidBrush(colorBk);
	HBRUSH  hOldBrush	= (HBRUSH)::SelectObject(hTempDC,hBrush);

	::FillRect(hTempDC,CRect(0,0,nWidth,nHeight), g_cbSystemCursor);
	::Rectangle(hTempDC,xHotspot,yHotspot,xTo,yTo);
	::FillRect(hTempDC,CRect(xHotspot+1,yHotspot+1,xTo-1,yTo-1), hForeBrush);
	
	::SelectObject(hTempDC,hOldBrush);
	::SelectObject(hTempDC,hOldPen);
	::SelectObject(hTempDC,hOldBitmap);

	::DeleteObject(hBrush);
	::DeleteObject(hPen);
	::DeleteDC(hTempDC);
	::ReleaseDC(NULL,hMainDC);

	return hTempBitmap;
}

HCURSOR CreateEraserCursor(int xHotspot, int yHotspot, int xTo, int yTo, int nWidth, int nHeight, COLORREF colorBk)
{
	HBITMAP m_hSourceBitmap = GetEraserCursorBitmap(xHotspot,yHotspot,xTo,yTo,nWidth,nHeight,colorBk);
	return CColorCursor::CreateCursorFromBitmap(m_hSourceBitmap,COLOR_SYSTEM_CURSOR,xHotspot,yHotspot);
}

HBITMAP InvertBitmap(HBITMAP source)
{
	HDC hMainDC = ::GetDC(NULL);
	HDC hTempDC = ::CreateCompatibleDC(hMainDC); 
	HBITMAP hOldBitmap	= (HBITMAP)::SelectObject(hTempDC,source);
	BITMAP bmBitmap;

	::GetObject(source, sizeof(BITMAP), &bmBitmap);
	::PatBlt(hTempDC,0,0,bmBitmap.bmWidth,bmBitmap.bmHeight,PATINVERT);
	::SelectObject(hTempDC,hOldBitmap);
	::DeleteDC(hTempDC);
	::ReleaseDC(NULL,hMainDC);
	return source;
}

HBITMAP AdvanceTransformBitmap(HBITMAP hSrcBitmap, LONG lDstWidth, LONG lDstHeight, CONST XFORM * pXForm, HBRUSH bkBrush)
{
	BITMAP bmSrcBitmap;
	
	HDC hMainDC = ::GetDC(NULL);
	HDC hSrcDC = ::CreateCompatibleDC(hMainDC);
	HDC hDesDC = ::CreateCompatibleDC(hSrcDC);
	HBITMAP hOldSrcBitmap = (HBITMAP)::SelectObject(hSrcDC,hSrcBitmap);
	HBITMAP hOldDstBitmap;
	HBITMAP hDstBitmap;

	hDstBitmap = ::CreateCompatibleBitmap(hSrcDC,lDstWidth,lDstHeight);
	hOldDstBitmap = (HBITMAP)::SelectObject(hDesDC,hDstBitmap);
	// Fill background color
	::FillRect(hDesDC, CRect(0,0,lDstWidth,lDstHeight), bkBrush);
		
	// Set the property of DC, enable SetWorldTransform
	SetGraphicsMode(hDesDC, GM_ADVANCED);
	// set the transformation of coordinate	
	SetWorldTransform(hDesDC, pXForm);		
	// display bitmap
	::GetObject(hSrcBitmap, sizeof(BITMAP), &bmSrcBitmap);
	::StretchBlt(hDesDC, 0, 0, bmSrcBitmap.bmWidth, bmSrcBitmap.bmHeight,  hSrcDC,  0, 0, bmSrcBitmap.bmWidth, bmSrcBitmap.bmHeight,SRCCOPY);

	::SelectObject(hSrcDC,hOldSrcBitmap);
	::SelectObject(hDesDC,hOldDstBitmap);
	::DeleteDC(hSrcDC);
	::DeleteDC(hDesDC);
	return hDstBitmap;
}

HBITMAP RotateBitmap(HBITMAP source, DOUBLE angle, HBRUSH bkBrush)
{
	BITMAP bmSrcBitmap;
	
	LONG SrcBmHeight;
	LONG SrcBmWidth;
	LONG DesBmHeight;
	LONG DesBmWidth;
	XFORM	xform;
	DOUBLE sine,cosine;
	LONG x1, y1, x2, y2, x3, y3;
	LONG minx, miny, maxx, maxy;

	::GetObject(source, sizeof(BITMAP), &bmSrcBitmap);
	SrcBmHeight = bmSrcBitmap.bmHeight;
	SrcBmWidth = bmSrcBitmap.bmWidth;

	sine   = sin(angle);
	cosine = cos(angle);

	x1 = (LONG)floor(-SrcBmHeight * sine   + 0.5);
	y1 = (LONG)floor( SrcBmHeight * cosine + 0.5);
	x2 = (LONG)floor( SrcBmWidth  * cosine - SrcBmHeight * sine + 0.5);
	y2 = (LONG)floor( SrcBmHeight * cosine + SrcBmWidth  * sine + 0.5);
	x3 = (LONG)floor( SrcBmWidth  * cosine + 0.5);
	y3 = (LONG)floor( SrcBmWidth  * sine   + 0.5);
	minx = min(0,min(x1, min(x2,x3)));
	miny = min(0,min(y1, min(y2,y3)));
	maxx = max(x1, max(x2,x3));
	maxy = max(y1, max(y2,y3));

	DesBmWidth  = maxx - minx;
	DesBmHeight = maxy - miny;

	/* 
	 x' = x * eM11 + y * eM21 + eDx
   y' = x * eM12 + y * eM22 + eDy 
	*/

	// Computer the parameters of rotation
	xform.eM11 = (FLOAT)cosine;	
	xform.eM21 = (FLOAT)-sine;	
	xform.eM12 = (FLOAT)sine;	
	xform.eM22 = (FLOAT)cosine;
				
	xform.eDx = (FLOAT)(-minx);	
	xform.eDy = (FLOAT)(-miny);
		
	return AdvanceTransformBitmap(source, DesBmWidth, DesBmHeight, &xform, bkBrush);
}

HBITMAP RotateBitmap(HBITMAP source, DOUBLE angle, POINT ptCenter, HBRUSH bkBrush)
{
	BITMAP bmSrcBitmap;
	
	LONG SrcBmHeight;
	LONG SrcBmWidth;
	LONG DesBmHeight;
	LONG DesBmWidth;
	XFORM	xform;
	DOUBLE sine,cosine;

	::GetObject(source, sizeof(BITMAP), &bmSrcBitmap);
	SrcBmHeight = bmSrcBitmap.bmHeight;
	SrcBmWidth = bmSrcBitmap.bmWidth;

	sine   = sin(angle);
	cosine = cos(angle);

	DesBmWidth  = SrcBmWidth;
	DesBmHeight = SrcBmHeight;

	/* 
	 x' = x * eM11 + y * eM21 + eDx
   y' = x * eM12 + y * eM22 + eDy

	 x  = (x'* eM22 - y'* eM21 - eDx * eM22 + eDy * eM21) / (eM22 * eM11 - eM21 * eM12)
	 y  =(-x'* eM12 + y'* eM11 + eDx * eM12 - eDy * eM11) / (eM22 * eM11 - eM21 * eM12)
	*/

	// Computer the parameters of rotation
	xform.eM11 = (FLOAT)cosine;
	xform.eM21 = (FLOAT)-sine;
	xform.eM12 = (FLOAT)sine;
	xform.eM22 = (FLOAT)cosine;
				
	xform.eDx = (FLOAT)(-ptCenter.x * cosine + ptCenter.y * sine + ptCenter.x);
	xform.eDy = (FLOAT)(-ptCenter.y * cosine - ptCenter.x * sine + ptCenter.y);
		
	return AdvanceTransformBitmap(source, DesBmWidth, DesBmHeight, &xform, bkBrush);
}


HBITMAP HorizontalContortBitmap(HBITMAP source, DOUBLE angle, HBRUSH bkBrush)
{
	return NULL;
}

HBITMAP NormalRotateBitmap(HBITMAP source, NORMAL_ROTATE_TYPE type)
{
	BITMAP bmSrcBitmap;
	
	LONG SrcBmHeight;
	LONG SrcBmWidth;
	LONG DesBmHeight;
	LONG DesBmWidth;
	HDC hMainDC = ::GetDC(NULL);
	HDC hSrcDC = ::CreateCompatibleDC(hMainDC);
	HDC hDesDC = ::CreateCompatibleDC(hSrcDC);
	HBITMAP hOldSrcBitmap = (HBITMAP)::SelectObject(hSrcDC,source);
	HBITMAP hOldDesBitmap;
	HBITMAP target;
	XFORM	xform;

	::GetObject(source, sizeof(BITMAP), &bmSrcBitmap);
	SrcBmHeight = bmSrcBitmap.bmHeight;
	SrcBmWidth = bmSrcBitmap.bmWidth;
	if (type == NRT_90)
	{
		DesBmHeight = SrcBmWidth;
		DesBmWidth = SrcBmHeight;
		target = ::CreateCompatibleBitmap(hSrcDC,DesBmWidth,DesBmHeight);
		hOldDesBitmap = (HBITMAP)::SelectObject(hDesDC,target);

		/* 
		x' = x * eM11 + y * eM21 + eDx
        y' = x * eM12 + y * eM22 + eDy 
		*/

  	// Computer the parameters of rotation
		xform.eM11 = 0;	
		xform.eM12 = 1;	
		xform.eM21 = -1;	
		xform.eM22 = 0;
					
		xform.eDx = (FLOAT)DesBmWidth;	
		xform.eDy = 0;
			
	  // Set the property of DC, enable SetWorldTransform
		SetGraphicsMode(hDesDC, GM_ADVANCED);		
	  // set the transformation of coordinate	
		SetWorldTransform(hDesDC, &xform);		
		// display bitmap
		::StretchBlt(hDesDC, 0, 0, DesBmHeight, DesBmWidth,  hSrcDC,  0, 0, SrcBmWidth,SrcBmHeight,SRCCOPY);
	}
	else if (type == NRT_180)
	{
		DesBmHeight = SrcBmHeight;
		DesBmWidth = SrcBmWidth;
		target = ::CreateCompatibleBitmap(hSrcDC,DesBmWidth,DesBmHeight);
		hOldDesBitmap = (HBITMAP)::SelectObject(hDesDC,target);
		
		::StretchBlt(hDesDC, 0, 0, DesBmWidth, DesBmHeight, hSrcDC, DesBmWidth - 1, SrcBmHeight - 1, -SrcBmWidth, -SrcBmHeight, SRCCOPY);
	}
	else if (type == NRT_270)
	{
		DesBmHeight = SrcBmWidth;
		DesBmWidth = SrcBmHeight;
		target = ::CreateCompatibleBitmap(hSrcDC,DesBmWidth,DesBmHeight);
		hOldDesBitmap = (HBITMAP)::SelectObject(hDesDC,target);

		/* 
		x' = x * eM11 + y * eM21 + eDx
    y' = x * eM12 + y * eM22 + eDy 
		*/

  	// Computer the parameters of rotation
		xform.eM11 = 0;	
		xform.eM12 = -1;	
		xform.eM21 = 1;	
		xform.eM22 = 0;
					
		xform.eDx = 0;	
		xform.eDy = (FLOAT)DesBmHeight;
			
	  // Set the property of DC, enable SetWorldTransform
		SetGraphicsMode(hDesDC, GM_ADVANCED);		
	  // set the transformation of coordinate	
		SetWorldTransform(hDesDC, &xform);		
		// display bitmap
		::StretchBlt(hDesDC, 0, 0, DesBmHeight, DesBmWidth,  hSrcDC,  0, 0, SrcBmWidth,SrcBmHeight,SRCCOPY);
	}
	else if (type == NRT_VER)
	{
		DesBmHeight = SrcBmHeight;
		DesBmWidth = SrcBmWidth;
		target = ::CreateCompatibleBitmap(hSrcDC,DesBmWidth,DesBmHeight);
		hOldDesBitmap = (HBITMAP)::SelectObject(hDesDC,target);
		
		::StretchBlt(hDesDC, 0, 0, DesBmWidth, DesBmHeight, hSrcDC, 0, SrcBmHeight - 1, SrcBmWidth, -SrcBmHeight, SRCCOPY);
	}
	else if (type == NRT_HOR)
	{
		DesBmHeight = SrcBmHeight;
		DesBmWidth = SrcBmWidth;
		target = ::CreateCompatibleBitmap(hSrcDC,DesBmWidth,DesBmHeight);
		hOldDesBitmap = (HBITMAP)::SelectObject(hDesDC,target);
		
		::StretchBlt(hDesDC, 0, 0, DesBmWidth, DesBmHeight, hSrcDC, SrcBmWidth - 1, 0, -SrcBmWidth, SrcBmHeight, SRCCOPY);
	}

	::SelectObject(hSrcDC,hOldSrcBitmap);
	::SelectObject(hDesDC,hOldDesBitmap);
	::DeleteDC(hSrcDC);
	::DeleteDC(hDesDC);
	return target;
}


HBITMAP Exposure(HBITMAP hBitmap, FLOAT fGamma, LONG lBrightness, LONG lContrast)
{
	CImage imgCurrent;
	imgCurrent.CreateFromHBITMAP(hBitmap);

	imgCurrent.Exposure(fGamma, lBrightness, lContrast);
	
	return (imgCurrent.Detach());
}
