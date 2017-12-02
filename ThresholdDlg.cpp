// ThresholdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ThresholdDlg.h"
//#include "GrayOperator.h"
#include "Mainfrm.h"
#include "ImageProcessDoc.h"
#include "ImageProcessView.h"

#define SAFE_DELETE(p)	\
{						\
	if ((p) != NULL)	\
	{					\
		delete (p);		\
		(p) = NULL;		\
	}					\
}

#define SAFE_DELETE_ARRAY(p)	\
{								\
	if ((p) != NULL)			\
	{							\
		delete[] (p);			\
		(p) = NULL;				\
	}							\
}


// CThresholdDlg �Ի���

IMPLEMENT_DYNAMIC(CThresholdDlg, CDialog)

CThresholdDlg::CThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThresholdDlg::IDD, pParent)
	, m_nNum(120)
	, m_nPos(0)
{
	m_bModify = false;
}

CThresholdDlg::~CThresholdDlg()
{
	SAFE_DELETE (dlg);
}

void CThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NUM, m_nNum);
	DDX_Control(pDX, IDC_SLIDER, m_SliderNum);
	DDX_Slider(pDX, IDC_SLIDER, m_nPos);
}


BEGIN_MESSAGE_MAP(CThresholdDlg, CDialog)
	ON_EN_CHANGE(IDC_NUM, &CThresholdDlg::OnEnChangeNum)
	ON_BN_CLICKED(IDC_SAVE, &CThresholdDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CThresholdDlg::OnBnClickedOk)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


/******************************************************************************
*	����:		�Ի����ʼ��ʱ��ʾԤ���Ի���
******************************************************************************/
BOOL CThresholdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	dlg = new CPreviewDlg(this);

	dlg->m_nType = 1;
	dlg->m_bEnableClose = false;
	dlg->Create(IDD_PREVIEW);
	dlg->ShowWindow(SW_SHOW);

	m_SliderNum.SetRange(0, 255);
	m_nPos = 120;
	UpdateData(FALSE);

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/******************************************************************************
*	����:		������Ӧ����
******************************************************************************/
void CThresholdDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	m_nNum = m_nPos;

	UpdateData(FALSE);
	Refresh();
	m_bModify = true;

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/******************************************************************************
*	����:		ˢ��Ԥ������
******************************************************************************/
void CThresholdDlg::Refresh()
{
	UpdateData(TRUE);

	//CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	//CImageProcessView* pView = (CImageProcessView*)pMain->GetActiveView();

	//ThresholdTrans(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
	//			   pView->m_nPicWidth, pView->m_nPicHeight, m_nNum);

	dlg->Refresh();
}

/******************************************************************************
*	����:		��ֵ���޸�ʱ����Ӧ����
******************************************************************************/
void CThresholdDlg::OnEnChangeNum()
{
	UpdateData(TRUE);

	if (m_nNum < 0)
		m_nNum = 0;
	else if (m_nNum > 255)
		m_nNum = 255;

	m_nPos = m_nNum;
	UpdateData(FALSE);
	Refresh();
	m_bModify = true;
}

void CThresholdDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CImageProcessView* pView = (CImageProcessView*)pMain->GetActiveView();

	//pView->OnFileSave();
	m_bModify = false;
}

void CThresholdDlg::OnBnClickedOk()
{
	if (m_bModify)
	{
		if (IDYES == MessageBox("ͼ���Ѿ����ģ��Ƿ񱣴棿", "Magic House", MB_YESNO))
			OnBnClickedSave();
	}
	OnOK();
}