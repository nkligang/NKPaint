#if !defined(AFX_ATTR_H__6A0A7C9C_F2E7_4360_9EF0_64FD2891BD12__INCLUDED_)
#define AFX_ATTR_H__6A0A7C9C_F2E7_4360_9EF0_64FD2891BD12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AttributeDlg.h : header file
//
#include "Utils.h"
#include "AdvanceDraw.h"

/////////////////////////////////////////////////////////////////////////////
// CAttributeDlg dialog

#define SYSTEM_DEFAULT_WIDTH	512
#define SYSTEM_DEFAULT_HEIGHT	384

class CDlgAttribute : public CDialog
{
// Construction
public:
	CDlgAttribute(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAttributeDlg)
	enum { IDD = IDD_DLG_ATTRIBUTE };
	float	m_fWidth;
	float	m_fHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAttributeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAttributeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioCm();
	afx_msg void OnRadioInch();
	afx_msg void OnRadioPixel();
	afx_msg void OnButtonDefault();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	UNIT_TYPE		m_eCurUnitType;
	UINT			m_uBitCount;
protected:
	BOOL GetLastSaveFileInfo(LPTSTR lpStrFileName, LONG & dwDiskSpace);
public:
	CSize GetSize();
	void SetSize(CSize size);
	void SetSize(int nWidth, int nHeight);
	void SetBitCount(UINT uBitCount);
	UINT GetBitCount();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATTR_H__6A0A7C9C_F2E7_4360_9EF0_64FD2891BD12__INCLUDED_)
