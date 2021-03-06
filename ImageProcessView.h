// ImageProcessView.h : interface of the CImageProcessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSVIEW_H__7C70D8EE_EC43_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_IMAGEPROCESSVIEW_H__7C70D8EE_EC43_11D2_9481_000021003EA5__INCLUDED_

#define SAFE_DELETE(p)	\
{						\
	if ((p) != NULL)	\
	{					\
		delete (p);		\
		(p) = NULL;		\
	}					\
}

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageProcessView : public CScrollView
{
protected: // create from serialization only
	CImageProcessView();
	DECLARE_DYNCREATE(CImageProcessView)

// Attributes
public:
	CImageProcessDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void EdgeEnhance(int nAlgorithm);



protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessView)
	afx_msg void OnEast();
	afx_msg void OnUpdateEast(CCmdUI* pCmdUI);
	afx_msg void OnHf1();
	afx_msg void OnUpdateHf1(CCmdUI* pCmdUI);
	afx_msg void OnHf2();
	afx_msg void OnUpdateHf2(CCmdUI* pCmdUI);
	afx_msg void OnHf3();
	afx_msg void OnUpdateHf3(CCmdUI* pCmdUI);
	afx_msg void OnHorz();
	afx_msg void OnUpdateHorz(CCmdUI* pCmdUI);
	afx_msg void OnLap1();
	afx_msg void OnUpdateLap1(CCmdUI* pCmdUI);
	afx_msg void OnLap2();
	afx_msg void OnUpdateLap2(CCmdUI* pCmdUI);
	afx_msg void OnLap3();
	afx_msg void OnUpdateLap3(CCmdUI* pCmdUI);
	afx_msg void OnLap4();
	afx_msg void OnUpdateLap4(CCmdUI* pCmdUI);
	afx_msg void OnLf1();
	afx_msg void OnUpdateLf1(CCmdUI* pCmdUI);
	afx_msg void OnLf2();
	afx_msg void OnUpdateLf2(CCmdUI* pCmdUI);
	afx_msg void OnLf3();
	afx_msg void OnUpdateLf3(CCmdUI* pCmdUI);
	afx_msg void OnNorth();
	afx_msg void OnUpdateNorth(CCmdUI* pCmdUI);
	afx_msg void OnNortheast();
	afx_msg void OnUpdateNortheast(CCmdUI* pCmdUI);
	afx_msg void OnNorthwest();
	afx_msg void OnUpdateNorthwest(CCmdUI* pCmdUI);
	afx_msg void OnSouth();
	afx_msg void OnUpdateSouth(CCmdUI* pCmdUI);
	afx_msg void OnSoutheast();
	afx_msg void OnUpdateSoutheast(CCmdUI* pCmdUI);
	afx_msg void OnSouthwest();
	afx_msg void OnUpdateSouthwest(CCmdUI* pCmdUI);
	afx_msg void OnVert();
	afx_msg void OnUpdateVert(CCmdUI* pCmdUI);
	afx_msg void OnVerthorz();
	afx_msg void OnUpdateVerthorz(CCmdUI* pCmdUI);
	afx_msg void OnWest();
	afx_msg void OnUpdateWest(CCmdUI* pCmdUI);
	afx_msg void OnSobel();
	afx_msg void OnUpdateSobel(CCmdUI* pCmdUI);
	afx_msg void OnHough();
	afx_msg void OnUpdateHough(CCmdUI* pCmdUI);
	afx_msg void OnMedianfilter();
	afx_msg void OnUpdateMedianfilter(CCmdUI* pCmdUI);
	afx_msg void OnCloseH();
	afx_msg void OnUpdateCloseH(CCmdUI* pCmdUI);
	afx_msg void OnCloseV();
	afx_msg void OnUpdateCloseV(CCmdUI* pCmdUI);
	afx_msg void OnDilationH();
	afx_msg void OnUpdateDilationH(CCmdUI* pCmdUI);
	afx_msg void OnDilationV();
	afx_msg void OnUpdateDilationV(CCmdUI* pCmdUI);
	afx_msg void OnErosionH();
	afx_msg void OnUpdateErosionH(CCmdUI* pCmdUI);
	afx_msg void OnErosionV();
	afx_msg void OnUpdateErosionV(CCmdUI* pCmdUI);
	afx_msg void OnOpenH();
	afx_msg void OnUpdateOpenH(CCmdUI* pCmdUI);
	afx_msg void OnOpenV();
	afx_msg void OnUpdateOpenV(CCmdUI* pCmdUI);
	afx_msg void OnThin();
	afx_msg void OnUpdateThin(CCmdUI* pCmdUI);
	afx_msg void OnFft();
	afx_msg void OnUpdateFft(CCmdUI* pCmdUI);
	afx_msg void OnDct();
	afx_msg void OnUpdateDct(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReverse(CCmdUI* pCmdUI);
	afx_msg void OnWalh();
	afx_msg void OnUpdateWalh(CCmdUI* pCmdUI);
	afx_msg void OnContourH();
	afx_msg void OnUpdateContourH(CCmdUI* pCmdUI);
	afx_msg void OnContourV();
	afx_msg void OnUpdateContourV(CCmdUI* pCmdUI);
	afx_msg void OnEmbossment();
	afx_msg void OnUpdateEmbossment(CCmdUI* pCmdUI);
	afx_msg void OnSpread();
	afx_msg void OnUpdateSpread(CCmdUI* pCmdUI);
	afx_msg void OnHomoedgedetect();
	afx_msg void OnUpdateHomoedgedetect(CCmdUI* pCmdUI);
	afx_msg void OnDifferentedgedetect();
	afx_msg void OnUpdateDifferentedgedetect(CCmdUI* pCmdUI);
	afx_msg void OnCannyed();
	afx_msg void OnUpdateCannyed(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnRealizePal(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNearestInterpolation();
	afx_msg void OnUpdateNearestInterpolation(CCmdUI *pCmdUI);
	afx_msg void OnBilinearInterpolation();
	afx_msg void OnUpdateBilinearInterpolation(CCmdUI *pCmdUI);
	afx_msg void OnBicubicInterpolation();
	afx_msg void OnUpdateBicubicInterpolation(CCmdUI *pCmdUI);
	afx_msg void OnMt4();
	afx_msg void OnUpdateMt4(CCmdUI *pCmdUI);
	afx_msg void OnImageNegatives();
	afx_msg void OnUpdateImageNegatives(CCmdUI *pCmdUI);
	afx_msg void OnLogTransformation();
	afx_msg void OnUpdateLogTransformation(CCmdUI *pCmdUI);
	afx_msg void OnContrastStretching();
	afx_msg void OnUpdateContrastStretching(CCmdUI *pCmdUI);
	afx_msg void OnLocalImageEnhancement();
	afx_msg void OnUpdateLocalImageEnhancement(CCmdUI *pCmdUI);
	afx_msg void OnMedianFilter();
	afx_msg void OnUpdateMedianFilter(CCmdUI *pCmdUI);
	afx_msg void OnSobleOperator();
	afx_msg void OnUpdateSobleOperator(CCmdUI *pCmdUI);
	afx_msg void OnLaplacianSharpening();
	afx_msg void OnUpdateLaplacianSharpening(CCmdUI *pCmdUI);
	afx_msg void OnPalmprintImageEnhancement();
	afx_msg void OnUpdatePalmprintImageEnhancement(CCmdUI *pCmdUI);
	afx_msg void OnFrequencyDomainFilter();
	afx_msg void OnUpdateFrequencyDomainFilter(CCmdUI *pCmdUI);
	afx_msg void OnHomomorphicFilter();
	afx_msg void OnUpdateHomomorphicFilter(CCmdUI *pCmdUI);
	afx_msg void OnBattivoLowPassFilter();
	afx_msg void OnUpdateBattivoLowPassFilter(CCmdUI *pCmdUI);
	afx_msg void OnBattivoHighPassFilter();
	afx_msg void OnUpdateBattivoHighPassFilter(CCmdUI *pCmdUI);
	afx_msg void OnBattivoBandpassFilter();
	afx_msg void OnUpdateBattivoBandpassFilter(CCmdUI *pCmdUI);
	afx_msg void OnBattivoBandRejectionFilter();
	afx_msg void OnUpdateBattivoBandRejectionFilter(CCmdUI *pCmdUI);
	afx_msg void OnExercise443();
	afx_msg void OnUpdateExercise443(CCmdUI *pCmdUI);
	afx_msg void OnVariableThreshold();
	afx_msg void OnUpdateVariableThreshold(CCmdUI *pCmdUI);
	afx_msg void OnOstuImageSegmentation();
	afx_msg void OnUpdateOstuImageSegmentation(CCmdUI *pCmdUI);
	afx_msg void OnFaceRecognition();
	afx_msg void OnUpdateFaceRecognition(CCmdUI *pCmdUI);
	afx_msg void OnIrisSegmentation();
	afx_msg void OnUpdateIrisSegmentation(CCmdUI *pCmdUI);
	afx_msg void OnBoundaryTrackingAlgorithm();
	afx_msg void OnUpdateBoundaryTrackingAlgorithm(CCmdUI *pCmdUI);
	afx_msg void OnRegionRefinement();
	afx_msg void OnUpdateRegionRefinement(CCmdUI *pCmdUI);
	afx_msg void OnGraylevelCooccurrenceMatrix();
	afx_msg void OnUpdateGraylevelCooccurrenceMatrix(CCmdUI *pCmdUI);
	afx_msg void OnExample1116();
	afx_msg void OnUpdateExample1116(CCmdUI *pCmdUI);
	afx_msg void OnExample1127();
	afx_msg void OnUpdateExample1127(CCmdUI *pCmdUI);
	afx_msg void OnFigureRecognition();
	afx_msg void OnUpdateFigureRecognition(CCmdUI *pCmdUI);
	afx_msg void OnPeachRmageRecognition();
	afx_msg void OnUpdatePeachRmageRecognition(CCmdUI *pCmdUI);


	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);

	afx_msg void OnHistogram();
	afx_msg void OnUpdateHistogram(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in ImageProcessView.cpp
inline CImageProcessDoc* CImageProcessView::GetDocument()
   { return (CImageProcessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSVIEW_H__7C70D8EE_EC43_11D2_9481_000021003EA5__INCLUDED_)
