#pragma once

class CZoomDlg : public CDialog
{
	// Construction
public:
	CZoomDlg(int nMin, int nMax, int nDef, CWnd* pParent = NULL);   // standard constructor

																		 // Dialog Data
																		 //{{AFX_DATA(CZoomDlg)
	enum { IDD = IDD_ZOOM};
	CSliderCtrl	m_Streng;
	CString	m_strValue;
	int		m_nStreng;
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	int m_nMax;
	int m_nMin;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
														//}}AFX_VIRTUAL

														// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CZoomDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};