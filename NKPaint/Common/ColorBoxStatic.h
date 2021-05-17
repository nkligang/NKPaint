#if !defined(AFX_COLORBOXSTATIC_H__2165EE1D_159C_4069_8A0D_0A1E77E6C51C__INCLUDED_)
#define AFX_COLORBOXSTATIC_H__2165EE1D_159C_4069_8A0D_0A1E77E6C51C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBoxStatic.h : header file
//

#define COLOR_BOX_WIDTH		14
#define COLOR_BOX_HEIGHT	14

/////////////////////////////////////////////////////////////////////////////
// CColorBoxStatic window

class CColorBoxStatic : public CStatic
{
// Construction
public:
	CColorBoxStatic();

// Attributes
public:

// Operations
public:
	void SetCurrentColor(COLORREF color);
	void SetCurrentMonoMode(int index);
	void SetParent(CWnd * pParent);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBoxStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorBoxStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorBoxStatic)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClicked();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF	m_colorCur;
	BOOL		m_bColorSet;
	CWnd *		m_pParent;
	CBitmap		m_bitmapCur;
	BOOL		m_bBitmapCreated;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBOXSTATIC_H__2165EE1D_159C_4069_8A0D_0A1E77E6C51C__INCLUDED_)
