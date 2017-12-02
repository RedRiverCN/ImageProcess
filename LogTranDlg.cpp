// LogTranDlg.cpp : ʵ���ļ�
//
//#pragma comment(lib,"gdiplus.lib")
#include "stdafx.h"
#include "ImageProcess.h"
#include "LogTranDlg.h"

#include "Mainfrm.h"
#include "ImageProcessDoc.h"
#include "ImageProcessView.h"
#include <cmath>


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


// CLogTranDlg �Ի���

IMPLEMENT_DYNAMIC(CLogTranDlg, CDialog)

CLogTranDlg::CLogTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogTranDlg::IDD, pParent)
	, m_nPosA(0)
	, m_nPosB(0)
	, m_nA(0)
	, m_nB(0)
{

}

CLogTranDlg::CLogTranDlg(CDib* pDib, CWnd * pParent)
	: CDialog(CLogTranDlg::IDD, pParent)
	, m_nPosA(0)
	, m_nPosB(0)
	, m_nA(0)
	, m_nB(0)
{
	m_pDib = pDib;

}

CLogTranDlg::~CLogTranDlg()
{
	SAFE_DELETE (dlg);
	// cleanup temp buffers
	//SAFE_DELETE(m_lpDestImage);
	GlobalUnlock(hNewDib);
	GlobalFree(hNewDib);
}

void CLogTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDERA, m_nPosA);
	DDX_Slider(pDX, IDC_SLIDERB, m_nPosB);
	DDX_Text(pDX, IDC_EDITA, m_nA);
	DDX_Text(pDX, IDC_EDITB, m_nB);
	DDX_Control(pDX, IDC_PREV, m_stiPrev);
}


BEGIN_MESSAGE_MAP(CLogTranDlg, CDialog)
	ON_EN_CHANGE(IDC_EDITA, &CLogTranDlg::OnEnChangeEdita)
	ON_EN_CHANGE(IDC_EDITB, &CLogTranDlg::OnEnChangeEditb)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SAVE, &CLogTranDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CLogTranDlg::OnBnClickedOk)
END_MESSAGE_MAP()


/******************************************************************************
*	����:		�Ի����ʼ��ʱ��ʾԤ���Ի���
******************************************************************************/
BOOL CLogTranDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_pDib->m_hDib == NULL)
		return FALSE;

	// first convert DIB to 24bpp
	HDIB hTmpDib = NULL;
	WORD wBitCount = DIBBitCount(m_pDib->m_hDib);
	if (wBitCount != 24)
		hTmpDib = ConvertDIBFormat(m_pDib->m_hDib, 24, NULL);
	else
		hTmpDib = CopyHandle(m_pDib->m_hDib);

	GlobalUnlock(m_pDib->m_hDib);
	GlobalFree(m_pDib->m_hDib);
	m_pDib->m_hDib = hTmpDib;


	wDIBWidth = (WORD)m_pDib->GetWidth();
	wDIBHeight = (WORD)m_pDib->GetHeight();
	wBytesPerLine = (WORD)m_pDib->GetWidthBytes();
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	hNewDib = CopyHandle(m_pDib->GetHandle());
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);// ָ��Ŀ��ʵ���ڴ�
	m_lpDestImage = FindDIBBits(lpDIB);// ָ��Ŀ��ʵ����������
	GlobalUnlock(hNewDib);
	// get bits address in DIB
	lpDIBBits = m_pDib->GetBitsPtr();// ָ��ԭʵ����������



	dlg = new CPreviewDlg(wDIBWidth, wDIBHeight, m_lpDestImage, hNewDib, this);

	dlg->m_nType = 1;
	dlg->m_bEnableClose = false;
	dlg->Create(IDD_PREVIEW);
	dlg->ShowWindow(SW_SHOW);

	m_nA = m_nPosA = 0;
	m_nPosA = 50;
	m_nB = m_nPosB = 25;
	UpdateData(FALSE);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDERA))->SetRange(-100, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/******************************************************************************
*	����:		ˢ��Ԥ����
******************************************************************************/
void CLogTranDlg::Refresh()
{
	UpdateData(TRUE);

	// ����Ԥ���Ի���
	//CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	//CImageProcessView* pView = (CImageProcessView*)pMain->GetActiveView();

	LogTrans(lpDIBBits, m_lpDestImage,
		wDIBWidth, wDIBHeight, wBytesPerLine, m_nA, m_nB * 0.001);

	dlg->Refresh();
	
	CDC*	pDC = m_stiPrev.GetDC();
	CRect	rect;
	CDC		memDC;
	CBitmap MemBitmap;

	// ��ȡ��ͼ����
	m_stiPrev.GetClientRect(rect);

	// �豸�������ʼ��
	memDC.CreateCompatibleDC(NULL);

	// ��������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	// ѡȡ�հ�λͼ
	memDC.SelectObject(MemBitmap);
	memDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));

	Graphics graph(memDC.GetSafeHdc());

	// ʹ�ð�ɫ����
	graph.FillRectangles(&SolidBrush(Color::White), 
		&Rect(0, 0, rect.Width(), rect.Height()), 
		1);

	// ����y��
	graph.DrawLine(&Pen(Color::Black), 10, 10, 10, 280);
	graph.DrawLine(&Pen(Color::Black), 10, 10, 5, 15);
	graph.DrawLine(&Pen(Color::Black), 10, 10, 15, 15);

	// ����x��
	graph.DrawLine(&Pen(Color::Black), 10, 280, 290, 280);
	graph.DrawLine(&Pen(Color::Black), 290, 280, 285, 285);
	graph.DrawLine(&Pen(Color::Black), 290, 280, 285, 275);

	// ��������ԭ��
	const WCHAR* strNum;
	Gdiplus::Font font( L"����", 10);

	strNum = L"0";
	graph.DrawString(strNum, -1, &font,
		PointF(8, 290), &SolidBrush(Color::Black));


	graph.DrawString(strNum, -1, &font, 
		PointF(8, 290), &SolidBrush(Color::Black));

	for (int i = 0; i < 256; i += 5)
	{
		if (i % 50 == 0)
			graph.DrawLine(&Pen(Color::Black), 10 + i, 280, 10 + i, 286);
		else if (i % 10 == 0)
			graph.DrawLine(&Pen(Color::Black), 10 + i, 280, 10 + i, 283);
	}

	// ����x��̶�
	strNum = L"50";
	graph.DrawString(strNum, -1, &font, 
		PointF(53, 290), &SolidBrush(Color::Black));
	strNum = L"100";
	graph.DrawString(strNum, -1, &font, 
		PointF(100, 290), &SolidBrush(Color::Black));
	strNum = L"150";
	graph.DrawString(strNum, -1, &font, 
		PointF(150, 290), &SolidBrush(Color::Black));
	strNum = L"200";
	graph.DrawString(strNum, -1, &font, 
		PointF(200, 290), &SolidBrush(Color::Black));
	strNum = L"255";
	graph.DrawString(strNum, -1, &font, 
		PointF(255, 290), &SolidBrush(Color::Black));
	// y��
	graph.DrawString(strNum, -1, &font, 
		PointF(10, 25), &SolidBrush(Color::Black));

	// ת������,��(10, 280)ת��Ϊ����ԭ��
	Matrix matrix1;
	Matrix matrix2(0, 1, 1, 0, 0, 0);	// ת�þ���

	matrix1.Translate(10, 280);
	matrix1.Rotate(270);
	matrix1.Multiply(&matrix2);
	graph.SetTransform(&matrix1);


	// ����ӳ����������
	// ӳ���,����256�ֻҶȱ任���ֵ
	double map[256];

	// ���㵱ǰ���ر任���ֵ
	for (int i = 0; i < 256; i++)
		map[i] = log((double)i + 1.0) / (double)(m_nB*0.001) + m_nA;


	GraphicsPath path;

	path.StartFigure();
	for (int i = 0; i < 255; i++)
		path.AddLine(i, (int)map[i], i+1, (int)map[i+1]);

	graph.DrawPath(&Pen(Color::Black), &path);

	// �����ڴ滭������
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	
	m_stiPrev.ReleaseDC(pDC);
}

/******************************************************************************
*	����:		�ı����Aʱ����Ӧ����
******************************************************************************/
void CLogTranDlg::OnEnChangeEdita()
{
	UpdateData(TRUE);

	if (m_nA < -100)
		m_nA = 100;
	else if (m_nA > 100)
		m_nA = 100;

	m_nPosA = m_nA;
	UpdateData(FALSE);
	Refresh();
	m_bModify = true;
}

/******************************************************************************
*	����:		�ı����Bʱ����Ӧ����
******************************************************************************/
void CLogTranDlg::OnEnChangeEditb()
{
	UpdateData(TRUE);

	if (m_nB < 0)
		m_nB = 0;
	else if (m_nB > 100)
		m_nB = 100;

	m_nPosB = m_nB;
	UpdateData(FALSE);
	Refresh();
	m_bModify = true;
}

/******************************************************************************
*	����:		���黬������Ӧ����
******************************************************************************/
void CLogTranDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	if (pScrollBar == GetDlgItem(IDC_SLIDERA))
		m_nA = m_nPosA;
	else
		m_nB = m_nPosB;

	UpdateData(FALSE);
	Refresh();
	m_bModify = true;

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/******************************************************************************
*	����:		�Ի����ػ溯��
******************************************************************************/
void CLogTranDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	Refresh();
}


/******************************************************************************
*	����:		�����桱��ť��Ӧ����
******************************************************************************/
void CLogTranDlg::OnBnClickedSave()
{
	//CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	//CImageProcessView* pView = (CImageProcessView*)pMain->GetActiveView();

//	pView->OnFileSave();

	m_pDib->LogTransformation(m_nA, m_nB);
	memcpy(lpDIBBits, m_lpDestImage, wBytesPerLine * wDIBHeight);
	m_bModify = false;
}


/******************************************************************************
*	����:		��ȷ������ť����Ӧ����
******************************************************************************/
void CLogTranDlg::OnBnClickedOk()
{
	if (m_bModify)
	{
		if (IDYES == MessageBox("save?", "LogTran", MB_YESNO))
			OnBnClickedSave();
	}

	OnOK();
}