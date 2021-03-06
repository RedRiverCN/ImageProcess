#if !defined(AFX_DIB_H__33817561_072D_11D1_8C94_000021003EA5__INCLUDED_)
#define AFX_DIB_H__33817561_072D_11D1_8C94_000021003EA5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Dib.h : header file
//

#include "DIBAPI.H"

// Effect consts
#define EFFECT_SCANDOWN		1
#define EFFECT_SCANUP		2
#define EFFECT_SCANRIGHT	3
#define EFFECT_SCANLEFT		4
#define EFFECT_VSPLITSCAN	5
#define EFFECT_HSPLITSCAN	6
#define EFFECT_MOVEDOWN		7
#define EFFECT_MOVEUP		8
#define EFFECT_MOVERIGHT	9
#define EFFECT_MOVELEFT		10
#define EFFECT_VSPLITMOVE	11
#define EFFECT_HSPLITMOVE	12
#define EFFECT_VCROSSMOVE	13
#define EFFECT_HCROSSMOVE	14
#define EFFECT_VRASTER		15
#define EFFECT_HRASTER		16
#define EFFECT_VBLIND		17
#define EFFECT_HBLIND		18
#define EFFECT_MOSAIC		19
#define EFFECT_FADE			20

////////////////////////////////////////////////////////////////////////////
// CDib
class CDib : public CObject
{                          
	DECLARE_SERIAL(CDib)

// Public member function
public: 
	// 构造函数
	CDib();
	// 拷贝构造函数
	CDib(CDib& other) 
	{ 
		// 1) 初始化 
		m_hDib		= NULL;
		m_hBitmap   = NULL;
		m_pPalette  = NULL;
		m_pBitmap   = NULL;

		// 2) 检查源对象是否有效 
		HDIB hDib = other.GetHandle();
		if (! hDib)
			return;
		LPBYTE lpDIB = (LPBYTE)GlobalLock(hDib);
		if (lpDIB == NULL)
		{
			GlobalUnlock(hDib);
			return; 
		}

		// 3) 创建对象 
		Create(lpDIB);
		GlobalUnlock(hDib);
	} 
	
	// 赋值函数 
	CDib& operator =(CDib& other) 
	{ 
		// 1) 检查自赋值 
		if(this == &other) 
			return *this; 

		// 2) 检查源对象是否有效 
		HDIB hDib = other.GetHandle();
		if (! hDib)
			return *this; 
		LPBYTE lpDIB = (LPBYTE)GlobalLock(hDib);
		if (lpDIB == NULL)
		{
			GlobalUnlock(hDib);
			return *this; 
		}

		// 3) 释放原有的内存资源 
		Destroy();
		m_hBitmap   = NULL;
		m_pPalette  = NULL;
		m_pBitmap   = NULL;

		// 4) 分配新的内存资源，并复制内容 
		Create(lpDIB);
		GlobalUnlock(hDib);

		// 4) 返回本对象的引用 
		return *this; 
	} 

	// create
	BOOL	Create(DWORD dwWidth, DWORD dwHeight);
	BOOL	Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount);
	BOOL	Create(LPBYTE lpDIB);
	BOOL	Create(LPBYTE lpDIB,		// DIB pointer
				   WORD  wBitCount);	// bits/pixel 
	BOOL	Create(HBITMAP hBitmap);	// DIBSection
	BOOL	Create(HBITMAP hBitmap,		// DIBSection
				   WORD  wBitCount);	// bits/pixel 
	BOOL	Create(HBITMAP hBitmap,		// Bitmap handle
				   HPALETTE hPalette);	// Palette handle
	BOOL	Create(HBITMAP hBitmap,		// Bitmap handle
				   HPALETTE hPalette,	// Palette handle
				   WORD  wBitCount);	// bits/pixel 
	BOOL	Create(CRect rcScreen);
	BOOL	Create(HWND hWnd, WORD fPrintArea);
	BOOL	Create(HWND hWnd, CRect rcClientArea);
	// attach handle
	BOOL	Attach(HDIB hDib);
	// load/save
    BOOL	Load(UINT uIDS, LPCTSTR lpszDibType);
    BOOL	Load(LPCTSTR lpszDibRes, LPCTSTR lpszDibType);
    BOOL	Load(LPCTSTR lpszDibFile);
    BOOL	Save(LPCTSTR lpszDibFile);
	BOOL	Read(CFile *pFile);
	BOOL	Write(CFile *pFile);

	// clone
	CDib *  Clone();

	// deconstructor
	virtual ~CDib();
	// destroy
	void	Destroy();
	HDIB	Detach();

	// overlaying Serialize
	virtual void Serialize(CArchive &ar);
    
	// display
	BOOL	Display(CDC * pDC, int xDest, int yDest, int nWidthDest, int nHeightDest, 
 				   int xSrc, int ySrc, DWORD dwRop=SRCCOPY);
	BOOL	Display(CDC * pDC, int xDest, int yDest, int nWidthDest, int nHeightDest, 
				    int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop=SRCCOPY);
    BOOL	Display(CDC* pDC, int x, int y, DWORD dwRop=SRCCOPY);
	BOOL	Display(CDC* pDC, CRect rcDest, CRect rcSrc,DWORD dwRop=SRCCOPY);
	BOOL	DisplayPalette(CDC* pDC, CRect rc);

	// DC for modify DIB
	CDC*	BeginPaint(CDC *pDC);
	void	EndPaint();

	// DDB and palette
	BOOL	BuildBitmap();
	BOOL	BuildPalette();

	// attributes
	BOOL	IsEmpty();
	DWORD	GetCompression();
    WORD	GetBitCount();
    LONG	GetWidth();
    LONG	GetHeight();    
	LONG	GetWidthBytes();
    WORD	GetColorNumber();
	WORD	GetPaletteSize();
    CBitmap*  GetBitmap();
	CPalette* GetPalette();
	HANDLE	GetHandle();
	LPBYTE	GetBitsPtr();
	COLORREF GetPixel(LONG x, LONG y);
	LONG    GetPixelOffset(LONG x, LONG y);
	BOOL	ConvertFormat(WORD wBitCount); 

	// display transparently
	BOOL	DisplayTransparent(CDC* pDC, int x, int y, COLORREF cTransparentColor);

	// effect operation
	BOOL	Display(int nEffect, CDC* pDC, int x, int y, int nDeta, DWORD dwDelayTime, DWORD dwRop=SRCCOPY);
	BOOL	Clear(int nEffect, CDC* pDC, int x, int y, int nDeta, DWORD dwDelayTime);

	// transform
	HDIB	CopyRect(CRect rc);
	BOOL	CutRect(CRect rc);
	BOOL	MergeDib(HDIB hDib, CPoint ptTopLeft);
	BOOL	TransparentMergeDib(HDIB hDib, CPoint ptTopLeft, COLORREF crTransparent);
	BOOL	Crop(CRect rc);
	BOOL	Rotate(double fDegrees, COLORREF clrBack);
	BOOL	Rotate90();
	BOOL	Rotate180();
	BOOL	Rotate270();
	BOOL	FlipHorz();
	BOOL	FlipVert();
	BOOL	Zoom(double fRatioX, double fRatioY);
	BOOL	ChangeImageSize(int nWidth, int nHeight);
	BOOL	ChangeCanvasSize(int nWidth, int nHeight, int nPosition);

	// color process
	BOOL	IsRequireDither(CDC* pDC);
	BOOL	DitherDisplay(CDC* pDC, CRect rcDest, CRect rcSrc, DWORD dwRop=SRCCOPY);
	BOOL	ColorQuantize(int nColorBits);
	BOOL	ChangeToGrayscale(int nMethod = MEAN_GRAY, 
							  double fRedWeight = 0.30, 
							  double fGreenWeight = 0.59, 
							  double fBlueWeight = 0.11);
	BOOL	AdjustColor(int nColorModel, int v1, int v2, int v3);
	BOOL	AdjustBrightness(int v);
	BOOL	AdjustContrast(int v);
	BOOL	SeparateRGB(int nIndex);
	BOOL	FilteRGB(int nIndex);
	BOOL    CorrectGamma(float fGamma);
	BOOL	Negative();
	BOOL	Exposure(BYTE threshold);
	
	// image process
	BOOL	HighPass(int Strength, int nAlgorithm);
	BOOL	LowPass(int Strength, int nAlgorithm);
	BOOL	EdgeEnhance(int Strength, int nAlgorithm);
	BOOL	MedianFilter();
	BOOL	Erosion(BOOL bHoriz);
	BOOL	Dilation(BOOL bHoriz);
	BOOL	MorphOpen(BOOL bHoriz);
	BOOL	MorphClose(BOOL bHoriz);
	BOOL	Contour(BOOL bHori);
	BOOL	Thinning();
	BOOL	FFT();
	BOOL	DCT();
	BOOL	WALh();
	BOOL    Embossment(int nBackground);
	BOOL    Spread(int nBlockSize);
	BOOL	EdegeDetect(int nAlgorithm);
	BOOL	HomogenityEdegeDetect();
	BOOL	DifferenceEdegeDetect();
	BOOL	CannyEdegeDetect();

	//图像处理作业
	BOOL NearestInterpolation(int Strength);
	BOOL BilinearInterpolation();
	BOOL BicubicInterpolation();//双三次插值
	BOOL Mt4();//m通路转换为4通路
	BOOL ImageNegatives();//图像反转
	BOOL LogTransformation(int iA, int iB);//对数变换
	BOOL ContrastStretching(float X1, float Y1, float X2, float Y2);//对比度拉伸
	BOOL LocalImageEnhancement();//基于直方图统计的局部增强方法
	BOOL MyMedianFilter();//中值滤波
	BOOL SobleOperator();//Soble运算
	BOOL LaplacianSharpening();//Laplacian锐化
	BOOL PalmprintImageEnhancement();//对掌纹图像进行图像增强
	BOOL FrequencyDomainFilter();//等效于3*3邻域均值平滑的频率域滤波
	BOOL HomomorphicFilter();//同态滤波
	BOOL BattivoLowPassFilter();//巴特沃思低通
	BOOL BattivoHighPassFilter();//巴特沃思高通
	BOOL BattivoBandpassFilter();//巴特沃思带通
	BOOL BattivoBandRejectionFilter();//巴特沃思带阻
	BOOL Exercise443();//习题4.43
	BOOL VariableThreshold();//可变阈值处理
	BOOL OstuImageSegmentation();//Ostu图像分割方法
	BOOL FaceRecognition();//设计人脸方案
	BOOL IrisSegmentation();//虹膜图像分割
	BOOL BoundaryTrackingAlgorithm();//边界追踪算法
	BOOL RegionRefinement();//二值区域细化算法
	BOOL GraylevelCooccurrenceMatrix();//灰度共生矩阵方法
	BOOL Example1116();//习题11.16
	BOOL Example1127();//习题11.27
	BOOL FigureRecognition();//编程实现印刷体数字识别
	BOOL PeachImageRecognition();//桃子图像识别
	
// private member function
private:
	BOOL	UpdateInternal();
	BOOL	DisplayFadeIn(CDC* pDC, int x, int y, int nDeta, DWORD dwDelayTime);
	BOOL	DisplayFadeOut(CDC* pDC, int x, int y, int nDeta, DWORD dwDelayTime);
	BOOL	DisplayMosaicIn(CDC *pDC, int x, int y, int nTileSize, DWORD dwDelayTime, DWORD dwRop=SRCCOPY);
	BOOL	DisplayMosaicOut(CDC *pDC, int x, int y, int nTileSize, DWORD dwDelayTime);

// public member data
public:
	HDIB		m_hDib;
	HBITMAP		m_hBitmap;	// handle of DIBSection
	CPalette* 	m_pPalette;
	CBitmap*	m_pBitmap;

// private member data
private:
	// for drawing in DIB
	CDC *		m_pMemDC;
	CBitmap*	m_pBitmapTmp;
	CPalette*	m_pPaletteTmp;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIB_H__33817561_072D_11D1_8C94_000021003EA5__INCLUDED_)
