#if !defined(AFX_BASEFRAME_H__9A10505E_1680_41F8_8317_6128419B1AE8__INCLUDED_)
#define AFX_BASEFRAME_H__9A10505E_1680_41F8_8317_6128419B1AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseFrame.h : header file
//

#include "PaintBoxDlgBar.h"
#include "ToolBoxBar.h"
#include "MapDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CBaseFrame frame
class AFX_EXT_CLASS CBaseFrame : public CFrameWnd
{
protected: // create from serialization only
	CBaseFrame();
	DECLARE_DYNCREATE(CBaseFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseFrame();

protected:  // control bar embedded members
  CPaintBoxDlgBar m_dlgPaintBox;
	CToolBoxBar			m_barToolBox;
	CStatusBar			m_wndStatusBar;
	CToolBar			  m_wndToolBar;
	CDlgMap				  m_dlgZoomMap;
	HACCEL				  m_hAccel;
	BOOL				    m_bUsePaintMenu;

// Generated message map functions
protected:
	//{{AFX_MSG(CBaseFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewToolbarTool();
	afx_msg void OnUpdateViewToolbarTool(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarColor();
	afx_msg void OnUpdateViewToolbarColor(CCmdUI* pCmdUI);
	afx_msg void OnSelectTool(UINT id = 0);
	afx_msg void OnUpdateViewShowMap(CCmdUI* pCmdUI);
	afx_msg void OnViewShowMap();
	//}}AFX_MSG
	afx_msg LRESULT OnBitmapUpdate(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
protected:
	void UpdateBar(CControlBar * pDockedBar);
	BOOL CreateMenu(UINT nResourceID);
	BOOL LoadAccelerators(UINT nResourceID);
	BOOL PFMergeAccelerator(HACCEL& hDestination, HACCEL hToMerge);

public:
	inline CStatusBar& GetStatusBar() { return m_wndStatusBar; }
	CPaintBoxDlgBar * GetPaintBox(){return &m_dlgPaintBox;};
 	UINT GetCurSelTool() {return m_barToolBox.GetCurSelTool();}
	void SetCurSelTool(UINT uID, UINT uStyle = INVALID_STYLE);
	UINT GetCurSelToolStyle() {return m_barToolBox.GetCurSelToolStyle();}
	COLORREF GetBkColor() {return m_dlgPaintBox.GetBackgoundColor();}
	void SetBkColor(COLORREF bkColor) {m_dlgPaintBox.SetForeBackColor(SET_BACK_GROUND_COLOR, bkColor, bkColor);}
	COLORREF GetFgColor() {return m_dlgPaintBox.GetForegoundColor();}
	void SetFgColor(COLORREF fgColor) {m_dlgPaintBox.SetForeBackColor(SET_FORE_GROUND_COLOR, fgColor, fgColor);}
	void ShowMousePos(CPoint pt);
	void EnableTool(UINT uID, BOOL bEnable = TRUE){m_barToolBox.EnableTool(uID, bEnable);}
	BOOL IsToolEnabled(UINT uID) { return m_barToolBox.IsToolEnabled(uID);}
	BOOL ShowPaintBox(BOOL bVisiable = TRUE) {	return m_dlgPaintBox.ShowWindow(bVisiable ? SW_SHOW : SW_HIDE);}
	BOOL ShowToolsBox(BOOL bVisiable = TRUE) {	return m_barToolBox.ShowWindow(bVisiable ? SW_SHOW : SW_HIDE);}
	void UsePaintMenu(BOOL bUsePaintMenu = TRUE) { m_bUsePaintMenu = bUsePaintMenu;}
	BOOL UseAccelerators();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEFRAME_H__9A10505E_1680_41F8_8317_6128419B1AE8__INCLUDED_)
