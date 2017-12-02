// ZoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ZoomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CZoomDlg::CZoomDlg(int nMin, int nMax, int nDef, CWnd * pParent /*=NULL*/)
	: CDialog(CZoomDlg::IDD, pParent)
{//{{AFX_DATA_INIT(CZoomDlg)
	m_strValue = _T("");
	m_nStreng = nDef;
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMax = nMax;
	m_nMin = nMin;
}


void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomDlg)
	DDX_Control(pDX, IDC_SLIDER1, m_Streng);
	DDX_Text(pDX, IDC_VALUE, m_strValue);
	DDX_Slider(pDX, IDC_SLIDER1, m_nStreng);
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
	//{{AFX_MSG_MAP(CZoomDlg)
	ON_WM_HSCROLL()
	// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg message handlers

BOOL CZoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Streng.SetRange(m_nMin, m_nMax, TRUE);
	m_strValue.Format("%d", m_nStreng);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CZoomDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData();
	m_strValue.Format("%d", m_nStreng);
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
