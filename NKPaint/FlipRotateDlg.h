#if !defined(AFX_FLIPROTATEDLG_H__C43ADA8C_6BA3_4FB3_A2C0_37AD30E56F47__INCLUDED_)
#define AFX_FLIPROTATEDLG_H__C43ADA8C_6BA3_4FB3_A2C0_37AD30E56F47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlipRotateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlipRotateDlg dialog
typedef enum {
	FRT_FLIP_HORIZONTAL = 0,
	FRT_FLIP_VERTICAL,
	FRT_ROTATE_90,
	FRT_ROTATE_180,
	FRT_ROTATE_270,
	FRT_ROTATE_ANY,
} FlipRotateType;

class CDlgFlipRotate : public CDialog
{
// Construction
public:
	CDlgFlipRotate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFlipRotateDlg)
	enum { IDD = IDD_DLG_FLIP_ROTATE };
	double	m_fRotateAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlipRotateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlipRotateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioAngle();
	afx_msg void OnRadioHorizontal();
	afx_msg void OnRadioVertical();
	virtual void OnOK();
	afx_msg void OnRadioAngleAny();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	FlipRotateType m_eFlipRotateType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLIPROTATEDLG_H__C43ADA8C_6BA3_4FB3_A2C0_37AD30E56F47__INCLUDED_)
