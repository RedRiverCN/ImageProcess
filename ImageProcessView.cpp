// ImageProcessView.cpp : implementation of the CImageProcessView class
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "MainFrm.h"

#include "ConvoluteDlg.h"
#include "EmbossmentDlg.h"
#include "SpreadDlg.h"
#include "ZoomDlg.h"
#include "LogTranDlg.h"
#include "StretchDlg.h"
#include "PreviewDlg.h"
#include "HistogramDlg.h"
#include "ThresholdDlg.h"
#include "ImageProcessDoc.h"
#include "ImageProcessView.h"
#include <string>

#include <iostream>
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView

IMPLEMENT_DYNCREATE(CImageProcessView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessView)
	ON_COMMAND(IDM_EAST, OnEast)
	ON_UPDATE_COMMAND_UI(IDM_EAST, OnUpdateEast)
	ON_COMMAND(IDM_HF1, OnHf1)
	ON_UPDATE_COMMAND_UI(IDM_HF1, OnUpdateHf1)
	ON_COMMAND(IDM_HF2, OnHf2)
	ON_UPDATE_COMMAND_UI(IDM_HF2, OnUpdateHf2)
	ON_COMMAND(IDM_HF3, OnHf3)
	ON_UPDATE_COMMAND_UI(IDM_HF3, OnUpdateHf3)
	ON_COMMAND(IDM_HORZ, OnHorz)
	ON_UPDATE_COMMAND_UI(IDM_HORZ, OnUpdateHorz)
	ON_COMMAND(IDM_LAP1, OnLap1)
	ON_UPDATE_COMMAND_UI(IDM_LAP1, OnUpdateLap1)
	ON_COMMAND(IDM_LAP2, OnLap2)
	ON_UPDATE_COMMAND_UI(IDM_LAP2, OnUpdateLap2)
	ON_COMMAND(IDM_LAP3, OnLap3)
	ON_UPDATE_COMMAND_UI(IDM_LAP3, OnUpdateLap3)
	ON_COMMAND(IDM_LAP4, OnLap4)
	ON_UPDATE_COMMAND_UI(IDM_LAP4, OnUpdateLap4)
	ON_COMMAND(IDM_LF1, OnLf1)
	ON_UPDATE_COMMAND_UI(IDM_LF1, OnUpdateLf1)
	ON_COMMAND(IDM_LF2, OnLf2)
	ON_UPDATE_COMMAND_UI(IDM_LF2, OnUpdateLf2)
	ON_COMMAND(IDM_LF3, OnLf3)
	ON_UPDATE_COMMAND_UI(IDM_LF3, OnUpdateLf3)
	ON_COMMAND(IDM_NORTH, OnNorth)
	ON_UPDATE_COMMAND_UI(IDM_NORTH, OnUpdateNorth)
	ON_COMMAND(IDM_NORTHEAST, OnNortheast)
	ON_UPDATE_COMMAND_UI(IDM_NORTHEAST, OnUpdateNortheast)
	ON_COMMAND(IDM_NORTHWEST, OnNorthwest)
	ON_UPDATE_COMMAND_UI(IDM_NORTHWEST, OnUpdateNorthwest)
	ON_COMMAND(IDM_SOUTH, OnSouth)
	ON_UPDATE_COMMAND_UI(IDM_SOUTH, OnUpdateSouth)
	ON_COMMAND(IDM_SOUTHEAST, OnSoutheast)
	ON_UPDATE_COMMAND_UI(IDM_SOUTHEAST, OnUpdateSoutheast)
	ON_COMMAND(IDM_SOUTHWEST, OnSouthwest)
	ON_UPDATE_COMMAND_UI(IDM_SOUTHWEST, OnUpdateSouthwest)
	ON_COMMAND(IDM_VERT, OnVert)
	ON_UPDATE_COMMAND_UI(IDM_VERT, OnUpdateVert)
	ON_COMMAND(IDM_VERTHORZ, OnVerthorz)
	ON_UPDATE_COMMAND_UI(IDM_VERTHORZ, OnUpdateVerthorz)
	ON_COMMAND(IDM_WEST, OnWest)
	ON_UPDATE_COMMAND_UI(IDM_WEST, OnUpdateWest)
	ON_COMMAND(IDM_SOBEL, OnSobel)
	ON_UPDATE_COMMAND_UI(IDM_SOBEL, OnUpdateSobel)
	ON_COMMAND(IDM_HOUGH, OnHough)
	ON_UPDATE_COMMAND_UI(IDM_HOUGH, OnUpdateHough)
	ON_COMMAND(IDM_MEDIANFILTER, OnMedianfilter)
	ON_UPDATE_COMMAND_UI(IDM_MEDIANFILTER, OnUpdateMedianfilter)
	ON_COMMAND(IDM_CLOSE_H, OnCloseH)
	ON_UPDATE_COMMAND_UI(IDM_CLOSE_H, OnUpdateCloseH)
	ON_COMMAND(IDM_CLOSE_V, OnCloseV)
	ON_UPDATE_COMMAND_UI(IDM_CLOSE_V, OnUpdateCloseV)
	ON_COMMAND(IDM_DILATION_H, OnDilationH)
	ON_UPDATE_COMMAND_UI(IDM_DILATION_H, OnUpdateDilationH)
	ON_COMMAND(IDM_DILATION_V, OnDilationV)
	ON_UPDATE_COMMAND_UI(IDM_DILATION_V, OnUpdateDilationV)
	ON_COMMAND(IDM_EROSION_H, OnErosionH)
	ON_UPDATE_COMMAND_UI(IDM_EROSION_H, OnUpdateErosionH)
	ON_COMMAND(IDM_EROSION_V, OnErosionV)
	ON_UPDATE_COMMAND_UI(IDM_EROSION_V, OnUpdateErosionV)
	ON_COMMAND(IDM_OPEN_H, OnOpenH)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_H, OnUpdateOpenH)
	ON_COMMAND(IDM_OPEN_V, OnOpenV)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_V, OnUpdateOpenV)
	ON_COMMAND(IDM_THIN, OnThin)
	ON_UPDATE_COMMAND_UI(IDM_THIN, OnUpdateThin)
	ON_COMMAND(IDM_FFT, OnFft)
	ON_UPDATE_COMMAND_UI(IDM_FFT, OnUpdateFft)
	ON_COMMAND(IDM_DCT, OnDct)
	ON_UPDATE_COMMAND_UI(IDM_DCT, OnUpdateDct)
	ON_UPDATE_COMMAND_UI(IDM_REVERSE, OnUpdateReverse)
	ON_COMMAND(IDM_WALH, OnWalh)
	ON_UPDATE_COMMAND_UI(IDM_WALH, OnUpdateWalh)
	ON_COMMAND(IDM_CONTOUR_H, OnContourH)
	ON_UPDATE_COMMAND_UI(IDM_CONTOUR_H, OnUpdateContourH)
	ON_COMMAND(IDM_CONTOUR_V, OnContourV)
	ON_UPDATE_COMMAND_UI(IDM_CONTOUR_V, OnUpdateContourV)
	ON_COMMAND(IDM_EMBOSSMENT, OnEmbossment)
	ON_UPDATE_COMMAND_UI(IDM_EMBOSSMENT, OnUpdateEmbossment)
	ON_COMMAND(IDM_SPREAD, OnSpread)
	ON_UPDATE_COMMAND_UI(IDM_SPREAD, OnUpdateSpread)
	ON_COMMAND(IDM_HOMOEDGEDETECT, OnHomoedgedetect)
	ON_UPDATE_COMMAND_UI(IDM_HOMOEDGEDETECT, OnUpdateHomoedgedetect)
	ON_COMMAND(IDM_DIFFERENTEDGEDETECT, OnDifferentedgedetect)
	ON_UPDATE_COMMAND_UI(IDM_DIFFERENTEDGEDETECT, OnUpdateDifferentedgedetect)
	ON_COMMAND(IDM_CANNYED, OnCannyed)
	ON_UPDATE_COMMAND_UI(IDM_CANNYED, OnUpdateCannyed)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(WM_REALIZEPAL, OnRealizePal)
	ON_COMMAND(IDM_Nearest_Interpolation, &CImageProcessView::OnNearestInterpolation)
	ON_UPDATE_COMMAND_UI(IDM_Nearest_Interpolation, &CImageProcessView::OnUpdateNearestInterpolation)
	ON_COMMAND(IDM_Bilinear_Interpolation, &CImageProcessView::OnBilinearInterpolation)
	ON_UPDATE_COMMAND_UI(IDM_Bilinear_Interpolation, &CImageProcessView::OnUpdateBilinearInterpolation)
	ON_COMMAND(IDM_Bicubic_Interpolation, &CImageProcessView::OnBicubicInterpolation)
	ON_UPDATE_COMMAND_UI(IDM_Bicubic_Interpolation, &CImageProcessView::OnUpdateBicubicInterpolation)
		ON_COMMAND(IDM_MT4, &CImageProcessView::OnMt4)
		ON_UPDATE_COMMAND_UI(IDM_MT4, &CImageProcessView::OnUpdateMt4)
		ON_COMMAND(ID_Image_Negatives, &CImageProcessView::OnImageNegatives)
		ON_UPDATE_COMMAND_UI(ID_Image_Negatives, &CImageProcessView::OnUpdateImageNegatives)
		ON_COMMAND(ID_Log_Transformation, &CImageProcessView::OnLogTransformation)
		ON_UPDATE_COMMAND_UI(ID_Log_Transformation, &CImageProcessView::OnUpdateLogTransformation)
		ON_COMMAND(ID_Contrast_Stretching, &CImageProcessView::OnContrastStretching)
		ON_UPDATE_COMMAND_UI(ID_Contrast_Stretching, &CImageProcessView::OnUpdateContrastStretching)
		ON_COMMAND(ID_Local_Image_Enhancement, &CImageProcessView::OnLocalImageEnhancement)
		ON_UPDATE_COMMAND_UI(ID_Local_Image_Enhancement, &CImageProcessView::OnUpdateLocalImageEnhancement)
		ON_COMMAND(ID_Median_Filter, &CImageProcessView::OnMedianFilter)
		ON_UPDATE_COMMAND_UI(ID_Median_Filter, &CImageProcessView::OnUpdateMedianFilter)
		ON_COMMAND(ID_Soble_Operator, &CImageProcessView::OnSobleOperator)
		ON_UPDATE_COMMAND_UI(ID_Soble_Operator, &CImageProcessView::OnUpdateSobleOperator)
		ON_COMMAND(ID_Laplacian_Sharpening, &CImageProcessView::OnLaplacianSharpening)
		ON_UPDATE_COMMAND_UI(ID_Laplacian_Sharpening, &CImageProcessView::OnUpdateLaplacianSharpening)
		ON_COMMAND(ID_Palmprint_Image_Enhancement, &CImageProcessView::OnPalmprintImageEnhancement)
		ON_UPDATE_COMMAND_UI(ID_Palmprint_Image_Enhancement, &CImageProcessView::OnUpdatePalmprintImageEnhancement)
		ON_COMMAND(ID_Frequency_Domain_Filter, &CImageProcessView::OnFrequencyDomainFilter)
		ON_UPDATE_COMMAND_UI(ID_Frequency_Domain_Filter, &CImageProcessView::OnUpdateFrequencyDomainFilter)
		ON_COMMAND(ID_Homomorphic_Filter, &CImageProcessView::OnHomomorphicFilter)
		ON_UPDATE_COMMAND_UI(ID_Homomorphic_Filter, &CImageProcessView::OnUpdateHomomorphicFilter)
		ON_COMMAND(ID_Battivo_Low_Pass_Filter, &CImageProcessView::OnBattivoLowPassFilter)
		ON_UPDATE_COMMAND_UI(ID_Battivo_Low_Pass_Filter, &CImageProcessView::OnUpdateBattivoLowPassFilter)
		ON_COMMAND(ID_Battivo_High_Pass_Filter, &CImageProcessView::OnBattivoHighPassFilter)
		ON_UPDATE_COMMAND_UI(ID_Battivo_High_Pass_Filter, &CImageProcessView::OnUpdateBattivoHighPassFilter)
		ON_COMMAND(ID_Battivo_Bandpass_Filter, &CImageProcessView::OnBattivoBandpassFilter)
		ON_UPDATE_COMMAND_UI(ID_Battivo_Bandpass_Filter, &CImageProcessView::OnUpdateBattivoBandpassFilter)
		ON_COMMAND(ID_Battivo_Band_Rejection_Filter, &CImageProcessView::OnBattivoBandRejectionFilter)
		ON_UPDATE_COMMAND_UI(ID_Battivo_Band_Rejection_Filter, &CImageProcessView::OnUpdateBattivoBandRejectionFilter)
		ON_COMMAND(ID_Exercise_443, &CImageProcessView::OnExercise443)
		ON_UPDATE_COMMAND_UI(ID_Exercise_443, &CImageProcessView::OnUpdateExercise443)
		ON_COMMAND(ID_Variable_Threshold, &CImageProcessView::OnVariableThreshold)
		ON_UPDATE_COMMAND_UI(ID_Variable_Threshold, &CImageProcessView::OnUpdateVariableThreshold)
		ON_COMMAND(ID_Ostu_Image_Segmentation, &CImageProcessView::OnOstuImageSegmentation)
		ON_UPDATE_COMMAND_UI(ID_Ostu_Image_Segmentation, &CImageProcessView::OnUpdateOstuImageSegmentation)
		ON_COMMAND(ID_Face_Recognition, &CImageProcessView::OnFaceRecognition)
		ON_UPDATE_COMMAND_UI(ID_Face_Recognition, &CImageProcessView::OnUpdateFaceRecognition)
		ON_COMMAND(ID_Iris_Segmentation, &CImageProcessView::OnIrisSegmentation)
		ON_UPDATE_COMMAND_UI(ID_Iris_Segmentation, &CImageProcessView::OnUpdateIrisSegmentation)
		ON_COMMAND(ID_Boundary_Tracking_Algorithm, &CImageProcessView::OnBoundaryTrackingAlgorithm)
		ON_UPDATE_COMMAND_UI(ID_Boundary_Tracking_Algorithm, &CImageProcessView::OnUpdateBoundaryTrackingAlgorithm)
		ON_COMMAND(ID_Region_Refinement, &CImageProcessView::OnRegionRefinement)
		ON_UPDATE_COMMAND_UI(ID_Region_Refinement, &CImageProcessView::OnUpdateRegionRefinement)
		ON_COMMAND(ID_Graylevel_Co_occurrence_Matrix, &CImageProcessView::OnGraylevelCooccurrenceMatrix)
		ON_UPDATE_COMMAND_UI(ID_Graylevel_Co_occurrence_Matrix, &CImageProcessView::OnUpdateGraylevelCooccurrenceMatrix)
		ON_COMMAND(ID_Example_1116, &CImageProcessView::OnExample1116)
		ON_UPDATE_COMMAND_UI(ID_Example_1116, &CImageProcessView::OnUpdateExample1116)
		ON_COMMAND(ID_Example_1127, &CImageProcessView::OnExample1127)
		ON_UPDATE_COMMAND_UI(ID_Example_1127, &CImageProcessView::OnUpdateExample1127)
		ON_COMMAND(ID_Figure_Recognition, &CImageProcessView::OnFigureRecognition)
		ON_UPDATE_COMMAND_UI(ID_Figure_Recognition, &CImageProcessView::OnUpdateFigureRecognition)
		ON_COMMAND(ID_Peach_Rmage_Recognition, &CImageProcessView::OnPeachRmageRecognition)
		ON_UPDATE_COMMAND_UI(ID_Peach_Rmage_Recognition, &CImageProcessView::OnUpdatePeachRmageRecognition)
		ON_COMMAND(ID_FILE_SAVE, &CImageProcessView::OnFileSave)
		ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CImageProcessView::OnUpdateFileSave)
		ON_COMMAND(ID_Histogram, &CImageProcessView::OnHistogram)
		ON_UPDATE_COMMAND_UI(ID_Histogram, &CImageProcessView::OnUpdateHistogram)
		END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView construction/destruction

CImageProcessView::CImageProcessView()
{
	// TODO: add construction code here

}

CImageProcessView::~CImageProcessView()
{
}

BOOL CImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView drawing

void CImageProcessView::OnDraw(CDC* pDC)
{
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (! pDoc->m_pDib->IsEmpty())
		pDoc->m_pDib->Display(pDC, 20, 20);
}

void CImageProcessView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CImageProcessDoc* pDoc = GetDocument();

	CSize sizeTotal(pDoc->m_pDib->GetWidth()+40, pDoc->m_pDib->GetHeight()+40);
	SetScrollSizes(MM_TEXT, sizeTotal);
	
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);
	CRect rc;
	pAppFrame->GetClientRect(&rc);
	if (rc.Width() >= sizeTotal.cx && rc.Height() >= sizeTotal.cy &&
		(sizeTotal.cx>0 || sizeTotal.cy>0))
		ResizeParentToFit(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView printing

BOOL CImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView diagnostics

#ifdef _DEBUG
void CImageProcessView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessDoc* CImageProcessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessDoc)));
	return (CImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessView message handlers

LRESULT CImageProcessView::OnRealizePal(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);
	CImageProcessDoc* pDoc = GetDocument();

	if (pDoc->m_pDib->IsEmpty())
		return 0L;  // must be a new document

	CPalette* pPal = pDoc->m_pDib->GetPalette();
	if (pPal != NULL)
	{
		CWnd* pAppFrame = AfxGetApp()->m_pMainWnd;

		CClientDC appDC(pAppFrame);
		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);

		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
				GetDocument()->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tSelectPalette failed!\n");
		}
	}

	return 0L;
}


void CImageProcessView::OnHorz() 
{
	EdgeEnhance(HORZ);
}

void CImageProcessView::OnUpdateHorz(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLap1() 
{
	EdgeEnhance(LAP1);
}

void CImageProcessView::OnUpdateLap1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLap2() 
{
	EdgeEnhance(LAP2);
}

void CImageProcessView::OnUpdateLap2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLap3() 
{
	EdgeEnhance(LAP3);
}

void CImageProcessView::OnUpdateLap3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLap4() 
{
	EdgeEnhance(LAP4);
}

void CImageProcessView::OnUpdateLap4(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnVert() 
{
	EdgeEnhance(VERT);
}

void CImageProcessView::OnUpdateVert(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnVerthorz() 
{
	EdgeEnhance(VERTHORZ);
}

void CImageProcessView::OnUpdateVerthorz(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnHf1() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->HighPass(FD.m_nStreng, FILTER1);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateHf1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnHf2() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->HighPass(FD.m_nStreng, FILTER2);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateHf2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnHf3() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->HighPass(FD.m_nStreng, FILTER3);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateHf3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLf1() 
{
	//选择卷积强度对话框
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->LowPass(FD.m_nStreng, FILTER1);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateLf1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLf2() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->LowPass(FD.m_nStreng, FILTER2);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateLf2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnLf3() 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->LowPass(FD.m_nStreng, FILTER3);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateLf3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnEast() 
{
	EdgeEnhance(EAST);
}

void CImageProcessView::OnUpdateEast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnNorth() 
{
	EdgeEnhance(NORTH);
}

void CImageProcessView::OnUpdateNorth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnNortheast() 
{
	EdgeEnhance(NORTHEAST);
}

void CImageProcessView::OnUpdateNortheast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnNorthwest() 
{
	EdgeEnhance(NORTHWEST);
}

void CImageProcessView::OnUpdateNorthwest(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnSouth() 
{
	EdgeEnhance(SOUTH);
}

void CImageProcessView::OnUpdateSouth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnSoutheast() 
{
	EdgeEnhance(SOUTHEAST);
}

void CImageProcessView::OnUpdateSoutheast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnSouthwest() 
{
	EdgeEnhance(SOUTHWEST);
}

void CImageProcessView::OnUpdateSouthwest(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnWest() 
{
	EdgeEnhance(WEST);
}

void CImageProcessView::OnUpdateWest(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnSobel() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->EdegeDetect(SOBEL);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateSobel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnHough() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->EdegeDetect(HOUGH);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateHough(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::EdgeEnhance(int nAlgorithm) 
{
	CConvoluteDlg FD(1,10,3,this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->EdgeEnhance(FD.m_nStreng, nAlgorithm);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

//中值滤波
void CImageProcessView::OnMedianfilter() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MedianFilter();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateMedianfilter(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}



void CImageProcessView::OnErosionH() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Erosion(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnErosionV() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Erosion(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateErosionH(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnUpdateErosionV(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnDilationH() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Erosion(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateDilationH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnDilationV() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Dilation(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateDilationV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnCloseH() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MorphClose(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateCloseH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnCloseV() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MorphClose(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateCloseV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnOpenH() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MorphOpen(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateOpenH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnOpenV() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MorphOpen(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateOpenV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnThin() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Thinning();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateThin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnUpdateReverse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnFft() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->FFT();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateFft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnDct() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->DCT();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateDct(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnWalh() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->WALh();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateWalh(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnContourH() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Contour(TRUE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateContourH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnContourV() 
{
	// TODO: Add your command handler code here
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Contour(FALSE);
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessView::OnUpdateContourV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
	
}

void CImageProcessView::OnEmbossment() 
{
	CEmbossmentDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->Embossment(dlg.m_nValue);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateEmbossment(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnSpread() 
{
	CSpreadDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->Spread(dlg.m_nValue);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
}

void CImageProcessView::OnUpdateSpread(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnHomoedgedetect() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->HomogenityEdegeDetect();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateHomoedgedetect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnDifferentedgedetect() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->DifferenceEdegeDetect();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateDifferentedgedetect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}

void CImageProcessView::OnCannyed() 
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->CannyEdegeDetect();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}

void CImageProcessView::OnUpdateCannyed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(! GetDocument()->m_pDib->IsEmpty());
}



//**********图像处理作业*************
//最近邻插值
void CImageProcessView::OnNearestInterpolation()
{

	CZoomDlg FD(9, 9, 9, this);
	if (FD.DoModal() == IDOK)
	{
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->NearestInterpolation(FD.m_nStreng);
		pDoc->SetModifiedFlag(TRUE);
		OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}

}
/*
//选择卷积强度对话框
CConvoluteDlg FD(1,10,3,this);
if (FD.DoModal() == IDOK)
{
CImageProcessDoc* pDoc = GetDocument();
pDoc->m_pDib->LowPass(FD.m_nStreng, FILTER1);
pDoc->SetModifiedFlag(TRUE);
OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
pDoc->UpdateAllViews(NULL);
}


CEmbossmentDlg dlg(this);
if (dlg.DoModal() == IDOK)
{
CImageProcessDoc* pDoc = GetDocument();
pDoc->m_pDib->Embossment(dlg.m_nValue);
pDoc->SetModifiedFlag(TRUE);
OnRealizePal((WPARAM)m_hWnd,0);  // realize the new palette
pDoc->UpdateAllViews(NULL);
}
*/

void CImageProcessView::OnUpdateNearestInterpolation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//双线性插值
void CImageProcessView::OnBilinearInterpolation()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->BilinearInterpolation();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateBilinearInterpolation(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//双三次插值
void CImageProcessView::OnBicubicInterpolation()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->BicubicInterpolation();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateBicubicInterpolation(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//m通路转换为4通路
void CImageProcessView::OnMt4()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Mt4();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateMt4(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}



//【3.1 编程实现图像反转、对数变换和对比度拉伸。】
void CImageProcessView::OnImageNegatives()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->ImageNegatives();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateImageNegatives(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnLogTransformation()
{
	CImageProcessDoc* pDoc = GetDocument();

	CLogTranDlg dlg;
	dlg.m_pDib = pDoc->m_pDib;
	dlg.DoModal();
	//pDoc->m_pDib->LogTransformation();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateLogTransformation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnContrastStretching()
{
	CImageProcessDoc* pDoc = GetDocument();
	CStretchDlg dlg;
	dlg.m_pDib = pDoc->m_pDib;
	dlg.DoModal();

	//pDoc->m_pDib->ContrastStretching();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateContrastStretching(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【3.2 试提出一种如3.3.4节中讨论的基于直方图统计的局部增强方法，并编程实现。】
void CImageProcessView::OnLocalImageEnhancement()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->LocalImageEnhancement();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateLocalImageEnhancement(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【3.3 编程实现中值滤波、Soble运算和Laplacian锐化。】
void CImageProcessView::OnMedianFilter()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->MyMedianFilter();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateMedianFilter(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnSobleOperator()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->SobleOperator();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateSobleOperator(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnLaplacianSharpening()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->LaplacianSharpening();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateLaplacianSharpening(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【3.4 对掌纹图像进行图像增强，使得掌纹纹线更清晰。说明增强方案，并编程实现。】
void CImageProcessView::OnPalmprintImageEnhancement()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->PalmprintImageEnhancement();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdatePalmprintImageEnhancement(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【4.1 编程实现等效于3*3邻域均值平滑的频率域滤波。】
void CImageProcessView::OnFrequencyDomainFilter()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->FrequencyDomainFilter();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateFrequencyDomainFilter(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【4.2 编程实现同态滤波以及巴特沃思低通、高通、带通、带阻滤波器。】
void CImageProcessView::OnHomomorphicFilter()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->HomomorphicFilter();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateHomomorphicFilter(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnBattivoLowPassFilter()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->BattivoLowPassFilter();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateBattivoLowPassFilter(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnBattivoHighPassFilter()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->BattivoHighPassFilter();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateBattivoHighPassFilter(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnBattivoBandpassFilter()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->BattivoBandpassFilter();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateBattivoBandpassFilter(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}


void CImageProcessView::OnBattivoBandRejectionFilter()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->BattivoBandRejectionFilter();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateBattivoBandRejectionFilter(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【4.3 习题4.43。】
void CImageProcessView::OnExercise443()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Exercise443();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateExercise443(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【5.1 编程实现可变阈值处理。】
void CImageProcessView::OnVariableThreshold()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->VariableThreshold();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateVariableThreshold(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【5.2 编程实现Ostu图像分割方法。】
void CImageProcessView::OnOstuImageSegmentation()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->OstuImageSegmentation();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateOstuImageSegmentation(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【5.3 设计人脸方案，并编程实现。】
void CImageProcessView::OnFaceRecognition()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->FaceRecognition();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateFaceRecognition(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【5.4 设计与实现虹膜图像分割。】
void CImageProcessView::OnIrisSegmentation()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->IrisSegmentation();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateIrisSegmentation(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【6.1 编程实现边界追踪算法。】
void CImageProcessView::OnBoundaryTrackingAlgorithm()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->BoundaryTrackingAlgorithm();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateBoundaryTrackingAlgorithm(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【	6.2 编程实现二值区域细化算法。】
void CImageProcessView::OnRegionRefinement()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->RegionRefinement();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateRegionRefinement(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【6.3 编程实现灰度共生矩阵方法。】
void CImageProcessView::OnGraylevelCooccurrenceMatrix()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->GraylevelCooccurrenceMatrix();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateGraylevelCooccurrenceMatrix(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【6.4 习题11.16。】
void CImageProcessView::OnExample1116()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Example1116();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateExample1116(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【6.5 习题11.27。】
void CImageProcessView::OnExample1127()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->Example1127();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateExample1127(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【7.1 编程实现印刷体数字识别（包括增强、分割、特征提取和识别）。】
void CImageProcessView::OnFigureRecognition()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->FigureRecognition();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateFigureRecognition(CCmdUI *pCmdUI)
{
 pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

//【7.2 编程实现桃子图像识别，要求能使识别蟠桃、水蜜桃、油桃、黄桃等亚种。（包括增强、分割、特征提取和识别）】
void CImageProcessView::OnPeachRmageRecognition()
{
	CImageProcessDoc* pDoc = GetDocument();
	pDoc->m_pDib->PeachImageRecognition();
	pDoc->SetModifiedFlag(TRUE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdatePeachRmageRecognition(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}

/******************************************************************************
*	作用:		"保存"的命令相应函数
******************************************************************************/
void CImageProcessView::OnFileSave()
{
	if (GetDocument()->m_pDib->IsEmpty())
	{
		MessageBox("没有可以保存的文件!", "Magic House", MB_ICONWARNING);
		return;
	}
	CFileDialog SaveDlg(FALSE, "bmp", "未命名", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, "图像文件格式(*.bmp) |*.bmp||", NULL);
	if (SaveDlg.DoModal() == IDOK)
	{
		CString tempname = SaveDlg.GetPathName();
		LPCSTR savePath = (LPCSTR)CStringA(tempname);
		CImageProcessDoc* pDoc = GetDocument();
		pDoc->m_pDib->Save(savePath);
		tempname.ReleaseBuffer();
		pDoc->SetModifiedFlag(FALSE);
		OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
		MessageBox("文件已成功保存!");
	}
}

void CImageProcessView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}



void CImageProcessView::OnHistogram()
{
	CImageProcessDoc* pDoc = GetDocument();
	CHistogramDlg dlg;
	dlg.m_pDib = pDoc->m_pDib;
	dlg.DoModal();

	pDoc->SetModifiedFlag(FALSE);
	OnRealizePal((WPARAM)m_hWnd, 0);  // realize the new palette
	pDoc->UpdateAllViews(NULL);
}


void CImageProcessView::OnUpdateHistogram(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->m_pDib->IsEmpty());
}
