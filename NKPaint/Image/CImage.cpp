// Image.cpp: implementation of the CImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CImage.h"

#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "FreeImagePlus.h"
#include "CxImagePlus.h"
#include "GdiPlus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef void (*ImageInitializer) ();
typedef void (*ImageDeInitializer) ();
typedef HRESULT (*ImageReader) (HBITMAP *, LPCTSTR);
typedef HRESULT (*ImageWriter) (HBITMAP, LPCTSTR);
typedef HBITMAP (*ImageExposure) (HBITMAP, FLOAT, LONG, LONG);

typedef struct 
{
	ImageInitializer init;
	ImageDeInitializer deinit;
	ImageReader reader;
	ImageWriter writer;
	ImageExposure exposure;
} ImagePlus;

static ImagePlus g_ImagePlus[] = 
{
	{
		FreeImage_Init,
		FreeImage_DeInit,
		FreeImage_LoadImageFromFile,
		FreeImage_SaveImageToFile,
		FreeImage_Exposure,
	},
	{
		CxImage_Init,
		CxImage_DeInit,
		CxImage_LoadImageFromFile,
		CxImage_SaveImageToFile,
		CxImage_Exposure,
	},
	{
		GdiPlus_Init,
		GdiPlus_DeInit,
		GdiPlus_LoadImageFromFile,
		GdiPlus_SaveImageToFile,
		GdiPlus_Exposure,
	},
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static CImage::CDCCache s_cache;

#pragma warning( disable : 4273 )

static int ComputePitch( int nWidth, int nBPP )
{
	return( (((nWidth*nBPP)+31)/32)*4 );
}


CImage::CDCCache::CDCCache() throw()
{
	int iDC;

	for( iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		m_ahDCs[iDC] = NULL;
	}
}

CImage::CDCCache::~CDCCache() throw()
{
	int iDC;

	for( iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		if( m_ahDCs[iDC] != NULL )
		{
			::DeleteDC( m_ahDCs[iDC] );
		}
	}
}

inline void* WINAPI InterlockedExchangePointer(void** pp, void* pNew) throw()
{
	return( reinterpret_cast<void*>(static_cast<LONG>(::InterlockedExchange(reinterpret_cast<LONG*>(pp), static_cast<LONG>(reinterpret_cast<LONG>(pNew))))) );
}

HDC CImage::CDCCache::GetDC() throw()
{
	HDC hDC;

	for( int iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		hDC = static_cast< HDC >( InterlockedExchangePointer( reinterpret_cast< void** >(&m_ahDCs[iDC]), NULL ) );
		if( hDC != NULL )
		{
			return( hDC );
		}
	}

	hDC = ::CreateCompatibleDC( NULL );

	return( hDC );
}

void CImage::CDCCache::ReleaseDC( HDC hDC ) throw()
{
	for( int iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		HDC hOldDC;

		hOldDC = static_cast< HDC >( InterlockedExchangePointer( reinterpret_cast< void** >(&m_ahDCs[iDC]), hDC ) );
		if( hOldDC == NULL )
		{
			return;
		}
		else
		{
			hDC = hOldDC;
		}
	}
	if( hDC != NULL )
	{
		::DeleteDC( hDC );
	}
}

CImage::CImage() throw() :
	m_hBitmap( NULL ),
	m_pBits( NULL ),
	m_hDC( NULL ),
	m_nDCRefCount( 0 ),
	m_hOldBitmap( NULL ),
	m_nWidth( 0 ),
	m_nHeight( 0 ),
	m_nPitch( 0 ),
	m_nBPP( 0 ),
	m_iTransparentColor( -1 ),
	m_bHasAlphaChannel( false ),
	m_bIsDIBSection( false )
{
	for (INT iPlus = 0; iPlus < sizeof(g_ImagePlus) / sizeof(ImagePlus); iPlus++)
	{
		(*g_ImagePlus[iPlus].init)();
	}
}

CImage::~CImage() throw()
{
	Destroy();
	for (INT iPlus = 0; iPlus < sizeof(g_ImagePlus) / sizeof(ImagePlus); iPlus++)
	{
		(*g_ImagePlus[iPlus].deinit)();
	}
}

HDC CImage::GetDC() const throw()
{
	ASSERT( m_hBitmap != NULL );

	m_nDCRefCount++;
	if( m_hDC == NULL )
	{
		m_hDC = s_cache.GetDC();
		m_hOldBitmap = HBITMAP( ::SelectObject( m_hDC, m_hBitmap ) );
	}

	return( m_hDC );
}

void CImage::Destroy() throw()
{
	HBITMAP hBitmap;

	if( m_hBitmap != NULL )
	{
		hBitmap = Detach();
		::DeleteObject( hBitmap );
	}
}

void CImage::Attach( HBITMAP hBitmap, DIBOrientation eOrientation ) throw()
{
	ASSERT( m_hBitmap == NULL );
	ASSERT( hBitmap != NULL );

	m_hBitmap = hBitmap;

	UpdateBitmapInfo( eOrientation );
}

HBITMAP CImage::Detach() throw()
{
	HBITMAP hBitmap;

	ASSERT( m_hBitmap != NULL );
	ASSERT( m_hDC == NULL );

	hBitmap = m_hBitmap;
	m_hBitmap = NULL;
	m_pBits = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBPP = 0;
	m_nPitch = 0;
	m_iTransparentColor = -1;
	m_bHasAlphaChannel = false;
	m_bIsDIBSection = false;

	return( hBitmap );
}

bool CImage::IsNull() const throw()
{
	return( m_hBitmap == NULL );
}

void CImage::UpdateBitmapInfo( DIBOrientation eOrientation )
{
	DIBSECTION dibsection;
	int nBytes;

	nBytes = ::GetObject( m_hBitmap, sizeof( DIBSECTION ), &dibsection );
	if( nBytes == sizeof( DIBSECTION ) )
	{
		m_bIsDIBSection = true;
		m_nWidth = dibsection.dsBmih.biWidth;
		m_nHeight = abs( dibsection.dsBmih.biHeight );
		m_nBPP = dibsection.dsBmih.biBitCount;
		m_nPitch = ComputePitch( m_nWidth, m_nBPP );
		m_pBits = dibsection.dsBm.bmBits;
		if( eOrientation == DIBOR_DEFAULT )
		{
			eOrientation = (dibsection.dsBmih.biHeight > 0) ? DIBOR_BOTTOMUP : DIBOR_TOPDOWN;
		}
		if( eOrientation == DIBOR_BOTTOMUP )
		{
			m_pBits = LPBYTE( m_pBits )+((m_nHeight-1)*m_nPitch);
			m_nPitch = -m_nPitch;
		}
	}
	else
	{
		// Non-DIBSection
		ASSERT( nBytes == sizeof( BITMAP ) );
		m_bIsDIBSection = false;
		m_nWidth = dibsection.dsBm.bmWidth;
		m_nHeight = dibsection.dsBm.bmHeight;
		m_nBPP = dibsection.dsBm.bmBitsPixel;
		m_nPitch = 0;
		m_pBits = 0;
	}
	m_iTransparentColor = -1;
	m_bHasAlphaChannel = false;
}

void CImage::ReleaseDC() const throw()
{
	HBITMAP hBitmap;

	ASSERT( m_hDC != NULL );

	m_nDCRefCount--;
	if( m_nDCRefCount == 0 )
	{
		hBitmap = HBITMAP( ::SelectObject( m_hDC, m_hOldBitmap ) );
		ASSERT( hBitmap == m_hBitmap );
		s_cache.ReleaseDC( m_hDC );
		m_hDC = NULL;
	}
}

int CImage::GetWidth() const
{
	ASSERT( m_hBitmap != NULL );

	return( m_nWidth );
}

int CImage::GetHeight() const
{
	ASSERT( m_hBitmap != NULL );

	return( m_nHeight );
}

CSize CImage::GetSize() const
{
	ASSERT( m_hBitmap != NULL );

	return CSize( m_nWidth, m_nHeight );
}

BOOL CImage::StretchBlt( HDC hDestDC, int xDest, int yDest, 
	int nDestWidth, int nDestHeight, DWORD dwROP ) const
{
	return( StretchBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight, 0, 0, 
		m_nWidth, m_nHeight, dwROP ) );
}

BOOL CImage::StretchBlt( HDC hDestDC, const RECT& rectDest, 
	DWORD dwROP ) const
{
	return( StretchBlt( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, 0, 0, m_nWidth, m_nHeight, 
		dwROP ) );
}

BOOL CImage::StretchBlt( HDC hDestDC, int xDest, int yDest, 
	int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, 
	int nSrcHeight, DWORD dwROP ) const
{
	BOOL bResult;

	ASSERT( m_hBitmap != NULL );
	ASSERT( hDestDC != NULL );

	GetDC();

	bResult = ::StretchBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight, m_hDC,
		xSrc, ySrc, nSrcWidth, nSrcHeight, dwROP );

	ReleaseDC();

	return( bResult );
}

BOOL CImage::StretchBlt( HDC hDestDC, const RECT& rectDest, 
	const RECT& rectSrc, DWORD dwROP ) const
{
	return( StretchBlt( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, rectSrc.left, rectSrc.top, 
		rectSrc.right-rectSrc.left, rectSrc.bottom-rectSrc.top, dwROP ) );
}

HBITMAP CopyBitmap(HBITMAP hBitmapSrc)
{
	HDC hDstDC;
	HDC hSrcDC;
	BITMAP bitmap;
	HBITMAP hBitmapDst;
	HBITMAP hOldBitmap,hOldBitmap2;

	hDstDC = ::CreateCompatibleDC(NULL);
	hSrcDC = ::CreateCompatibleDC(NULL);

	::GetObject(hBitmapSrc,sizeof(bitmap),&bitmap);
	hOldBitmap2 = (HBITMAP)::SelectObject(hSrcDC, hBitmapSrc);
	hBitmapDst  = ::CreateCompatibleBitmap(hSrcDC,bitmap.bmWidth,bitmap.bmHeight);
	hOldBitmap	= (HBITMAP)::SelectObject(hDstDC, hBitmapDst);
	::GetObject(hDstDC,sizeof(bitmap),&bitmap);
	
	::BitBlt(hDstDC, 0, 0, bitmap.bmWidth,bitmap.bmHeight, hSrcDC, 0, 0, SRCCOPY);

	::SelectObject(hDstDC,hOldBitmap);
	::SelectObject(hSrcDC ,hOldBitmap2);

	::DeleteDC(hDstDC);
	::DeleteDC(hSrcDC);
	
	return hBitmapDst;
}

HRESULT CImage::CreateFromHBITMAP(HBITMAP hBitmap)
{
	ASSERT(hBitmap);

	Destroy();
	m_hBitmap = CopyBitmap(hBitmap);
	if(m_hBitmap != NULL)
	{
		BITMAP bitmap;
		::GetObject(m_hBitmap,sizeof(bitmap),&bitmap);
		m_nWidth = bitmap.bmWidth;
		m_nHeight = bitmap.bmHeight;
		return ( NOERROR );
	}
	return( E_FAIL );
}


HRESULT CImage::Load( LPCTSTR pszFileName) throw()
{
	HRESULT hRet = E_NOTIMPL;
	for (INT iPlus = 0; iPlus < sizeof(g_ImagePlus) / sizeof(ImagePlus); iPlus++)
	{
		hRet = (*g_ImagePlus[iPlus].reader)(&m_hBitmap, pszFileName);
		if (hRet == NOERROR)
			break;
	}
	if (hRet == NOERROR)
	{
		BITMAP bitmap;
		::GetObject(m_hBitmap,sizeof(bitmap),&bitmap);
		m_nWidth = bitmap.bmWidth;
		m_nHeight = bitmap.bmHeight;
		return (NOERROR);
	}
	return (hRet);
}

HRESULT CImage::Save( LPCTSTR pszFileName) const throw()
{
	HRESULT hRet = E_NOTIMPL;
	for (INT iPlus = 0; iPlus < sizeof(g_ImagePlus) / sizeof(ImagePlus); iPlus++)
	{
		hRet = (*g_ImagePlus[iPlus].writer)(m_hBitmap, pszFileName);
		if (hRet == NOERROR)
			break;
	}
	return hRet;
}

BOOL CImage::Exposure(FLOAT fGamma, LONG lBrightness, LONG lContrast)
{
	HRESULT hRet = E_NOTIMPL;
	HBITMAP hBitmap = NULL;
	for (INT iPlus = 0; iPlus < sizeof(g_ImagePlus) / sizeof(ImagePlus); iPlus++)
	{
		hBitmap = (*g_ImagePlus[iPlus].exposure)(m_hBitmap, fGamma, lBrightness, lContrast);
		if (hBitmap != NULL)
			break;
	}
	if (hBitmap != NULL)
	{
		hRet = CreateFromHBITMAP(hBitmap);
		::DeleteObject(hBitmap);
	}

	return (FALSE);
}

BOOL AddFileExt(LPCSTR szExt, LPTSTR lpszExtFilter)
{
	TCHAR szFileExt[MAX_PATH];
	int nSize = 0;
	TCHAR * pExt;

	lstrcpy(szFileExt, szExt);
	strlwr(szFileExt);

	pExt = lpszExtFilter;
	while( *pExt )
	{
		nSize += lstrlen( pExt ) + 1;
		pExt  += lstrlen( pExt ) + 1;
	}
	if (nSize + lstrlen( szFileExt ) + 1 > MAX_PATH)
		return FALSE;
	lstrcpy(pExt, szFileExt);
	return TRUE;
}

int GetSupportedFileExt(LPTSTR lpszStringBuf, int nMaxCount)
{
	int nCount = 0;
	memset(lpszStringBuf, 0, sizeof(lpszStringBuf));
/*#if CXIMAGE_SUPPORT_BMP
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("bmp"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_GIF
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("gif"), lpszStringBuf);
	nCount += 4;
#endif*/
//#if CXIMAGE_SUPPORT_JPG
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("jpg"), lpszStringBuf);
	nCount += 4;
	ASSERT(nCount + 5 < nMaxCount);
	AddFileExt(_T("jpeg"), lpszStringBuf);
	nCount += 5;
//#endif
/*#if CXIMAGE_SUPPORT_PNG
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("png"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_MNG
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("mng"), lpszStringBuf);
	nCount += 4;
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("jng"), lpszStringBuf);
	nCount += 4;
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("png"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_ICO
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("ico"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_TIF
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("tif"), lpszStringBuf);
	nCount += 4;
	ASSERT(nCount + 5 < nMaxCount);
	AddFileExt(_T("tiff"), lpszStringBuf);
	nCount += 5;
#endif
#if CXIMAGE_SUPPORT_TGA
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("tga"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_PCX
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("pcx"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_WBMP
	ASSERT(nCount + 5 < nMaxCount);
	AddFileExt(_T("wbmp"), lpszStringBuf);
	nCount += 5;
#endif
#if CXIMAGE_SUPPORT_WMF
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("wmf"), lpszStringBuf);
	nCount += 4;
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("emf"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_J2K
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("j2k"), lpszStringBuf);
	nCount += 4;
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("jp2"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_JBG
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("jbg"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_JP2
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("j2k"), lpszStringBuf);
	nCount += 4;
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("jp2"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_JPC
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("j2c"), lpszStringBuf);
	nCount += 4;
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("jpc"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_PGX
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("pgx"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_PNM
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("pnm"), lpszStringBuf);
	nCount += 4;
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("pgm"), lpszStringBuf);
	nCount += 4;
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("ppm"), lpszStringBuf);
	nCount += 4;
#endif
#if CXIMAGE_SUPPORT_RAS
	ASSERT(nCount + 4 < nMaxCount);
	AddFileExt(_T("ras"), lpszStringBuf);
	nCount += 4;
#endif*/
	return nCount;
}

BOOL IsFileSupported(LPCTSTR lpszFileName, LPCTSTR lpszExtFilter)
{
	TCHAR szFileName[MAX_PATH];
	const TCHAR * pExt;
	const TCHAR * nametype;		//type part of name
	pExt = lpszExtFilter;
	if (*pExt == NULL) return TRUE;

	lstrcpy(szFileName, lpszFileName);
	strlwr(szFileName);
								//find extension
	nametype = strrchr (szFileName, '.');
	if (nametype != NULL)
		nametype++;             //ptr to extension
	else
		return FALSE;

	while( *pExt )
	{
		if (lstrcmpi (pExt, nametype) == 0)
			return TRUE;
		pExt += lstrlen( pExt ) + 1;
	}
	return FALSE;
}
