#ifndef __ADVANCEDRAW_H_INCLUDED__
#define __ADVANCEDRAW_H_INCLUDED__

#include "DrawObject.h"

BOOL DrawHotPoint(CDrawDC * pDC,
				  int x, int y,
				  int nWidth, int nHeight,
				  int nHotWidth, int nHotHeight,
				  CBrush * pBrushBG,
				  CBrush * pBrushWhite,
				  BOOL bSelectedRegion);

BOOL LoadImageList(CImageList * pImageList, UINT uBitmapResource, CPoint ptMask, int nImageWidth, int nImageCount);

enum UNIT_TYPE
{
	UT_INCH,
	UT_CM,
	UT_PIXEL,
};

typedef enum{
	NRT_90 = 0,
	NRT_180,
	NRT_270,
	NRT_VER,
	NRT_HOR
} NORMAL_ROTATE_TYPE;

float ConventUnit(UNIT_TYPE oldType, UNIT_TYPE newType, float value);

HBITMAP GetCursorBitmap();

HBITMAP AFX_EXT_API CreatBitmap(HBITMAP hBitmapRef, int nWidth, int nHeight, COLORREF colorDef);

HCURSOR CreateEraserCursor(int xHotspot, int yHotspot, int xTo, int yTo, int nWidth, int nHeight, COLORREF colorBk);

HBITMAP InvertBitmap(HBITMAP source);

HBITMAP AFX_EXT_API RotateBitmap(HBITMAP source, DOUBLE angle, HBRUSH bkBrush);

HBITMAP AFX_EXT_API RotateBitmap(HBITMAP source, DOUBLE angle, POINT ptCenter, HBRUSH bkBrush);

HBITMAP HorizontalContortBitmap(HBITMAP source, DOUBLE angle, HBRUSH bkBrush);

HBITMAP NormalRotateBitmap(HBITMAP source, NORMAL_ROTATE_TYPE type);

HBITMAP AFX_EXT_API CopyBitmap(HBITMAP hBitmapSrc);

BOOL AFX_EXT_API CopyBitmapEx(HBITMAP hBitmapDst, INT nToX, INT nToY, INT nToWidth, INT nToHeight, HBITMAP hBitmapSrc, INT nFromX, INT nFromY);

HBITMAP AFX_EXT_API ClipBitmap(HBITMAP hBitmapSrc, INT nFromX, INT nFromY, INT nWidth, INT nHeight);

HBITMAP AFX_EXT_API StretchBitmap(HBITMAP hBitmapSrc, int nWStretch, int nHStretch, BOOL bRate = TRUE);

HBITMAP ChangeBitmap(HBITMAP hBitmapSrc, int nWidth, int nHeight, COLORREF bk);

HBITMAP AFX_EXT_API Exposure(HBITMAP hBitmap, FLOAT fGamma, LONG lBrightness, LONG lContrast);

#endif