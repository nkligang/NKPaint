#if !defined(AFX_PAINTBOXDLGBAR_H__A30F60A0_C0B8_4E67_A7B4_34943870D530__INCLUDED_)
#define AFX_PAINTBOXDLGBAR_H__A30F60A0_C0B8_4E67_A7B4_34943870D530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaintBoxDlgBar.h : header file
//

#include "NKPaint.h"
#include "ColorBoxStatic.h"
#include "Utils.h"

/////////////////////////////////////////////////////////////////////////////
// CPaintBoxDlgBar dialog

class AFX_EXT_CLASS CPaintBoxDlgBar : public CDialogBar
{
// Construction
public:
	CPaintBoxDlgBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPaintBoxDlgBar)
	enum { IDD = IDD_DLG_COLOR };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaintBoxDlgBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPaintBoxDlgBar)
	afx_msg LONG OnInitDialog(UINT, LONG);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnSetBackground(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetForeground(WPARAM wParam, LPARAM lParam);
	//lParam define
	#define TYPE_CHANGE	0x00
	#define TYPE_SET	0x01
	afx_msg LRESULT OnModifyBitCountType(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CColorBoxStatic m_staticBoxes[28];

	CStatic			m_staticFG;
	COLORREF		m_colorBackground;
	COLORREF		m_colorForeground;
	CBitmap			m_bitmapForeBack;
	BOOL			m_bBitmapCreated;

	UINT			m_uBitCount;
protected:
public:
	// dwFlag define
	#define	SET_BACK_GROUND_COLOR	0x01
	#define SET_FORE_GROUND_COLOR	0x02
	void SetForeBackColor(DWORD dwFlag, COLORREF fg, COLORREF bg);
	void SetBoxColor(int index, COLORREF color);

	COLORREF GetForegoundColor(){return m_colorForeground;}
	COLORREF GetBackgoundColor(){return m_colorBackground;}

	UINT GetBitCount() {return m_uBitCount;}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAINTBOXDLGBAR_H__A30F60A0_C0B8_4E67_A7B4_34943870D530__INCLUDED_)
