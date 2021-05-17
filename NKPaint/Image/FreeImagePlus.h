#ifndef __FREEIMAGE_PLUS_H_INCLUDED__
#define __FREEIMAGE_PLUS_H_INCLUDED__

void FreeImage_Init();

HRESULT FreeImage_LoadImageFromFile(HBITMAP * pHBitmap, LPCTSTR lpszPathName);

HRESULT FreeImage_SaveImageToFile(HBITMAP hBitmap, LPCTSTR lpszPathName);

void FreeImage_DeInit();

HBITMAP FreeImage_Exposure(HBITMAP hBitmap, FLOAT fGamma, LONG lBrightness, LONG lContrast);

#endif