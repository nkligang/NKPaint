// FlipRotateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NKPaint.h"
#include "FlipRotateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlipRotateDlg dialog


CDlgFlipRotate::CDlgFlipRotate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFlipRotate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlipRotateDlg)
	m_fRotateAngle = 0.0;
	//}}AFX_DATA_INIT
	m_eFlipRotateType = FRT_FLIP_HORIZONTAL;
}


void CDlgFlipRotate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlipRotateDlg)
	DDX_Text(pDX, IDC_DLG_FLIP_ROTATE_EDIT_ANGLE, m_fRotateAngle);
	DDV_MinMaxDouble(pDX, m_fRotateAngle, -360., 360.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFlipRotate, CDialog)
	//{{AFX_MSG_MAP(CFlipRotateDlg)
	ON_BN_CLICKED(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE, OnRadioAngle)
	ON_BN_CLICKED(IDC_DLG_FLIP_ROTATE_RADIO_HORIZONTAL, OnRadioHorizontal)
	ON_BN_CLICKED(IDC_DLG_FLIP_ROTATE_RADIO_VERTICAL, OnRadioVertical)
	ON_BN_CLICKED(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_ANY, OnRadioAngleAny)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlipRotateDlg message handlers

BOOL CDlgFlipRotate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_HORIZONTAL))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_90))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_90))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_180))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_270))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_EDIT_ANGLE))->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFlipRotate::OnRadioAngle()
{
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_90))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_180))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_270))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_EDIT_ANGLE))->EnableWindow(FALSE);
}

void CDlgFlipRotate::OnRadioHorizontal() 
{
	// TODO: Add your control notification handler code here
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_90))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_180))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_270))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_EDIT_ANGLE))->EnableWindow(FALSE);
}

void CDlgFlipRotate::OnRadioVertical() 
{
	// TODO: Add your control notification handler code here
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_90))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_180))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_270))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_EDIT_ANGLE))->EnableWindow(FALSE);
}

void CDlgFlipRotate::OnOK() 
{
	// TODO: Add extra validation here
	if (((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_HORIZONTAL))->GetCheck())
		m_eFlipRotateType = FRT_FLIP_HORIZONTAL;
	else if (((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_VERTICAL))->GetCheck())
		m_eFlipRotateType = FRT_FLIP_VERTICAL;
	else if (((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_90))->IsWindowEnabled() && ((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_90))->GetCheck())
		m_eFlipRotateType = FRT_ROTATE_90;
	else if (((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_180))->IsWindowEnabled() && ((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_180))->GetCheck())
		m_eFlipRotateType = FRT_ROTATE_180;
	else if (((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_270))->IsWindowEnabled() && ((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_270))->GetCheck())
		m_eFlipRotateType = FRT_ROTATE_270;
	else if (((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_ANY))->IsWindowEnabled() && ((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_ANY))->GetCheck())
		m_eFlipRotateType = FRT_ROTATE_ANY;
	CDialog::OnOK();
}

void CDlgFlipRotate::OnRadioAngleAny() 
{
	// TODO: Add your control notification handler code here
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_90))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_180))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_RADIO_ANGLE_270))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_DLG_FLIP_ROTATE_EDIT_ANGLE))->EnableWindow(TRUE);
}
