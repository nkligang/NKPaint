
#ifndef __CTOOLPALETTEBAR__
#define __CTOOLPALETTEBAR__

#include "StyleListCtrl.h"

typedef enum
{
	TBS_ENABLE = 0x01,	
} ToolButtonState;

class CToolBoxBar : public CToolBar
{
// Constructor
public:
	CToolBoxBar();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CToolBoxBar();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToolBoxBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateTools(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CStyleListCtrl  m_listStyle;
	UINT			m_nColumns;
	UINT			m_nCurSelToolID;
	UINT			m_uToolState[ID_NP_TOOL_ROUND_RECT - ID_NP_TOOL_POLY_CLIP + 1];
protected:
public:
 	UINT GetCurSelTool(){return m_nCurSelToolID;}
	void SetCurSelTool(UINT index){m_nCurSelToolID = index;}
	void OnChangeTool(UINT index, UINT uStyle = INVALID_STYLE);
	void SetColumns(UINT nColumns);
	UINT GetColumns() {return m_nColumns;};
	UINT GetCurSelToolStyle(){return m_listStyle.GetCurSelToolStyle();}
	void EnableTool(UINT uID, BOOL bEnable = TRUE);
	BOOL IsToolEnabled(UINT uID);

	/*
	 * uImageListType	: TB_SETIMAGELIST,TB_SETHOTIMAGELIST,TB_SETDISABLEDIMAGELIST
	 * uBitmapResource	: resource id of bitmap
	 *
	 * uBitCount		: ILC_COLOR,ILC_COLORDDB,ILC_COLOR4,ILC_COLOR8,ILC_COLOR24,ILC_COLOR32
	 *
	 * rgbMask			:
		1. uBitCount >= 24bits
			RGBTRIPLE* rgb		= (RGBTRIPLE*)(bmBitmap.bmBits);
			COLORREF   rgbMask	= RGB(rgb[0].rgbtRed,rgb[0].rgbtGreen,rgb[0].rgbtBlue);
		2. uBitCount == 8bits(256 Colors)
			RED = 255
	 * nBtnWidth		: width per button
	 */
	BOOL SetImageList(UINT uImageListType, UINT uBitmapResource, CPoint ptMask, int nImageWidth, int nImageCount);
};
#endif
/////////////////////////////////////////////////////////////////////////////
