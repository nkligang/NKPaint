#ifndef __GDI_PIUS_H_INCLUDED__
#define __GDI_PIUS_H_INCLUDED__


void GdiPlus_Init();

HRESULT GdiPlus_LoadImageFromFile(HBITMAP * pHBitmap, LPCTSTR pszFileName);

HRESULT GdiPlus_SaveImageToFile(HBITMAP hBitmap, LPCTSTR pszFileName);

void GdiPlus_DeInit();

HBITMAP GdiPlus_Exposure(HBITMAP hBitmap, FLOAT fGamma, LONG lBrightness, LONG lContrast);

#endif