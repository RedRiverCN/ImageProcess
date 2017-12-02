#include "stdafx.h"
#include <math.h>

#include "dibapi.h"
//ͼ������

// Definitions required for convolution image filtering
#define KERNELCOLS 3
#define KERNELROWS 3
#define KERNELELEMENTS (KERNELCOLS * KERNELROWS)

// struct for convolute kernel 
typedef struct 
{
  int Element[KERNELELEMENTS];
  int Divisor;
} KERNEL;

// The following kernel definitions are for convolution filtering.
// Kernel entries are specified with a divisor to get around the
// requirement for floating point numbers in the low pass filters. 

KERNEL HP1 = {                    // HP filter #1
  {-1, -1, -1,
   -1,  9, -1,
   -1, -1, -1},
    1                             // Divisor = 1
};

KERNEL HP2 = {                    // HP filter #2
  { 0, -1,  0,
   -1,  5, -1,
    0, -1,  0},
    1                             // Divisor = 1
};

KERNEL HP3 = {                    // HP filter #3
  { 1, -2,  1,
   -2,  5, -2,
    1, -2,  1},
    1                             // Divisor = 1
};

KERNEL LP1 = {                    // LP filter #1
  { 1,  1,  1,
    1,  1,  1,
    1,  1,  1},
    9                             // Divisor = 9
};

KERNEL LP2 = {                    // LP filter #2
  { 1,  1,  1,
    1,  2,  1,
    1,  1,  1},
    10                            // Divisor = 10
};

KERNEL LP3 = {                    // LP filter #3
  { 1,  2,  1,
    2,  4,  2,
    1,  2,  1},
    16                            // Divisor = 16
};

KERNEL VertEdge = {              // Vertical edge
  { 0,  0,  0,
    -1, 1,  0,
    0,  0,  0},
    1                             // Divisor = 1
};

KERNEL HorzEdge = {              // Horizontal edge
  { 0,  -1,  0,
    0,  1,  0,
    0,  0,  0},
    1                             // Divisor = 1
};

KERNEL VertHorzEdge = {           // Vertical Horizontal edge
  { -1, 0,  0,
    0,  1,  0,
    0,  0,  0},
    1                             // Divisor = 1
};

KERNEL EdgeNorth = {              // North gradient
  { 1,  1,  1,
    1, -2,  1,
   -1, -1, -1},
    1                             // Divisor = 1
};

KERNEL EdgeNorthEast = {          // North East gradient
  { 1,  1,  1,
   -1, -2,  1,
   -1, -1,  1},
    1                             // Divisor = 1
};

KERNEL EdgeEast = {               // East gradient
  {-1,  1,  1,
   -1, -2,  1,
   -1,  1,  1},
    1                             // Divisor = 1
};

KERNEL EdgeSouthEast = {          // South East gradient
  {-1, -1,  1,
   -1, -2,  1,
    1,  1,  1},
    1                             // Divisor = 1
};

KERNEL EdgeSouth = {              // South gadient
  {-1, -1, -1,
    1, -2,  1,
    1,  1,  1},
    1                             // Divisor = 1
};

KERNEL EdgeSouthWest = {          // South West gradient
  { 1, -1, -1,
    1, -2, -1,
    1,  1,  1},
    1                             // Divisor = 1
};

KERNEL EdgeWest = {               // West gradient
  { 1,  1, -1,
    1, -2, -1,
    1,  1, -1},
    1                             // Divisor = 1
};

KERNEL EdgeNorthWest = {          // North West gradient
  { 1,  1,  1,
    1, -2, -1,
    1, -1, -1},
    1                             // Divisor = 1
};

KERNEL Lap1 = {					  // Laplace filter 1
  { 0,  1,  0,
    1, -4,  1,
    0,  1,  0},
    1                             // Divisor = 1
};

KERNEL Lap2 = {					  // Laplace filter 2
  { -1, -1, -1,
    -1,  8, -1,
    -1, -1, -1},
    1                             // Divisor = 1
};

KERNEL Lap3 = {					  // Laplace filter 3
  { -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1},
    1                             // Divisor = 1
};

KERNEL Lap4 = {					  // Laplace filter 4
  { 1, -2, 1,
    -2, 4, -2,
    1, -2, 1},
    1                             // Divisor = 1
};

KERNEL Sobel[2] = {
	{                    // Sobel1
		{-1, 0, 1,
		 -2, 0, 2,
		 -1, 0, 1},
		1                             // Divisor = 1
	},
	{                    // Sobel2
		{1, 2, 1,
		  0,  0,  0,
		-1,-2,-1},
		1                             // Divisor = 1
	}
};

KERNEL Hough[2] = {
	{                    // Hough1
		{-1, 0, 1,
		 -1, 0, 1,
		 -1, 0, 1},
		1                             // Divisor = 1
	},
	{                    // Hough2
		{ 1,  1,  1,
		  0,  0,  0,
		 -1, -1, -1},
		1                             // Divisor = 1
	}
};

// local use macro
#define PIXEL_OFFSET(i, j, nWidthBytes)	\
		(LONG)((LONG)(i)*(LONG)(nWidthBytes) + (LONG)(j)*3)

// local function prototype
int compare(const void *e1, const void *e2);
void DoMedianFilterDIB(int *red, int *green, int *blue, int i, int j, 
			WORD wBytesPerLine, LPBYTE lpDIBits);
void DoConvoluteDIB(int *red, int *green, int *blue, int i, int j, 
			WORD wBytesPerLine, LPBYTE lpDIBits, KERNEL *lpKernel);
BOOL ConvoluteDIB(HDIB hDib, KERNEL *lpKernel, int Strength, int nKernelNum=1);
BOOL EdegeDetectDIB(HDIB hDib, KERNEL *lpKernel);
BOOL GausianFilterDIB(HDIB hDib);

// function body

/************************************************************************* 
 * 
 * HighPassDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * int nAlgorithm   - specify the filter to use
 * int Strength     - operation strength set to the convolute
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * High pass filtering to sharp DIB
 * 
 ************************************************************************/ 
BOOL HighPassDIB(HDIB hDib, int Strength, int nAlgorithm) 
{
	switch (nAlgorithm)
	{
	case FILTER1:
		return ConvoluteDIB(hDib, &HP1, Strength);
	case FILTER2:
		return ConvoluteDIB(hDib, &HP2, Strength);
	case FILTER3:
		return ConvoluteDIB(hDib, &HP3, Strength);
	}

	return FALSE;
}

/************************************************************************* 
 * 
 * LowPassDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * int nAlgorithm   - specify the filter to use
 * int Strength     - operation strength set to the convolute
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * Low pass filtering to blur DIB
 * 
 ************************************************************************/ 
BOOL LowPassDIB(HDIB hDib, int Strength, int nAlgorithm) 
{
	switch (nAlgorithm)
	{
	case FILTER1:
		return ConvoluteDIB(hDib, &LP1, Strength);
	case FILTER2:
		return ConvoluteDIB(hDib, &LP2, Strength);
	case FILTER3:
		return ConvoluteDIB(hDib, &LP3, Strength);
	}

	return FALSE;
}

/************************************************************************* 
 * 
 * EdgeEnhanceDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * int nAlgorithm   - specify the filter to use
 * int Strength     - operation strength set to the convolute
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * Edge enhance DIB
 * 
 ************************************************************************/ 
BOOL EdgeEnhanceDIB(HDIB hDib, int Strength, int nAlgorithm)
{
	switch (nAlgorithm)
	{
	case VERT:
		return ConvoluteDIB(hDib, &VertEdge, Strength);
	case HORZ:
		return ConvoluteDIB(hDib, &HorzEdge, Strength);
	case VERTHORZ:
		return ConvoluteDIB(hDib, &VertHorzEdge, Strength);
	case NORTH:
		return ConvoluteDIB(hDib, &EdgeNorth, Strength);
	case NORTHEAST:
		return ConvoluteDIB(hDib, &EdgeNorthEast, Strength);
	case EAST:
		return ConvoluteDIB(hDib, &EdgeEast, Strength);
	case SOUTH:
		return ConvoluteDIB(hDib, &EdgeSouth, Strength);
	case SOUTHEAST:
		return ConvoluteDIB(hDib, &EdgeSouthEast, Strength);
	case SOUTHWEST:
		return ConvoluteDIB(hDib, &EdgeSouthWest, Strength);
	case WEST:
		return ConvoluteDIB(hDib, &EdgeWest, Strength);
	case NORTHWEST:
		return ConvoluteDIB(hDib, &EdgeNorthWest, Strength);
	case LAP1:
		return ConvoluteDIB(hDib, &Lap1, Strength);
	case LAP2:
		return ConvoluteDIB(hDib, &Lap2, Strength);
	case LAP3:
		return ConvoluteDIB(hDib, &Lap3, Strength);
	case LAP4:
		return ConvoluteDIB(hDib, &Lap4, Strength);
	}

	return FALSE;
}

/************************************************************************* 
 * 
 * MedianFilterDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * This is the media filtering function to DIB
 * 
 ************************************************************************/ 
BOOL MedianFilterDIB(HDIB hDib) 
{
	WaitCursorBegin();

	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);

	if (! hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);
	if (!hFilteredBits) 
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpDIBits = FindDIBBits(lpDIB);

	// convolute...
	for (int i=1; i<wDIBHeight-1; i++) 
		for (int j=1; j<wDIBWidth-1; j++) 
		{
			int  red=0, green=0, blue=0; 
			DoMedianFilterDIB(&red, &green, &blue, i, j, wBytesPerLine, lpDIBits);

			LONG lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
			*(lpDestImage + lOffset++) = BOUND(blue, 0, 255);
			*(lpDestImage + lOffset++) = BOUND(green, 0, 255);
			*(lpDestImage + lOffset)   = BOUND(red, 0, 255);
		}

	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);
	GlobalUnlock(hNewDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
}

/************************************************************************* 
 * 
 * ConvoluteDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * KERNEL *lpKernel - pointer of kernel used to convolute with DIB
 * int Strength     - operation strength set to the convolute
 * int nKernelNum   - kernel number used to convolute
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * This is the generic convolute function to DIB
 * 
 ************************************************************************/ 
BOOL ConvoluteDIB(HDIB hDib, KERNEL *lpKernel, int Strength, int nKernelNum) 
{
	WaitCursorBegin();
	
	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);

	if (! hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);
	if (!hFilteredBits) 
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpDIBits = FindDIBBits(lpDIB);

	// convolute...
	for (int i=1; i<wDIBHeight-1; i++) 
		for (int j=1; j<wDIBWidth-1; j++) 
		{
			int  red=0, green=0, blue=0; 

			for (int k=0; k<nKernelNum; ++k)
			{
				int r=0, g=0, b=0; 
				DoConvoluteDIB(&r, &g, &b, i, j, 
					wBytesPerLine, lpDIBits, lpKernel+k);
				if (r > red)
					red = r;
				if (g > green)
					green = g;
				if (b > blue)
					blue = b;
				//red += r; green += g; blue += b;
			}

			// original RGB value in center pixel  (j, i)
			LONG lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
			BYTE OldB = *(lpDIBits + lOffset++);
			BYTE OldG = *(lpDIBits + lOffset++);
			BYTE OldR = *(lpDIBits + lOffset);
			// When we get here, red, green and blue have the new RGB value.
			if (Strength != 10) 
			{
				// Interpolate pixel data
				red   = OldR + (((red - OldR) * Strength) / 10);
				green = OldG + (((green - OldG) * Strength) / 10);
				blue  = OldB + (((blue - OldB) * Strength) / 10);
			}

			lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
			*(lpDestImage + lOffset++) = BOUND(blue, 0, 255);
			*(lpDestImage + lOffset++) = BOUND(green, 0, 255);
			*(lpDestImage + lOffset)   = BOUND(red, 0, 255);
		}

	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);
	GlobalUnlock(hNewDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
}

// local function: perform convolution to DIB with a kernel
void DoConvoluteDIB(int *red, int *green, int *blue, int i, int j, 
			WORD wBytesPerLine, LPBYTE lpDIBits, KERNEL *lpKernel)
{
	BYTE b[9], g[9], r[9];
	LONG lOffset;
	
	lOffset= PIXEL_OFFSET(i-1,j-1, wBytesPerLine);
	b[0] = *(lpDIBits + lOffset++);
	g[0] = *(lpDIBits + lOffset++);
	r[0] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i-1,j, wBytesPerLine);
	b[1] = *(lpDIBits + lOffset++);
	g[1] = *(lpDIBits + lOffset++);
	r[1] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i-1,j+1, wBytesPerLine);
	b[2] = *(lpDIBits + lOffset++);
	g[2] = *(lpDIBits + lOffset++);
	r[2] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i,j-1, wBytesPerLine);
	b[3] = *(lpDIBits + lOffset++);
	g[3] = *(lpDIBits + lOffset++);
	r[3] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
	b[4] = *(lpDIBits + lOffset++);
	g[4] = *(lpDIBits + lOffset++);
	r[4] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i,j+1, wBytesPerLine);
	b[5] = *(lpDIBits + lOffset++);
	g[5] = *(lpDIBits + lOffset++);
	r[5] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i+1,j-1, wBytesPerLine);
	b[6] = *(lpDIBits + lOffset++);
	g[6] = *(lpDIBits + lOffset++);
	r[6] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i+1,j, wBytesPerLine);
	b[7] = *(lpDIBits + lOffset++);
	g[7] = *(lpDIBits + lOffset++);
	r[7] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i+1,j+1, wBytesPerLine);
	b[8] = *(lpDIBits + lOffset++);
	g[8] = *(lpDIBits + lOffset++);
	r[8] = *(lpDIBits + lOffset);

	*red = *green = *blue = 0;
	for (int k=0; k<=8; ++k)
	{
		*red   += lpKernel->Element[k]*r[k];
		*green += lpKernel->Element[k]*g[k];
		*blue  += lpKernel->Element[k]*b[k];
	}

	if (lpKernel->Divisor != 1) 
	{
		*red   /= lpKernel->Divisor;
		*green /= lpKernel->Divisor;
		*blue  /= lpKernel->Divisor;
	}

	// getoff opposite
	*red   = abs(*red);
	*green = abs(*green);
	*blue  = abs(*blue);
}

// local function: perform median filter to DIB
void DoMedianFilterDIB(int *red, int *green, int *blue, int i, int j, 
			WORD wBytesPerLine, LPBYTE lpDIBits)
{
	BYTE b[9], g[9], r[9];
	LONG lOffset;
	
	lOffset= PIXEL_OFFSET(i-1,j-1, wBytesPerLine);
	b[0] = *(lpDIBits + lOffset++);
	g[0] = *(lpDIBits + lOffset++);
	r[0] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i-1,j, wBytesPerLine);
	b[1] = *(lpDIBits + lOffset++);
	g[1] = *(lpDIBits + lOffset++);
	r[1] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i-1,j+1, wBytesPerLine);
	b[2] = *(lpDIBits + lOffset++);
	g[2] = *(lpDIBits + lOffset++);
	r[2] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i,j-1, wBytesPerLine);
	b[3] = *(lpDIBits + lOffset++);
	g[3] = *(lpDIBits + lOffset++);
	r[3] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
	b[4] = *(lpDIBits + lOffset++);
	g[4] = *(lpDIBits + lOffset++);
	r[4] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i,j+1, wBytesPerLine);
	b[5] = *(lpDIBits + lOffset++);
	g[5] = *(lpDIBits + lOffset++);
	r[5] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i+1,j-1, wBytesPerLine);
	b[6] = *(lpDIBits + lOffset++);
	g[6] = *(lpDIBits + lOffset++);
	r[6] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i+1,j, wBytesPerLine);
	b[7] = *(lpDIBits + lOffset++);
	g[7] = *(lpDIBits + lOffset++);
	r[7] = *(lpDIBits + lOffset);

	lOffset= PIXEL_OFFSET(i+1,j+1, wBytesPerLine);
	b[8] = *(lpDIBits + lOffset++);
	g[8] = *(lpDIBits + lOffset++);
	r[8] = *(lpDIBits + lOffset);

    qsort(r, 9, 1, compare);
    qsort(g, 9, 1, compare);
    qsort(b, 9, 1, compare);

	*red   = r[0];
	*green = g[0];
	*blue  = b[0];
}

// function used to sort in the call of qsort
int compare(const void *e1, const void *e2)
{
	if (*(BYTE *)e1 < *(BYTE *)e2)
		return -1;
	if (*(BYTE *)e1 > *(BYTE *)e2)
		return 1;

	return 0;
}

/************************************************************************* 
 * 
 * ErosionDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * BOOL bHori	    - erosion direction
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * This function do erosion with the specified direction
 * 
 ************************************************************************/ 
BOOL ErosionDIB(HDIB hDib, BOOL bHori)
{
	// start wait cursor
	WaitCursorBegin();

    // Old DIB buffer
	if (hDib == NULL)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// only support 256 color image
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 8)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// new DIB
	HDIB hNewDIB = CopyHandle(hDib);
	if (! hNewDIB)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// source dib buffer
    LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDib);
	if (! lpSrcDIB)
	{
		WaitCursorBegin();
		return FALSE;
	}
    // New DIB buffer
    LPBITMAPINFO lpbmi = (LPBITMAPINFO)GlobalLock(hNewDIB);
	if (! lpbmi)
	{
		WaitCursorBegin();
		return FALSE;
	}

	// start erosion...
	LPSTR lpPtr;
	LPSTR lpTempPtr;
	LONG  x,y;
	BYTE  num, num0;
	int   i;
	LONG lHeight = DIBHeight(lpSrcDIB);
	LONG lWidth = DIBWidth(lpSrcDIB);
	DWORD dwBufferSize = GlobalSize(lpSrcDIB);
	int nLineBytes = BytesPerLine(lpSrcDIB);
	if(bHori)
	{
		for (y=0; y<lHeight; y++)
		{
			lpPtr=(char *)lpbmi+(dwBufferSize-nLineBytes-y*nLineBytes)+1;
			lpTempPtr=(char *)lpSrcDIB+(dwBufferSize-nLineBytes-y*nLineBytes)+1;
			for (x=1; x<lWidth-1; x++)
			{
				num0 = num = 0 ;
				for(i=0;i<3;i++)
				{
					num=(unsigned char)*(lpPtr+i-1);
					if(num > num0)
						num0 = num;
				}
				*lpTempPtr=(unsigned char)num0;
				/*
				num=(unsigned char)*lpPtr;
				if (num==0)
				{
					*lpTempPtr=(unsigned char)0;
					for(i=0;i<3;i++)
					{
						num=(unsigned char)*(lpPtr+i-1);
						if(num==255)
						{
							*lpTempPtr=(unsigned char)255;
							break;
						}
					}
				}
				else 
					*lpTempPtr=(unsigned char)255;
				*/
				lpPtr++;
				lpTempPtr++;
			}
		}
	}
	else		// Vertical
	{
		for (y=1; y<lHeight-1; y++)
		{
			lpPtr=(char *)lpbmi+(dwBufferSize-nLineBytes-y*nLineBytes);
			lpTempPtr=(char *)lpSrcDIB+(dwBufferSize-nLineBytes-y*nLineBytes);
			for (x=0; x<lWidth; x++)
			{
				num0 = num = 0 ;
				for(i=0;i<3;i++)
				{
					num=(unsigned char)*(lpPtr+i-1);
					if(num > num0)
						num0 = num;
				}
				*lpTempPtr=(unsigned char)num0;
				/*
				num=(unsigned char)*lpPtr;
				if (num==0)
				{
					*lpTempPtr=(unsigned char)0;
					for(i=0;i<3;i++)
					{
						num=(unsigned char)*(lpPtr+(i-1)*nLineBytes);
						if(num==255)
						{
							*lpTempPtr=(unsigned char)255;
							break;
						}
					}
				}
				else 
					*lpTempPtr=(unsigned char)255;
				*/
				lpPtr++;
				lpTempPtr++;
			}
		}
	}

	// cleanup
	GlobalUnlock(hDib);
	GlobalUnlock(hNewDIB);
	GlobalFree(hNewDIB);
	WaitCursorEnd();

	return TRUE;
}

/************************************************************************* 
 * 
 * DilationDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * BOOL bHori	    - dilation direction
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * This function do dilation with the specified direction
 * 
 ************************************************************************/ 
BOOL DilationDIB(HDIB hDib, BOOL bHori)
{
	// start wait cursor
	WaitCursorBegin();

    // Old DIB buffer
	if (hDib == NULL)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// only support 256 color image
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 8)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// new DIB
	HDIB hNewDIB = CopyHandle(hDib);
	if (! hNewDIB)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// source dib buffer
    LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDib);
	if (! lpSrcDIB)
	{
		WaitCursorBegin();
		return FALSE;
	}
    // New DIB buffer
    LPBITMAPINFO lpbmi = (LPBITMAPINFO)GlobalLock(hNewDIB);
	if (! lpbmi)
	{
		WaitCursorBegin();
		return FALSE;
	}

	// start erosion...
	LPSTR lpPtr;
	LPSTR lpTempPtr;
	LONG  x,y;
	BYTE  num, num0;
	int   i;
	LONG lHeight = DIBHeight(lpSrcDIB);
	LONG lWidth = DIBWidth(lpSrcDIB);
	DWORD dwBufferSize = GlobalSize(lpSrcDIB);
	int nLineBytes = BytesPerLine(lpSrcDIB);
	if(bHori)
	{
		for(y=0;y<lHeight;y++)
		{
			lpPtr=(char *)lpbmi+(dwBufferSize-nLineBytes-y*nLineBytes)+1;
			lpTempPtr=(char *)lpSrcDIB+(dwBufferSize-nLineBytes-y*nLineBytes)+1;
			for(x=1;x<lWidth-1;x++)
			{
				num0 = num = 255;
				for(i=0;i<3;i++)
				{
					num=(unsigned char)*(lpPtr+i-1);
					if(num < num0)
						num0 = num;
				}
				*lpTempPtr=(unsigned char)num0;
				/*
				num=(unsigned char)*lpPtr;
				if (num==255)
				{
					*lpTempPtr=(unsigned char)255;
					for(i=0;i<3;i++)
					{
						num=(unsigned char)*(lpPtr+i-1);
						if(num==0)
						{
							*lpTempPtr=(unsigned char)0;
							break;
						}
					}
				}
				else 
					*lpTempPtr=(unsigned char)0;
				*/
				lpPtr++;
				lpTempPtr++;
			}
		}
	}
	else
	{
		for(y=1;y<lHeight-1;y++)
		{
			lpPtr=(char *)lpbmi+(dwBufferSize-nLineBytes-y*nLineBytes);
			lpTempPtr=(char *)lpSrcDIB+(dwBufferSize-nLineBytes-y*nLineBytes);
			for(x=0;x<lWidth;x++)
			{
				num0 = num = 255;
				for(i=0;i<3;i++)
				{
					num=(unsigned char)*(lpPtr+i-1);
					if(num < num0)
						num0 = num;
				}
				*lpTempPtr=(unsigned char)num0;
				/*
				num=(unsigned char)*lpPtr;
				if (num==255)
				{
					*lpTempPtr=(unsigned char)255;
					for(i=0;i<3;i++)
					{
						num=(unsigned char)*(lpPtr+(i-1)*nLineBytes);
						if(num==0)
						{
							*lpTempPtr=(unsigned char)0;
							break;
						}
					}
				}
				else 
					*lpTempPtr=(unsigned char)0;
				*/
				lpPtr++;
				lpTempPtr++;
			}
		}
	}

	// cleanup
	GlobalUnlock(hDib);
	GlobalUnlock(hNewDIB);
	GlobalFree(hNewDIB);
	WaitCursorEnd();

	return TRUE;
}

/************************************************************************* 
 * 
 * MorphOpenDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * BOOL bHori	    - open direction
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * This function do open with the specified direction
 * 
 ************************************************************************/ 
BOOL MorphOpenDIB(HDIB hDib, BOOL bHori)
{
	// Step 1: erosion
	if (! ErosionDIB(hDib, bHori))
		return FALSE;

	// Step 2: dilation	
	if (! DilationDIB(hDib, bHori))
		return FALSE;

	return TRUE;
}

/************************************************************************* 
 * 
 * MorphCloseDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * BOOL bHori	    - close direction
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * This function do close with the specified direction
 * 
 ************************************************************************/ 
BOOL MorphCloseDIB(HDIB hDib, BOOL bHori)
{
	// Step 1: dilation	
	if (! DilationDIB(hDib, bHori))
		return FALSE;

	// Step 2: erosion
	if (! ErosionDIB(hDib, bHori))
		return FALSE;

	return TRUE;
}

/************************************************************************* 
 * 
 * ContourDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * BOOL bHori	    - open direction
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * This function contour DIB with the specified direction
 * 
 ************************************************************************/ 
BOOL ContourDIB(HDIB hDib, BOOL bHori)
{
	// start wait cursor
	WaitCursorBegin();

    // Old DIB buffer
	if (hDib == NULL)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// only support 256 color image
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 8)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// new DIB
	HDIB hNewDIB = CopyHandle(hDib);
	if (! hNewDIB)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// source dib buffer
    LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDib);
	if (! lpSrcDIB)
	{
		WaitCursorBegin();
		return FALSE;
	}
    // New DIB buffer
    LPBITMAPINFO lpbmi = (LPBITMAPINFO)GlobalLock(hNewDIB);
	if (! lpbmi)
	{
		WaitCursorBegin();
		return FALSE;
	}

	// start erosion...
	LPBYTE lpPtr;
	LPBYTE lpTempPtr;
	LONG  x,y;
	BYTE  num, num0;
	int   i;
	LONG lHeight = DIBHeight(lpSrcDIB);
	LONG lWidth = DIBWidth(lpSrcDIB);
	DWORD dwBufferSize = GlobalSize(lpSrcDIB);
	int nLineBytes = BytesPerLine(lpSrcDIB);

	// Step 1: erosion
	if(bHori)
	{
		for (y=0; y<lHeight; y++)
		{
			lpPtr=(BYTE *)lpSrcDIB+(dwBufferSize-nLineBytes-y*nLineBytes)+1;
			lpTempPtr=(BYTE *)lpbmi+(dwBufferSize-nLineBytes-y*nLineBytes)+1;
			for (x=1; x<lWidth-1; x++)
			{
				num0 = num = 0 ;
				for(i=0;i<3;i++)
				{
					num=(unsigned char)*(lpPtr+i-1);
					if(num > num0)
						num0 = num;
				}
				*lpTempPtr=(unsigned char)num0;
				/*
				num=(unsigned char)*lpPtr;
				if (num==0)
				{
					*lpTempPtr=(unsigned char)0;
					for(i=0;i<3;i++)
					{
						num=(unsigned char)*(lpPtr+i-1);
						if(num==255)
						{
							*lpTempPtr=(unsigned char)255;
							break;
						}
					}
				}
				else 
					*lpTempPtr=(unsigned char)255;
				*/
				lpPtr++;
				lpTempPtr++;
			}
		}
	}
	else		// Vertical
	{
		for (y=1; y<lHeight-1; y++)
		{
			lpPtr=(BYTE *)lpSrcDIB+(dwBufferSize-nLineBytes-y*nLineBytes);
			lpTempPtr=(BYTE *)lpbmi+(dwBufferSize-nLineBytes-y*nLineBytes);
			for (x=0; x<lWidth; x++)
			{
				num0 = num = 0 ;
				for(i=0;i<3;i++)
				{
					num=(unsigned char)*(lpPtr+i-1);
					if(num > num0)
						num0 = num;
				}
				*lpTempPtr=(unsigned char)num0;
				/*
				num=(unsigned char)*lpPtr;
				if (num==0)
				{
					*lpTempPtr=(unsigned char)0;
					for(i=0;i<3;i++)
					{
						num=(unsigned char)*(lpPtr+(i-1)*nLineBytes);
						if(num==255)
						{
							*lpTempPtr=(unsigned char)255;
							break;
						}
					}
				}
				else 
					*lpTempPtr=(unsigned char)255;
				*/
				lpPtr++;
				lpTempPtr++;
			}
		}
	}
	
	// Step 2: original image minues dilation image
	if(bHori)
	{
		for(y=0;y<lHeight;y++)
		{
			lpPtr=(BYTE *)lpbmi+(dwBufferSize-nLineBytes-y*nLineBytes)+1;
			lpTempPtr=(BYTE *)lpSrcDIB+(dwBufferSize-nLineBytes-y*nLineBytes)+1;
			for(x=1;x<lWidth-1;x++)
			{
				if (*lpTempPtr == *lpPtr)
					*lpTempPtr = (BYTE)255;
				else
					*lpTempPtr = *lpTempPtr - *lpPtr;

				lpPtr++;
				lpTempPtr++;
			}
		}
	}
	else
	{
		for(y=1;y<lHeight-1;y++)
		{
			lpPtr=(BYTE *)lpbmi+(dwBufferSize-nLineBytes-y*nLineBytes);
			lpTempPtr=(BYTE *)lpSrcDIB+(dwBufferSize-nLineBytes-y*nLineBytes);
			for(x=0;x<lWidth;x++)
			{
				if (*lpTempPtr == *lpPtr)
					*lpTempPtr = (BYTE)255;
				else
					*lpTempPtr = *lpTempPtr - *lpPtr;

				lpPtr++;
				lpTempPtr++;
			}
		}
	}

	// cleanup
	GlobalUnlock(hDib);
	GlobalUnlock(hNewDIB);
	GlobalFree(hNewDIB);

	return TRUE;
}

/************************************************************************* 
 * 
 * ThinningDIB() 
 * 
 * Parameters: 
 * 
 * HDIB hDib        - objective DIB handle
 * 
 * Return Value: 
 * 
 * BOOL             - True is success, else False
 * 
 * Description: 
 * 
 * This function thins a DIB
 * 
 ************************************************************************/ 
BOOL ThinningDIB(HDIB hDib)
{
	static int erasetable[256]=
	{
		0,0,1,1,0,0,1,1,
		1,1,0,1,1,1,0,1,
		1,1,0,0,1,1,1,1,
		0,0,0,0,0,0,0,1,

		0,0,1,1,0,0,1,1,
		1,1,0,1,1,1,0,1,
		1,1,0,0,1,1,1,1,
		0,0,0,0,0,0,0,1,

		1,1,0,0,1,1,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,

		1,1,0,0,1,1,0,0,
		1,1,0,1,1,1,0,1,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,

		0,0,1,1,0,0,1,1,
		1,1,0,1,1,1,0,1,
		1,1,0,0,1,1,1,1,
		0,0,0,0,0,0,0,1,

		0,0,1,1,0,0,1,1,
		1,1,0,1,1,1,0,1,
		1,1,0,0,1,1,1,1,
		0,0,0,0,0,0,0,0,

		1,1,0,0,1,1,0,0,
		0,0,0,0,0,0,0,0,
		1,1,0,0,1,1,1,1,
		0,0,0,0,0,0,0,0,

		1,1,0,0,1,1,0,0,
		1,1,0,1,1,1,0,0,
		1,1,0,0,1,1,1,0,
		1,1,0,0,1,0,0,0
	};

	// start wait cursor
	WaitCursorBegin();

    // Old DIB buffer
	if (hDib == NULL)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// only support 256 color image
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 8)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// new DIB
	HDIB hNewDIB = CopyHandle(hDib);
	if (! hNewDIB)
	{
		WaitCursorEnd();
        return FALSE;
	}

	// source dib buffer
    LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDib);
	if (! lpSrcDIB)
	{
		WaitCursorBegin();
		return FALSE;
	}
    // New DIB buffer
    LPBITMAPINFO lpbmi = (LPBITMAPINFO)GlobalLock(hNewDIB);
	if (! lpbmi)
	{
		WaitCursorBegin();
		return FALSE;
	}

	// start erosion...
	LPSTR lpPtr;
	LPSTR lpTempPtr;
	LONG  x,y;
	BYTE  num;
	LONG lHeight = DIBHeight(lpSrcDIB);
	LONG lWidth = DIBWidth(lpSrcDIB);
	DWORD dwBufferSize = GlobalSize(lpSrcDIB);
	int nLineBytes = BytesPerLine(lpSrcDIB);
	int nw,n,ne,w,e,sw,s,se;

	BOOL Finished=FALSE;
    while(!Finished)
	{
    	Finished=TRUE;
		for (y=1;y<lHeight-1;y++)
		{ 
			lpPtr=(char *)lpbmi+(dwBufferSize-nLineBytes-y*nLineBytes);
			lpTempPtr=(char *)lpSrcDIB+(dwBufferSize-nLineBytes-y*nLineBytes);
			x=1; 
			while(x<lWidth-1)
			{
				if(*(lpPtr+x)==0)
				{
					w=(unsigned char)*(lpPtr+x-1);
					e=(unsigned char)*(lpPtr+x+1);
					if( (w==255)|| (e==255))
					{
						nw=(unsigned char)*(lpPtr+x+nLineBytes-1);
						n=(unsigned char)*(lpPtr+x+nLineBytes);
						ne=(unsigned char)*(lpPtr+x+nLineBytes+1);
						sw=(unsigned char)*(lpPtr+x-nLineBytes-1);
						s=(unsigned char)*(lpPtr+x-nLineBytes);
						se=(unsigned char)*(lpPtr+x-nLineBytes+1);
						num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+sw/255*32+s/255*64+se/255*128;
						if(erasetable[num]==1)
						{
							*(lpPtr+x)=(BYTE)255;
							*(lpTempPtr+x)=(BYTE)255;
							Finished=FALSE;
							x++;
						}
					}
				}
				x++;
			}
		}
	
		for (x=1;x<lWidth-1;x++)
		{ 
			y=1;
			while(y<lHeight-1)
			{
				lpPtr=(char *)lpbmi+(dwBufferSize-nLineBytes-y*nLineBytes);
				lpTempPtr=(char *)lpSrcDIB+(dwBufferSize-nLineBytes-y*nLineBytes);
				if(*(lpPtr+x)==0)
				{
					n=(unsigned char)*(lpPtr+x+nLineBytes);
					s=(unsigned char)*(lpPtr+x-nLineBytes);
					if( (n==255)|| (s==255))
					{
						nw=(unsigned char)*(lpPtr+x+nLineBytes-1);
						ne=(unsigned char)*(lpPtr+x+nLineBytes+1);
						w=(unsigned char)*(lpPtr+x-1);
						e=(unsigned char)*(lpPtr+x+1);
						sw=(unsigned char)*(lpPtr+x-nLineBytes-1);
						se=(unsigned char)*(lpPtr+x-nLineBytes+1);
						num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+sw/255*32+s/255*64+se/255*128;
						if(erasetable[num]==1)
						{
							*(lpPtr+x)=(BYTE)255;
							*(lpTempPtr+x)=(BYTE)255;
							Finished=FALSE;
							y++;
						}
					}
				}
				y++;
			}
		} 
	}

	// cleanup
	GlobalUnlock(hDib);
	GlobalUnlock(hNewDIB);
	GlobalFree(hNewDIB);

	return TRUE;
}

//////////////////////////////////////////////////////////
// internal definitions

#define PI (double)3.14159265359

/*complex number*/
typedef struct
{
	double re;
	double im;
}COMPLEX;

/*complex add*/
COMPLEX Add(COMPLEX c1, COMPLEX c2)
{
	COMPLEX c;
	c.re=c1.re+c2.re;
	c.im=c1.im+c2.im;
	return c;
}

/*complex substract*/
COMPLEX Sub(COMPLEX c1, COMPLEX c2)
{
	COMPLEX c;
	c.re=c1.re-c2.re;
	c.im=c1.im-c2.im;
	return c;
}

/*complex multiple*/
COMPLEX Mul(COMPLEX c1, COMPLEX c2)
{
	COMPLEX c;
	c.re=c1.re*c2.re-c1.im*c2.im;
	c.im=c1.re*c2.im+c2.re*c1.im;
	return c;
}
//////////////////////////////////////////////////////////

/*
void FFT(COMPLEX * TD, COMPLEX * FD, int power);
void IFFT(COMPLEX * FD, COMPLEX * TD, int power);
void DCT(double *f, double *F, int power);
void IDCT(double *F, double *f, int power);
void WALh(double *f, double *W, int power);
void IWALh(double *W, double *f, int power);
*/

/****************************************************
	FFT()

	������

		TDΪʱ��ֵ
		FDΪƵ��ֵ
		powerΪ2������

	����ֵ��

		��

	˵����

		������ʵ�ֿ��ٸ���Ҷ�任
****************************************************/
void FFT(COMPLEX * TD, COMPLEX * FD, int power)
{
	int count;
	int i,j,k,bfsize,p;
	double angle;
	COMPLEX *W,*X1,*X2,*X;

	/*���㸵��Ҷ�任����*/
	count=1<<power;
	
	/*������������洢��*/
	W=(COMPLEX *)malloc(sizeof(COMPLEX)*count/2);
	X1=(COMPLEX *)malloc(sizeof(COMPLEX)*count);
	X2=(COMPLEX *)malloc(sizeof(COMPLEX)*count);
	
	/*�����Ȩϵ��*/
	for(i=0;i<count/2;i++)
	{
		angle=-i*PI*2/count;
		W[i].re=cos(angle);
		W[i].im=sin(angle);
	}
	
	/*��ʱ���д��洢��*/
	memcpy(X1,TD,sizeof(COMPLEX)*count);
	
	/*��������*/
	for(k=0;k<power;k++)
	{
		for(j=0;j<1<<k;j++)
		{
			bfsize=1<<(power-k);
			for(i=0;i<bfsize/2;i++)
			{
				p=j*bfsize;
				X2[i+p]=Add(X1[i+p],X1[i+p+bfsize/2]);
				X2[i+p+bfsize/2]=Mul(Sub(X1[i+p],X1[i+p+bfsize/2]),W[i*(1<<k)]);
			}
		}
		X=X1;
		X1=X2;
		X2=X;
	}
	
	/*��������*/
	for(j=0;j<count;j++)
	{
		p=0;
		for(i=0;i<power;i++)
		{
			if (j&(1<<i)) p+=1<<(power-i-1);
		}
		FD[j]=X1[p];
	}
	
	/*�ͷŴ洢��*/
	free(W);
	free(X1);
	free(X2);
}

/****************************************************
	IFFT()

	������

		FDΪƵ��ֵ
		TDΪʱ��ֵ
		powerΪ2������

	����ֵ��

		��

	˵����

		���������ÿ��ٸ���Ҷ�任ʵ�ָ���Ҷ���任
****************************************************/
void IFFT(COMPLEX * FD, COMPLEX * TD, int power)
{
	int i, count;
	COMPLEX *x;

	/*���㸵��Ҷ���任����*/
	count=1<<power;

	/*������������洢��*/
	x=(COMPLEX *)malloc(sizeof(COMPLEX)*count);

	/*��Ƶ���д��洢��*/
	memcpy(x,FD,sizeof(COMPLEX)*count);
	
	/*��Ƶ���Ĺ���*/
	for(i=0;i<count;i++)
		x[i].im = -x[i].im;

	/*����FFT*/
	FFT(x, TD, power);

	/*��ʱ���Ĺ���*/
	for(i=0;i<count;i++)
	{
		TD[i].re /= count;
		TD[i].im = -TD[i].im / count;
	}

	/*�ͷŴ洢��*/
	free(x);
}

/*******************************************************
	DCT()

	������

		fΪʱ��ֵ
		FΪƵ��ֵ
		powerΪ2������

	����ֵ��

		��

	˵����

		���������ÿ��ٸ���Ҷ�任ʵ�ֿ�����ɢ���ұ任
********************************************************/
void DCT(double *f, double *F, int power)
{
	int i,count;
	COMPLEX *X;
	double s;

	/*������ɢ���ұ任����*/
	count=1<<power;
	
	/*������������洢��*/
	X=(COMPLEX *)malloc(sizeof(COMPLEX)*count*2);
	
	/*����*/
	memset(X,0,sizeof(COMPLEX)*count*2);
	
	/*��ʱ���д��洢��*/
	for(i=0;i<count;i++)
	{
		X[i].re=f[i];
	}
	
	/*���ÿ��ٸ���Ҷ�任*/
	FFT(X,X,power+1);
	
	/*����ϵ��*/
	s=1/sqrt(count);
	F[0]=X[0].re*s;
	s*=sqrt(2);
	for(i=1;i<count;i++)
	{
		F[i]=(X[i].re*cos(i*PI/(count*2))+X[i].im*sin(i*PI/(count*2)))*s;
	}
	
	/*�ͷŴ洢��*/
	free(X);
}

/************************************************************
	IDCT()

	������

		FΪƵ��ֵ
		fΪʱ��ֵ
		powerΪ2������

	����ֵ��

		��

	˵����

		���������ÿ��ٸ���Ҷ���任ʵ�ֿ�����ɢ�����ұ任
*************************************************************/
void IDCT(double *F, double *f, int power)
{
	int i,count;
	COMPLEX *X;
	double s;

	/*������ɢ�����ұ任����*/
	count=1<<power;
	
	/*������������洢��*/
	X=(COMPLEX *)malloc(sizeof(COMPLEX)*count*2);
	
	/*����*/
	memset(X,0,sizeof(COMPLEX)*count*2);
	
	/*����Ƶ��㣬д��洢��*/
	for(i=0;i<count;i++)
	{
		X[i].re=F[i]*cos(i*PI/(count*2));
		X[i].im=F[i]*sin(i*PI/(count*2));
	}
	
	/*���ÿ��ٸ���Ҷ���任*/
	IFFT(X,X,power+1);
	
	/*����ϵ��*/
	s=1/sqrt(count);
	for(i=1;i<count;i++)
	{
		f[i]=(1-sqrt(2))*s*F[0]+sqrt(2)*s*X[i].re*count*2;
	}
	
	/*�ͷŴ洢��*/
	free(X);
}

/**********************************************************
	WALh()

	������

		fΪʱ��ֵ
		WΪƵ��ֵ
		powerΪ2������

	����ֵ��

		��

	˵����

		���������ÿ��ٸ���Ҷ�任ʵ�ֿ����ֶ�ʲ-������任
*************************************************************/
void WALh(double *f, double *W, int power)
{
	int count;
	int i,j,k,bfsize,p;
	double *X1,*X2,*X;

	/*��������ֶ�ʲ�任����*/

	count=1<<power;
	
	/*������������洢��*/
	X1=(double *)malloc(sizeof(double)*count);
	X2=(double *)malloc(sizeof(double)*count);
	
	/*��ʱ���д��洢��*/
	memcpy(X1,f,sizeof(double)*count);
	
	/*��������*/
	for(k=0;k<power;k++)
	{
		for(j=0;j<1<<k;j++)
		{
			bfsize=1<<(power-k);
			for(i=0;i<bfsize/2;i++)
			{
				p=j*bfsize;
				X2[i+p]=X1[i+p]+X1[i+p+bfsize/2];
				X2[i+p+bfsize/2]=X1[i+p]-X1[i+p+bfsize/2];
			}
		}
		X=X1;
		X1=X2;
		X2=X;
	}
	/*����ϵ��*/
//	for(i=0;i<count;i++)
//	{
//		W[i]=X1[i]/count;
//	}
	for(j=0;j<count;j++)
	{
		p=0;
		for(i=0;i<power;i++)
		{
			if (j&(1<<i)) p+=1<<(power-i-1);
		}
		W[j]=X1[p]/count;
	}
	
	/*�ͷŴ洢��*/
	free(X1);
	free(X2);
}

/*********************************************************************
	IWALh()

	������

		WΪƵ��ֵ
		fΪʱ��ֵ
		powerΪ2������

	����ֵ��

		��

	˵����

		���������ÿ����ֶ�ʲ-������任ʵ�ֿ����ֶ�ʲ-�����귴�任
**********************************************************************/
void IWALh(double *W, double *f, int power)
{
	int i, count;

	/*��������ֶ�ʲ���任����*/
	count=1<<power;

	/*���ÿ����ֶ�ʲ-������任*/
	WALh(W, f, power);

	/*����ϵ��*/
	for(i=0;i<count;i++)
	{
		f[i] *= count;
	}
}

//////////////////////////////////////////////////////////////////////////////////
// internal functions

#define Point(x,y) lpPoints[(x)+(y)*nWidth]

void GetPoints(int nWidth,int nHeight,BYTE *lpBits,BYTE *lpPoints)
{
	int x,y,p;
	int nByteWidth = WIDTHBYTES(nWidth*24); //nWidth*3;
	//if (nByteWidth%4) nByteWidth+=4-(nByteWidth%4);

	for(y=0;y<nHeight;y++)
	{
		for(x=0;x<nWidth;x++)
		{
			p=x*3+y*nByteWidth;
			lpPoints[x+y*nWidth]=(BYTE)(0.299*(float)lpBits[p+2]+0.587*(float)lpBits[p+1]+0.114*(float)lpBits[p]+0.1);
		}
	}
}

void PutPoints(int nWidth,int nHeight,BYTE *lpBits,BYTE *lpPoints)
{
	int x,y,p,p1;
	int nByteWidth = WIDTHBYTES(nWidth*24); //nWidth*3;
	//if (nByteWidth%4) nByteWidth+=4-(nByteWidth%4);

	for(y=0;y<nHeight;y++)
	{
		for(x=0;x<nWidth;x++)
		{
			p=x*3+y*nByteWidth;
			p1=x+y*nWidth;
			lpBits[p]=lpPoints[p1];
			lpBits[p+1]=lpPoints[p1];
			lpBits[p+2]=lpPoints[p1];
		}
	}
}
//////////////////////////////////////////////////////////////////////////////

/****************************************************
	FFTDIB()

	������

		hDIBΪ�����DIB���

	����ֵ��

		�ɹ�ΪTRUE��ʧ��ΪFALSE

	˵����

		������ʵ��DIBλͼ�Ŀ��ٸ���Ҷ�任
****************************************************/
BOOL FFTDIB(HDIB hDIB)
{
	if (hDIB == NULL)
		return FALSE;

	// start wait cursor
	WaitCursorBegin();

	HDIB hDib = NULL;
	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDIB);
	if (wBitCount != 24)
	{
		hNewDib = ConvertDIBFormat(hDIB, 24, NULL);
		hDib = CopyHandle(hNewDib);
	}
	else
	{
		hNewDib = CopyHandle(hDIB);
		hDib = CopyHandle(hDIB);
	}

	if (hNewDib == NULL && hDib == NULL)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// process!
	LPBYTE lpSrcDIB = (LPBYTE)GlobalLock(hDib);
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpInput = FindDIBBits(lpSrcDIB);
	LPBYTE lpOutput = FindDIBBits(lpDIB);
	int nWidth = DIBWidth(lpSrcDIB);
	int nHeight = DIBHeight(lpSrcDIB);
	
	int w=1,h=1,wp=0,hp=0;
	while(w*2<=nWidth)
	{
		w*=2;
		wp++;
	}
	while(h*2<=nHeight)
	{
		h*=2;
		hp++;
	}
	int x,y;
	BYTE *lpPoints=new BYTE[nWidth*nHeight];
	GetPoints(nWidth,nHeight,lpInput,lpPoints);

	COMPLEX *TD=new COMPLEX[w*h];
	COMPLEX *FD=new COMPLEX[w*h];

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			TD[x+w*y].re=Point(x,y);
			TD[x+w*y].im=0;
		}
	}

	for(y=0;y<h;y++)
	{
		FFT(&TD[w*y],&FD[w*y],wp);
	}
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			TD[y+h*x]=FD[x+w*y];
//			TD[x+w*y]=FD[x*h+y];
		}
	}
	for(x=0;x<w;x++)
	{
		FFT(&TD[x*h],&FD[x*h],hp);
	}

	memset(lpPoints,0,nWidth*nHeight);
	double m;
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			m=sqrt(FD[x*h+y].re*FD[x*h+y].re+FD[x*h+y].im*FD[x*h+y].im)/100;
			if (m>255) m=255;
			Point((x<w/2?x+w/2:x-w/2),nHeight-1-(y<h/2?y+h/2:y-h/2))=(BYTE)(m);
		}
	}
	delete TD;
	delete FD;
	PutPoints(nWidth,nHeight,lpOutput,lpPoints);
	delete lpPoints;

	// recover
	DWORD dwSize = GlobalSize(hDib);
	memcpy(lpSrcDIB, lpDIB, dwSize);
	GlobalUnlock(hDib);
	GlobalUnlock(hNewDib);
	if (wBitCount != 24)
	{
		hNewDib = ConvertDIBFormat(hDib, wBitCount, NULL);

		lpSrcDIB = (LPBYTE)GlobalLock(hDIB);
		lpDIB = (LPBYTE)GlobalLock(hNewDib);
		dwSize = GlobalSize(hNewDib);
		memcpy(lpSrcDIB, lpDIB, dwSize);
		GlobalUnlock(hDIB);
		GlobalUnlock(hNewDib);
	}
	else
	{
		lpSrcDIB = (LPBYTE)GlobalLock(hDIB);
		lpDIB = (LPBYTE)GlobalLock(hDib);
		dwSize = GlobalSize(hDib);
		memcpy(lpSrcDIB, lpDIB, dwSize);
		GlobalUnlock(hDIB);
		GlobalUnlock(hDib);
	}

	// cleanup
	GlobalFree(hDib);
	GlobalFree(hNewDib);

	// return
	WaitCursorEnd();
	return TRUE;
}

/****************************************************
	DCTDIB()

	������

		hDIBΪ�����DIB���

	����ֵ��

		�ɹ�ΪTRUE��ʧ��ΪFALSE

	˵����

		������ʵ��DIBλͼ�Ŀ������ұ任
****************************************************/
BOOL DCTDIB(HDIB hDIB)
{
	if (hDIB == NULL)
		return FALSE;

	// start wait cursor
	WaitCursorBegin();

	HDIB hDib = NULL;
	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDIB);
	if (wBitCount != 24)
	{
		hNewDib = ConvertDIBFormat(hDIB, 24, NULL);
		hDib = CopyHandle(hNewDib);
	}
	else
	{
		hNewDib = CopyHandle(hDIB);
		hDib = CopyHandle(hDIB);
	}

	if (hNewDib == NULL && hDib == NULL)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// process!
	LPBYTE lpSrcDIB = (LPBYTE)GlobalLock(hDib);
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpInput = FindDIBBits(lpSrcDIB);
	LPBYTE lpOutput = FindDIBBits(lpDIB);
	int nWidth = DIBWidth(lpSrcDIB);
	int nHeight = DIBHeight(lpSrcDIB);

	int w=1,h=1,wp=0,hp=0;
	while(w*2<=nWidth)
	{
		w*=2;
		wp++;
	}
	while(h*2<=nHeight)
	{
		h*=2;
		hp++;
	}
	int x,y;
	BYTE *lpPoints=new BYTE[nWidth*nHeight];
	GetPoints(nWidth,nHeight,lpInput,lpPoints);

	double *f=new double[w*h];
	double *W=new double[w*h];

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			f[x+y*w]=Point(x,y);
		}
	}

	for(y=0;y<h;y++)
	{
		DCT(&f[w*y],&W[w*y],wp);
	}
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			f[x*h+y]=W[x+w*y];
		}
	}
	for(x=0;x<w;x++)
	{
		DCT(&f[x*h],&W[x*h],hp);
	}
	double a;
	memset(lpPoints,0,nWidth*nHeight);

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			a=fabs(W[x*h+y]);
			if (a>255) a=255;
			Point(x,nHeight-y-1)=(BYTE)(a);
		}
	}
	delete f;
	delete W;
	PutPoints(nWidth,nHeight,lpOutput,lpPoints);
	delete lpPoints;

	// recover
	DWORD dwSize = GlobalSize(hDib);
	memcpy(lpSrcDIB, lpDIB, dwSize);
	GlobalUnlock(hDib);
	GlobalUnlock(hNewDib);
	if (wBitCount != 24)
	{
		hNewDib = ConvertDIBFormat(hDib, wBitCount, NULL);

		lpSrcDIB = (LPBYTE)GlobalLock(hDIB);
		lpDIB = (LPBYTE)GlobalLock(hNewDib);
		dwSize = GlobalSize(hNewDib);
		memcpy(lpSrcDIB, lpDIB, dwSize);
		GlobalUnlock(hDIB);
		GlobalUnlock(hNewDib);
	}
	else
	{
		lpSrcDIB = (LPBYTE)GlobalLock(hDIB);
		lpDIB = (LPBYTE)GlobalLock(hDib);
		dwSize = GlobalSize(hDib);
		memcpy(lpSrcDIB, lpDIB, dwSize);
		GlobalUnlock(hDIB);
		GlobalUnlock(hDib);
	}

	// cleanup
	GlobalFree(hDib);
	GlobalFree(hNewDib);

	// return
	WaitCursorEnd();
	return TRUE;
}

/****************************************************
	WALhDIB()

	������

		hDIBΪ�����DIB���

	����ֵ��

		�ɹ�ΪTRUE��ʧ��ΪFALSE

	˵����

		������ʵ��DIBλͼ�Ŀ����ֶ�ʲ-������任
****************************************************/
BOOL WALhDIB(HDIB hDIB)
{
	if (hDIB == NULL)
		return FALSE;

	// start wait cursor
	WaitCursorBegin();

	HDIB hDib = NULL;
	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDIB);
	if (wBitCount != 24)
	{
		hNewDib = ConvertDIBFormat(hDIB, 24, NULL);
		hDib = CopyHandle(hNewDib);
	}
	else
	{
		hNewDib = CopyHandle(hDIB);
		hDib = CopyHandle(hDIB);
	}

	if (hNewDib == NULL && hDib == NULL)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// process!
	LPBYTE lpSrcDIB = (LPBYTE)GlobalLock(hDib);
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpInput = FindDIBBits(lpSrcDIB);
	LPBYTE lpOutput = FindDIBBits(lpDIB);
	int nWidth = DIBWidth(lpSrcDIB);
	int nHeight = DIBHeight(lpSrcDIB);

	int w=1,h=1,wp=0,hp=0;
	while(w*2<=nWidth)
	{
		w*=2;
		wp++;
	}
	while(h*2<=nHeight)
	{
		h*=2;
		hp++;
	}
	int x,y;
	BYTE *lpPoints=new BYTE[nWidth*nHeight];
	GetPoints(nWidth,nHeight,lpInput,lpPoints);

	double *f=new double[w*h];
	double *W=new double[w*h];

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			f[x+y*w]=Point(x,y);
		}
	}

	for(y=0;y<h;y++)
	{
		WALh(f+w*y,W+w*y,wp);
	}
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			f[x*h+y]=W[x+w*y];
		}
	}
	for(x=0;x<w;x++)
	{
		WALh(f+x*h,W+x*h,hp);
	}
	double a;
	memset(lpPoints,0,nWidth*nHeight);

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			a=fabs(W[x*h+y]*1000);
			if (a>255) a=255;
			Point(x,nHeight-y-1)=(BYTE)a;
		}
	}
	delete f;
	delete W;
	PutPoints(nWidth,nHeight,lpOutput,lpPoints);
	delete lpPoints;

	// recover
	DWORD dwSize = GlobalSize(hDib);
	memcpy(lpSrcDIB, lpDIB, dwSize);
	GlobalUnlock(hDib);
	GlobalUnlock(hNewDib);
	if (wBitCount != 24)
	{
		hNewDib = ConvertDIBFormat(hDib, wBitCount, NULL);

		lpSrcDIB = (LPBYTE)GlobalLock(hDIB);
		lpDIB = (LPBYTE)GlobalLock(hNewDib);
		dwSize = GlobalSize(hNewDib);
		memcpy(lpSrcDIB, lpDIB, dwSize);
		GlobalUnlock(hDIB);
		GlobalUnlock(hNewDib);
	}
	else
	{
		lpSrcDIB = (LPBYTE)GlobalLock(hDIB);
		lpDIB = (LPBYTE)GlobalLock(hDib);
		dwSize = GlobalSize(hDib);
		memcpy(lpSrcDIB, lpDIB, dwSize);
		GlobalUnlock(hDIB);
		GlobalUnlock(hDib);
	}

	// cleanup
	GlobalFree(hDib);
	GlobalFree(hNewDib);

	// return
	WaitCursorEnd();
	return TRUE;
}

/************************************************************************* 
 * EmbossmentDIB() 
 * 
 * Parameters: 
 *		HDIB hDib        - handle to global memory with a DIB spec 
 *							in it followed by the DIB bits 
 *      nBackground		 - background constant
 * Return Value: 
 *		BOOL				- TRUE is success, else FALSE
 * Description: 
 *		Make DIB embossment
 ************************************************************************/ 
BOOL EmbossmentDIB(HDIB hDib, int nBackground)
{
	WaitCursorBegin();

	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);

	if (! hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);
	if (!hFilteredBits) 
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpDIBits = FindDIBBits(lpDIB);

	// process...
	for (int i=1; i<wDIBHeight-1; i++) 
	{
		for (int j=1; j<wDIBWidth-1; j++) 
		{
			LONG lOffset = PIXEL_OFFSET(i,j, wBytesPerLine);
			BYTE b1 = *(lpDIBits + lOffset++);
			BYTE g1 = *(lpDIBits + lOffset++);
			BYTE r1 = *(lpDIBits + lOffset);

			BYTE r2 = 0;
			BYTE g2 = 0;
			BYTE b2 = 0;
			if (i >= 1 && j >= 1)
			{
				lOffset= PIXEL_OFFSET(i-1,j-1, wBytesPerLine);
				b2 = *(lpDIBits + lOffset++);
				g2 = *(lpDIBits + lOffset++);
				r2 = *(lpDIBits + lOffset);
			}

			BYTE rr = r1 - r2 + nBackground;
			BYTE gg = g1 - g2 + nBackground;
			BYTE bb = b1 - b2 + nBackground;

			lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
			*(lpDestImage + lOffset++) = BOUND(bb, 0, 255);
			*(lpDestImage + lOffset++) = BOUND(gg, 0, 255);
			*(lpDestImage + lOffset)   = BOUND(rr, 0, 255);
		}
	}

	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);
	GlobalUnlock(hNewDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
}

/************************************************************************* 
 * SpreadDIB() 
 * 
 * Parameters: 
 *		HDIB hDib        - handle to global memory with a DIB spec 
 *							in it followed by the DIB bits 
 *      nBlockSize		 - block size to spread
 * Return Value: 
 *		BOOL				- TRUE is success, else FALSE
 * Description: 
 *		Spread DIB
 ************************************************************************/ 
BOOL SpreadDIB(HDIB hDib, int nBlockSize)
{
	WaitCursorBegin();

	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);

	if (! hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);
	if (!hFilteredBits) 
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpDIBits = FindDIBBits(lpDIB);

	// process...
    for (UINT x=nBlockSize/2; x<UINT(wDIBHeight-nBlockSize/2); x++)
	{
	    for (UINT y=nBlockSize/2; y<UINT(wDIBWidth-nBlockSize/2); y++)	
		{
			int nRand1 = rand();
			int nDeltx = nRand1%nBlockSize-nBlockSize/2;
			int nRand2 = rand();
			int nDelty = nRand2%nBlockSize-nBlockSize/2;

			LONG lOffset = PIXEL_OFFSET(x+nDeltx, y+nDelty, wBytesPerLine);
			BYTE b = *(lpDIBits + lOffset++);
			BYTE g = *(lpDIBits + lOffset++);
			BYTE r = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(x,y, wBytesPerLine);
			*(lpDestImage + lOffset++) = b;
			*(lpDestImage + lOffset++) = g;
			*(lpDestImage + lOffset)   = r;
		}
	}

	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);
	GlobalUnlock(hNewDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
}

/************************************************************************* 
 * EdegeDetectDIB() 
 * 
 * Parameters: 
 *		HDIB hDIB        - handle to global memory with a DIB spec 
 *							in it followed by the DIB bits 
 *		KERNEL *lpKernel - kernel used to detect edge
 * Return Value: 
 *		BOOL			 - TRUE is success, else FALSE
 * Description: 
 *		Edege Detect DIB
 ************************************************************************/ 
BOOL EdegeDetectDIB(HDIB hDib, KERNEL *lpKernel) 
{
	WaitCursorBegin();

	// 1. filter noise
	if (! MedianFilterDIB(hDib))
	{
		WaitCursorEnd();
		return FALSE;
	}

	// 2. convert to gray scale
	ConvertToGrayscale(hDib, MEAN_GRAY, 0, 0, 0);
	HDIB hNewDib = CopyHandle(hDib);

	if (! hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);

	// original DIB become dest image
	LPBYTE lpDestDIB = (LPBYTE)GlobalLock(hDib);
	LPBYTE lpDestImage = FindDIBBits(lpDestDIB);

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpDIBits = FindDIBBits(lpDIB);

	// 3. detect...
	int i,j,y,x,ir;
	LONG lOffset;
	BYTE dst;
	double v, g, gx, gy, maxV = 0;
	for (i=1; i<wDIBHeight-1; i++)
	{
		for (j=1; j<wDIBWidth-1; j++) 
		{
			gx = gy = 0;

			BYTE b[9];
			lOffset= PIXEL_OFFSET(i-1,j-1, wBytesPerLine);
			b[0] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i-1,j, wBytesPerLine);
			b[1] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i-1,j+1, wBytesPerLine);
			b[2] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i,j-1, wBytesPerLine);
			b[3] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
			b[4] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i,j+1, wBytesPerLine);
			b[5] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i+1,j-1, wBytesPerLine);
			b[6] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i+1,j, wBytesPerLine);
			b[7] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i+1,j+1, wBytesPerLine);
			b[8] = *(lpDIBits + lOffset);

			for (int k=0; k<=8; ++k)
			{
				gx += lpKernel[0].Element[k]*b[k];
				gy += lpKernel[1].Element[k]*b[k];
			}

			// get gradient value
			g = min(fabs(gx) + fabs(gy), 255.0);	// approximation
			if (g > maxV)
				maxV = g;
			lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
			*(lpDestImage + lOffset) = (BYTE) g;
		}
	}

	double ratio = 255.0/(double)maxV;

	for (i=1; i<wDIBHeight-1; i++)
	{
		for (j=1; j<wDIBWidth-1; j++) 
		{
			LONG lOffset= PIXEL_OFFSET(i, j, wBytesPerLine);
			double value = (double)*(lpDestImage + lOffset) * ratio;
			*(lpDestImage + lOffset) = BYTE(int(value+0.5));
		}
	}

	// cleanup temp buffers
	GlobalUnlock(hNewDib);
	GlobalFree(hNewDib);
	GlobalUnlock(hDib);
	
	WaitCursorEnd();

	return TRUE;
}

/************************************************************************* 
 * EdegeDetectDIB() 
 * 
 * Parameters: 
 *		HDIB hDIB        - handle to global memory with a DIB spec 
 *							in it followed by the DIB bits 
 *		int nAlgorithm   - algorithm used to detect edge
 * Return Value: 
 *		BOOL			 - TRUE is success, else FALSE
 * Description: 
 *		Edege Detect DIB
 ************************************************************************/ 
BOOL EdgeDetectDIB(HDIB hDib, int nAlgorithm)
{
	switch (nAlgorithm)
	{
	case SOBEL:
		return EdegeDetectDIB(hDib, Sobel);
	case HOUGH:
		return EdegeDetectDIB(hDib, Hough);
	}

	return FALSE;
}

/************************************************************************* 
 * HomogenityEdegeDetectDIB() 
 * 
 * Parameters: 
 *		HDIB hDIB        - handle to global memory with a DIB spec 
 *							in it followed by the DIB bits 
 * Return Value: 
 *		BOOL			 - TRUE is success, else FALSE
 * Description: 
 *		Homogenity Edege Detect DIB
 ************************************************************************/ 
BOOL HomogenityEdegeDetectDIB(HDIB hDib) 
{
	WaitCursorBegin();

	// 1. filter noise
	if (! MedianFilterDIB(hDib))
	{
		WaitCursorEnd();
		return FALSE;
	}
	
	// 2. convert to gray scale
	ConvertToGrayscale(hDib, MEAN_GRAY, 0, 0, 0);
	HDIB hNewDib = CopyHandle(hDib);

	if (! hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);

	// original DIB become dest image
	LPBYTE lpDestDIB = (LPBYTE)GlobalLock(hDib);
	LPBYTE lpDestImage = FindDIBBits(lpDestDIB);

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpDIBits = FindDIBBits(lpDIB);

	// 3. detect...
	LONG lOffset;
	BYTE dst;
	int maxV = 0;
	for (int i=1; i<wDIBHeight-1; i++)
	{
		for (int j=1; j<wDIBWidth-1; j++) 
		{
			maxV = 0;

			BYTE b[9];
			lOffset= PIXEL_OFFSET(i-1,j-1, wBytesPerLine);
			b[0] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i-1,j, wBytesPerLine);
			b[1] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i-1,j+1, wBytesPerLine);
			b[2] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i,j-1, wBytesPerLine);
			b[3] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
			b[4] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i,j+1, wBytesPerLine);
			b[5] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i+1,j-1, wBytesPerLine);
			b[6] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i+1,j, wBytesPerLine);
			b[7] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i+1,j+1, wBytesPerLine);
			b[8] = *(lpDIBits + lOffset);

			for (int k=0; k<=8; ++k)
			{
				if (k != 4)
				{
					int d = b[4] - b[k];
					if (d < 0)
						d = -d;
					if (d > maxV)
						maxV = d;
				}
			}

			lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
			*(lpDestImage + lOffset) = (BYTE)maxV;
		}
	}

	// cleanup temp buffers
	GlobalUnlock(hNewDib);
	GlobalFree(hNewDib);
	GlobalUnlock(hDib);
	
	WaitCursorEnd();

	return TRUE;
}

/************************************************************************* 
 * DifferenceEdegeDetectDIB() 
 * 
 * Parameters: 
 *		HDIB hDIB        - handle to global memory with a DIB spec 
 *							in it followed by the DIB bits 
 * Return Value: 
 *		BOOL			 - TRUE is success, else FALSE
 * Description: 
 *		Difference Edege Detect DIB
 ************************************************************************/ 
BOOL DifferenceEdegeDetectDIB(HDIB hDib) 
{
	WaitCursorBegin();

	// 1. filter noise
	if (! MedianFilterDIB(hDib))
	{
		WaitCursorEnd();
		return FALSE;
	}

	// 2. convert to gray scale
	ConvertToGrayscale(hDib, MEAN_GRAY, 0, 0, 0);
	HDIB hNewDib = CopyHandle(hDib);

	if (! hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);

	// original DIB become dest image
	LPBYTE lpDestDIB = (LPBYTE)GlobalLock(hDib);
	LPBYTE lpDestImage = FindDIBBits(lpDestDIB);

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpDIBits = FindDIBBits(lpDIB);

	// 3. detect...
	LONG lOffset;
	BYTE dst;
	int maxV = 0;
	for (int i=1; i<wDIBHeight-1; i++)
	{
		for (int j=1; j<wDIBWidth-1; j++) 
		{
			maxV = 0;

			BYTE b[9];
			lOffset= PIXEL_OFFSET(i-1,j-1, wBytesPerLine);
			b[0] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i-1,j, wBytesPerLine);
			b[1] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i-1,j+1, wBytesPerLine);
			b[2] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i,j-1, wBytesPerLine);
			b[3] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
			b[4] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i,j+1, wBytesPerLine);
			b[5] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i+1,j-1, wBytesPerLine);
			b[6] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i+1,j, wBytesPerLine);
			b[7] = *(lpDIBits + lOffset);

			lOffset= PIXEL_OFFSET(i+1,j+1, wBytesPerLine);
			b[8] = *(lpDIBits + lOffset);

			// left diagonal
			int d = (int) b[0] - b[8];
			if (d < 0)
				d = -d;
			if (d > maxV)
				maxV = d;
			// right diagonal
			d = (int) b[2] - b[6];
			if (d < 0)
				d = -d;
			if (d > maxV)
				maxV = d;
			// vertical
			d = (int) b[1] - b[7];
			if (d < 0)
				d = -d;
			if (d > maxV)
				maxV = d;
			// horizontal
			d = (int) b[3] - b[5];
			if (d < 0)
				d = -d;
			if (d > maxV)
				maxV = d;

			lOffset= PIXEL_OFFSET(i,j, wBytesPerLine);
			*(lpDestImage + lOffset) = (BYTE)maxV;
		}
	}

	// cleanup temp buffers
	GlobalUnlock(hNewDib);
	GlobalFree(hNewDib);
	GlobalUnlock(hDib);
	
	WaitCursorEnd();

	return TRUE;
}

/************************************************************************* 
 * GausianFilterDIB
 * 
 * Parameters: 
 *		HDIB hDIB        - handle to global memory with a DIB spec 
 *							in it followed by the DIB bits 
 * Return Value: 
 *		BOOL			 - TRUE is success, else FALSE
 * Description: 
 *		Process Gausian filtering
 ************************************************************************/ 
BOOL GausianFilterDIB(HDIB hDib)
{
	unsigned char mask[5][5];
	int r, c, i, j;
	int SUM;

	WaitCursorBegin();

	// 5x5 Gaussian mask
	mask[0][0] = 2; mask[0][1] =  4; mask[0][2] =  5; mask[0][3] =  4; mask[0][4] = 2;
	mask[1][0] = 4; mask[1][1] =  9; mask[1][2] = 12; mask[1][3] =  9; mask[1][4] = 4;
	mask[2][0] = 5; mask[2][1] = 12; mask[2][2] = 15; mask[2][3] = 12; mask[2][4] = 5;
	mask[3][0] = 4; mask[3][1] =  9; mask[3][2] = 12; mask[3][3] =  9; mask[3][4] = 4;
	mask[4][0] = 2; mask[4][1] =  4; mask[4][2] =  5; mask[4][3] =  4; mask[4][4] = 2;

	// copy to a new DIB to store original data
	HDIB hNewDib = CopyHandle(hDib);
	if (! hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);

	// original DIB become dest image
	LPBYTE lpDestDIB = (LPBYTE)GlobalLock(hDib);
	LPBYTE lpDestImage = FindDIBBits(lpDestDIB);

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpDIBits = FindDIBBits(lpDIB);

	for(r=0; r<wDIBHeight; r++)
	{
		for(c=0; c<wDIBWidth ; c++)
		{
			SUM=0;

			long lOffset = PIXEL_OFFSET(r,c, wBytesPerLine);

			if(r==0 || r==1 || r==wDIBHeight-2 || r==wDIBHeight-1)
				SUM =  *(lpDIBits + lOffset);
			else if(c==0 || c==1 || c==wDIBWidth-2 || c==wDIBWidth-1)
				SUM =  *(lpDIBits + lOffset);
				 
			// Convolution starts here
			else   
			{
				for(i=-2; i<=2; i++)  
				{
					for(j=-2; j<=2; j++)  
					{
						lOffset = PIXEL_OFFSET(r+i,c+j, wBytesPerLine);
						SUM = SUM + (int)(*(lpDIBits + lOffset) * mask[i+2][j+2]/115);
					}
				}
			}

			if(SUM>255)  
				SUM=255;
			if(SUM<0)    
				SUM=0;

			lOffset = PIXEL_OFFSET(r,c, wBytesPerLine);
			*(lpDestImage + lOffset) = (unsigned char)(SUM);
		}
	}

	// cleanup temp buffers
	GlobalUnlock(hNewDib);
	GlobalFree(hNewDib);
	GlobalUnlock(hDib);
	
	WaitCursorEnd();
	return TRUE;
}


/************************************************************************* 
 * CannyEdegeDetectDIB() 
 * 
 * Parameters: 
 *		HDIB hDIB        - handle to global memory with a DIB spec 
 *							in it followed by the DIB bits 
 * Return Value: 
 *		BOOL			 - TRUE is success, else FALSE
 * Description: 
 *		Difference Edege Detect DIB
 ************************************************************************/ 
BOOL CannyEdegeDetectDIB(HDIB hDib) 
{
	int GX[3][3], GY[3][3];
	int sumX, sumY, SUM;
	int r,c, i, j;
	double ORIENT;
	int	edgeDirection;
	int	highThreshold, lowThreshold;
	int	leftPixel, rightPixel;
	int	P1, P2, P3, P4, P5, P6, P7, P8;
	long lOffset;

	WaitCursorBegin();

	// 1. convert to gray scale
	ConvertToGrayscale(hDib, MEAN_GRAY, 0, 0, 0);

	// 2. Gausian Filter DIB
	if (! GausianFilterDIB(hDib))
	{
		WaitCursorEnd();
		return FALSE;
	}

	// 3. process...

	// 3x3 GX Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html 
	GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
	GX[1][0] = -2; GX[1][1] = 0; GX[1][2] = 2;
	GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;

	// 3x3 GY Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html 
	GY[0][0] =  1; GY[0][1] =  2; GY[0][2] =  1;
	GY[1][0] =  0; GY[1][1] =  0; GY[1][2] =  0;
	GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;

	// copy to a new DIB to store original data
	HDIB hNewDib = CopyHandle(hDib);
	if (! hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);

	// original DIB become dest image
	LPBYTE lpDestDIB = (LPBYTE)GlobalLock(hDib);
	LPBYTE lpDestImage = FindDIBBits(lpDestDIB);

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	LPBYTE lpDIBits = FindDIBBits(lpDIB);

	for(r=0; r<wDIBHeight; r++)
	{
		for(c=0; c<wDIBWidth; c++)
		{
			sumX = 0;
			sumY = 0;

			// image boundaries
			if(r==0 || r==wDIBHeight-1)
				SUM = 0;
			else if(c==0 || c==wDIBWidth-1)
				SUM = 0;

			// Convolution starts here
			else   
			{
				/***********************************
				* X gradient approximation
				***********************************/
				for(i=-1; i<=1; i++)  
				{
					for(j=-1; j<=1; j++)  
					{
						lOffset = PIXEL_OFFSET(r+i,c+j, wBytesPerLine);
						sumX = sumX + (int)(*(lpDIBits + lOffset) * GX[i+1][j+1]);
					}
				}

				/**************************
				* Y gradient approximation
				**************************/
				for(i=-1; i<=1; i++)  
				{
					for(j=-1; j<=1; j++)  
					{
						lOffset = PIXEL_OFFSET(r+i,c+j, wBytesPerLine);
						sumY = sumY + (int)(int)(*(lpDIBits + lOffset) * GY[i+1][j+1]);
					}
				}

				/***********************************************
				* GRADIENT MAGNITUDE APPROXIMATION (Myler p.218)
				***********************************************/
				SUM = abs(sumX) + abs(sumY);

				if(SUM>255)    
					SUM=255;
				if(SUM<0)      
					SUM=0;
			}

			/***************************
			* Magnitude orientation
			***************************/
			/* Cannot divide by zero*/
			if(sumX == 0)   
			{
				if(sumY==0) 
					ORIENT = 0.0;
				else if (sumY<0)   
				{
					sumY = -sumY;

					ORIENT = 90.0;
				}
				else 
					ORIENT = 90.0;
			}

			// Can't take invtan of angle in 2nd Quad
			else if(sumX<0 && sumY>0)   
			{
				sumX = -sumX;
				ORIENT = 180 - ((atan((float)(sumY)/(float)(sumX))) * (180/PI));
			}

			// Can't take invtan of angle in 4th Quad
			else if(sumX>0 && sumY<0)   
			{
				sumY = -sumY;
				ORIENT = 180 - ((atan((float)(sumY)/(float)(sumX))) * (180/PI));
			}

			// else angle is in 1st or 3rd Quad
			else 
				ORIENT = (atan((float)(sumY)/(float)(sumX))) * (180/PI);


			/***************************************************
			* Find edgeDirection by assigning ORIENT a value of
			* either 0, 45, 90 or 135 degrees, depending on which
			* value ORIENT is closest to
			****************************************************/
			if(ORIENT < 22.5) 
				edgeDirection = 0;
			else if(ORIENT < 67.5) 
				edgeDirection = 45;
			else if(ORIENT < 112.5) 
				edgeDirection = 90;
			else if(ORIENT < 157.5) 
				edgeDirection = 135;
			else 
				edgeDirection = 0;

			/***************************************************
			* Obtain values of 2 adjacent pixels in edge
			* direction.
			****************************************************/
			if(edgeDirection == 0)   
			{
				lOffset = PIXEL_OFFSET(r,c-1, wBytesPerLine);
				leftPixel = (int)(*(lpDIBits + lOffset));
				lOffset = PIXEL_OFFSET(r,c+1, wBytesPerLine);
				rightPixel = (int)(*(lpDIBits + lOffset));
			}

			else if(edgeDirection == 45)   
			{
				lOffset = PIXEL_OFFSET(r+1,c-1, wBytesPerLine);
				leftPixel = (int)(*(lpDIBits + lOffset));
				lOffset = PIXEL_OFFSET(r+1,c+1, wBytesPerLine);
				rightPixel = (int)(*(lpDIBits + lOffset));
			}

			else if(edgeDirection == 90)   
			{
				lOffset = PIXEL_OFFSET(r-1,c, wBytesPerLine);
				leftPixel = (int)(*(lpDIBits + lOffset));
				lOffset = PIXEL_OFFSET(r+1,c, wBytesPerLine);
				rightPixel = (int)(*(lpDIBits + lOffset));
			}

			else   
			{
				lOffset = PIXEL_OFFSET(r-1,c-1, wBytesPerLine);
				leftPixel = (int)(*(lpDIBits + lOffset));
				lOffset = PIXEL_OFFSET(r-1,c+1, wBytesPerLine);
				rightPixel = (int)(*(lpDIBits + lOffset));
			}

			/*********************************************
			* Compare current magnitude to both adjacent
			* pixel values.  And if it is less than either
			* of the 2 adjacent values - suppress it and make
			* a nonedge.
			*********************************************/

			if(SUM < leftPixel || SUM < rightPixel) 
				SUM = 0;
			else   
			{
				/**********************
				* Hysteresis
				**********************/
				 highThreshold = 120;
				 lowThreshold = 40;

				 lOffset = PIXEL_OFFSET(r,c, wBytesPerLine);
				 if(SUM >= highThreshold)
					   SUM = *(lpDIBits + lOffset); // edge
				 else if(SUM < lowThreshold)
					 SUM = 0;  // nonedge

				 // SUM is between T1 & T2
				 else   
				 {
					// Determine values of neighboring pixels
					lOffset = PIXEL_OFFSET(r-1,c-1, wBytesPerLine);
					P1 = (int)*(lpDIBits + lOffset);
					lOffset = PIXEL_OFFSET(r-1,c, wBytesPerLine);
					P2 = (int)*(lpDIBits + lOffset);
					lOffset = PIXEL_OFFSET(r-1,c+1, wBytesPerLine);
					P3 = (int)*(lpDIBits + lOffset);
					lOffset = PIXEL_OFFSET(r,c-1, wBytesPerLine);
					P4 = (int)*(lpDIBits + lOffset);
					lOffset = PIXEL_OFFSET(r,c+1, wBytesPerLine);
					P5 = (int)*(lpDIBits + lOffset);
					lOffset = PIXEL_OFFSET(r+1,c-1, wBytesPerLine);
					P6 = (int)*(lpDIBits + lOffset);
					lOffset = PIXEL_OFFSET(r+1,c, wBytesPerLine);
					P7 = (int)*(lpDIBits + lOffset);
					lOffset = PIXEL_OFFSET(r+1,c+1, wBytesPerLine);
					P8 = (int)*(lpDIBits + lOffset);

					// Check to see if neighboring pixel values are edges
					lOffset = PIXEL_OFFSET(r,c, wBytesPerLine);
					if( P1 > highThreshold || P2 > highThreshold || 
						P3 > highThreshold || P4 > highThreshold || 
						P5 > highThreshold || P6 > highThreshold || 
						P7 > highThreshold || P8 > highThreshold)
						SUM = *(lpDIBits + lOffset); // make edge
					else 
						SUM = 0; // make nonedge
				}
			}

			lOffset = PIXEL_OFFSET(r,c, wBytesPerLine);
			*(lpDestImage + lOffset) =  (BYTE)(SUM);
		}
	}

	// cleanup temp buffers
	GlobalUnlock(hNewDib);
	GlobalFree(hNewDib);
	GlobalUnlock(hDib);
	
	WaitCursorEnd();
	return TRUE;
}


//һ��ĵ��������

/******************************************************************************
*	����:			���Ա任����
*	����:
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		slope		���Ժ�����б��
*		inter		���Ժ����Ľؾ�
*	��ע:			�˺������ڲ�ɫͼͬ������
******************************************************************************/
void LineTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height,
	double slope, double inter)
{
	// �Ҷ�ӳ���
	BYTE map[256];

	// ������������ʱֵ
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// ���㵱ǰ���ر任���ֵ
		dTemp = slope * i + inter;

		// ����������޸���ֵ
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// ��������
		map[i] = int(dTemp + 0.5);
	}

	// ���Ա任���ֱֵ����ӳ����в���
	for (UINT i = 0; i < width * height; i++)
	{
		for (int j = 0; j < 4; j++)
			tempPixel[i * 4 + j] = map[pixel[i * 4 + j]];
	}
}

/******************************************************************************
*	����:			�����任����
*	����:
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		a			���Ʋ���,��ʾ���ߵ�����ƫ����
*		b			���Ʋ���,��ʾ���ߵ������̶�
*	��ע:			�˺������ڲ�ɫͼͬ������
******************************************************************************/
void LogTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, WORD wBytesPerLine,
	double a, double b)
{
	// ӳ���,����256�ֻҶȱ任���ֵ
	BYTE map[256];

	// ������������ʱֵ
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// ���㵱ǰ���ر任���ֵ
		dTemp = log((double)i + 1.0) / b + a;

		// ����������޸���ֵ
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// ��������
		map[i] = int(dTemp + 0.5);
	}

	LONG lOffset;
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			lOffset = PIXEL_OFFSET(i, j, wBytesPerLine);// ����Ŀ��ͼ��ǰ����ƫ����
			*(tempPixel + lOffset++) = map[*(pixel + lOffset)];
			*(tempPixel + lOffset++) = map[*(pixel + lOffset)];
			*(tempPixel + lOffset) = map[*(pixel + lOffset)];
		}
	//for (UINT i = 0; i < width * height; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//		tempPixel[i * 4 + j] = map[pixel[i * 4 + j]];
	//}
}

/******************************************************************************
*	����:			�ݴα任����
*	����:
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		b			���Ʋ���,��ʾ���ߵ�����ƫ����
*		c			���Ʋ���,��ʾ���ߵ������̶�
*		r			���Ʋ���,��ʾ�������ݴ�
*	��ע:			�˺������ڲ�ɫͼͬ������
******************************************************************************/
void PowerTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height,
	double b, double c, double r)
{
	// ӳ���,����256�ֻҶȱ任���ֵ
	BYTE map[256];

	// ������������ʱֵ
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// ���㵱ǰ���ر任���ֵ
		dTemp = c * pow(i / 255.0, r) * 255 + b;

		// ����������޸���ֵ
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// ��������
		map[i] = int(dTemp + 0.5);
	}

	for (UINT i = 0; i < width * height; i++)
	{
		for (int j = 0; j < 4; j++)
			tempPixel[i * 4 + j] = map[pixel[i * 4 + j]];
	}
}

/******************************************************************************
*	����:			ָ���任����
*	����:
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		a			���Ʋ���,��ʾ���ߵ�����ƫ����
*		b,c			���Ʋ���,��ʾ���ߵ������̶�
*	��ע:			�˺������ڲ�ɫͼͬ������
******************************************************************************/
void ExpTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height,
	double a, double b, double c)
{
	// ӳ���,����256�ֻҶȱ任���ֵ
	BYTE map[256];

	// ������������ʱֵ
	double dTemp;

	for (int i = 0; i < 256; i++)
	{
		// ���㵱ǰ���ر任���ֵ
		dTemp = pow(b, c * (i - a)) - 1;

		// ����������޸���ֵ
		if (dTemp < 0)
			dTemp = 0.0;
		else if (dTemp > 255)
			dTemp = 255;

		// ��������
		map[i] = int(dTemp + 0.5);
	}

	for (UINT i = 0; i < width * height; i++)
	{
		for (int j = 0; j < 4; j++)
			tempPixel[i * 4 + j] = map[pixel[i * 4 + j]];
	}
}


/******************************************************************************
*	����:			�Ҷ���ֵ�任����
*	����:
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		height		ԭʼͼ��߶�
*		thr			��ֵ
******************************************************************************/
void ThresholdTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, int thr)
{
	// �Ҷ�ӳ���
	BYTE map[256];

	for (int i = 0; i < 256; i++)
	{
		if (i >= thr)
			map[i] = 255;
		else
			map[i] = 0;
	}

	// �任���ֱֵ����ӳ����в���
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];

		tempPixel[i * 4] = tempPixel[i * 4 + 1] = tempPixel[i * 4 + 2] = map[x];
		tempPixel[i * 4 + 3] = 255;
	}
}

/******************************************************************************
*	����:			�Ҷ����캯��
*	����:
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
*		X1,Y1		���Ƶ�1������
*		X2,Y2		���Ƶ�2������
******************************************************************************/
void GrayStretch(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, WORD wBytesPerLine,
	float X1, float Y1, float X2, float Y2)
{
	// ӳ���,����256�ֻҶȱ任���ֵ
	BYTE map[256];

	// ������������ʱֵ
	double dTemp;

	for (int x = 0; x < 256; x++)
	{
		// ����ڵ�һ��ֱ����
		if (x < X1)
		{
			if (fabs(X1 - 0) > 0.0001)
				dTemp = (float)Y1 / (float)X1 * x;
			else
				dTemp = 0;
		}
		// ����ڵڶ���ֱ����
		else if (x <= X2)
		{
			if (fabs(X2 - X1) > 0.0001)
				dTemp = (float)(Y2 - Y1) / (float)(X2 - X1) * (x - X1) + Y1;
			else
				dTemp = x;
		}
		// ����ڵ�����ֱ����
		else
		{
			if (fabs(Y2 - 255) > 0.0001)
				dTemp = (float)(255 - Y2) / (float)(255 - X2) * (x - X2) + Y2;
			else
				dTemp = 255;
		}

		// ��������
		map[x] = int(dTemp + 0.5);
	}

	// �任���ֱֵ����ӳ����в���
	LONG lOffset;
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			lOffset = PIXEL_OFFSET(i, j, wBytesPerLine);// ����Ŀ��ͼ��ǰ����ƫ����
			int x = pixel[lOffset];
			*(tempPixel + lOffset++) = map[x];
			*(tempPixel + lOffset++) = map[x];
			*(tempPixel + lOffset) = map[x];
		}


}

/******************************************************************************
*	����:			�ҶȾ��⺯��
*	����:
*		pixel		ԭʼ��������
*		tempPixel	����任��ͼ�����������
*		width		ԭʼͼ����
******************************************************************************/
void GrayEqualize(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height)
{

	// �Ҷ�ӳ���
	BYTE map[256];
	long lCounts[256];

	memset(lCounts, 0, sizeof(long) * 256);

	// ������Ҷ�ֵ����
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];
		lCounts[x]++;
	}

	// ���������е���ʱֵ
	long lTemp;

	for (int i = 0; i < 256; i++)
	{
		lTemp = 0;
		for (int j = 0; j <= i; j++)
			lTemp += lCounts[j];

		map[i] = (BYTE)(lTemp * 255.0f / width / height);
	}

	// �任���ֱֵ����ӳ����в���
	for (UINT i = 0; i < width * height; i++)
	{
		int x = pixel[i * 4];

		tempPixel[i * 4] = tempPixel[i * 4 + 1] = tempPixel[i * 4 + 2] = map[x];
		tempPixel[i * 4 + 3] = 255;
	}
}



/*
   ����ڲ�ֵ�Ŵ�9��
*/
BOOL NearestInterpolationDIB(HDIB hDib, int Strength)
{
	double sy = 3, sx = 3;

	//!1 �п�
	if (hDib == NULL)
		return FALSE;

	// start wait cursor
	WaitCursorBegin();

	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);//��������Դʵ��

	if (!hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	HDIB hDib24 = CopyHandle(hNewDib);
	if (!hDib24)
	{
		WaitCursorEnd();
		return FALSE;
	}

	//! 2  process!
	 //! Source DIB attributes
	WORD wSrcDIBWidth = (WORD)DIBWidth(hDib24);
	WORD wSrcDIBHeight = (WORD)DIBHeight(hDib24);
	WORD wSrcBytesPerLine = (WORD)BytesPerLine(hDib24);
	DWORD dwSrcImageSize = wSrcBytesPerLine * wSrcDIBHeight;

	//! ��ͼ�������ű�
	HDIB hTmpDib = ChangeDIBCanvasSize(hNewDib, (int)(wSrcDIBWidth * sy), (int)(wSrcDIBHeight * sx), LEFT_UP);
	GlobalFree(hNewDib);
	hNewDib = CopyHandle(hTmpDib);
	GlobalFree(hTmpDib);
	if (!hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);//����Ŀ�껭���ڴ�
	if (!hFilteredBits)
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);// ָ����ʱ��Ŀ��ͼ��λ�ڴ�

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);// ָ��Ŀ��ʵ���ڴ�
	LPBYTE lpDIBits = FindDIBBits(lpDIB);// ָ��Ŀ��ʵ��λ��ַ
	LPBYTE lpSrcDIBits = FindDIBBits((LPBYTE)GlobalLock(hDib24));// ָ��Դʵ��λ��ַ

	//! 3
	//! ��Ҫ������.
	LONG lOffset;
	BYTE b, g, r;
	for (int i = 0; i < wDIBHeight; i++)
		for (int j = 0; j < wDIBWidth; j++)
		{
			lOffset = PIXEL_OFFSET(i/sy, j/sx, wSrcBytesPerLine);// ����Դͼ��ǰ����ƫ����
			b = *(lpSrcDIBits + lOffset++);
			g = *(lpSrcDIBits + lOffset++);
			r = *(lpSrcDIBits + lOffset);

			lOffset = PIXEL_OFFSET(i, j, wBytesPerLine);// ����Ŀ��ͼ��ǰ����ƫ����
			*(lpDestImage + lOffset++) = BOUND(b, 0, 255);
			*(lpDestImage + lOffset++) = BOUND(g, 0, 255);
			*(lpDestImage + lOffset) = BOUND(r, 0, 255);
		}


	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);
	GlobalUnlock(hDib24);
	GlobalFree(hDib24);
	GlobalUnlock(hNewDib);

	//! ԭͼ�Ŵ�9��
	GlobalUnlock(hDib);
	hTmpDib = ChangeDIBCanvasSize(hDib, (int)(wSrcDIBWidth * sy), (int)(wSrcDIBHeight * sx), LEFT_UP);
	GlobalFree(hDib);
	hDib = CopyHandle(hTmpDib);
	GlobalFree(hTmpDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
}

/*
	˫���Բ�ֵ�Ŵ�9��
*/
BOOL BilinearInterpolationDIB(HDIB hDib)
{
	double sy = 3, sx = 3;

	//!1 �п�
	if (hDib == NULL)
		return FALSE;

	// start wait cursor
	WaitCursorBegin();

	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);//��������Դʵ��

	if (!hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	HDIB hDib24 = CopyHandle(hNewDib);
	if (!hDib24)
	{
		WaitCursorEnd();
		return FALSE;
	}

	//! 2  process!
	//! Source DIB attributes
	WORD wSrcDIBWidth = (WORD)DIBWidth(hDib24);
	WORD wSrcDIBHeight = (WORD)DIBHeight(hDib24);
	WORD wSrcBytesPerLine = (WORD)BytesPerLine(hDib24);
	DWORD dwSrcImageSize = wSrcBytesPerLine * wSrcDIBHeight;

	//! ��ͼ�������ű�
	HDIB hTmpDib = ChangeDIBCanvasSize(hNewDib, (int)(wSrcDIBWidth * sy), (int)(wSrcDIBHeight * sx), LEFT_UP);
	GlobalFree(hNewDib);
	hNewDib = CopyHandle(hTmpDib);
	GlobalFree(hTmpDib);
	if (!hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);//����Ŀ�껭���ڴ�
	if (!hFilteredBits)
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);// ָ����ʱ��Ŀ��ͼ��λ�ڴ�

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);// ָ��Ŀ��ʵ���ڴ�
	LPBYTE lpDIBits = FindDIBBits(lpDIB);// ָ��Ŀ��ʵ��λ��ַ
	LPBYTE lpSrcDIBits = FindDIBBits((LPBYTE)GlobalLock(hDib24));// ָ��Դʵ��λ��ַ

	 //! 3
	 //! ��Ҫ������.
	double distance_to_a_x,  distance_to_a_y, dSrcHeightY, dSrcWidthX;
	int  iSrcHeightY, iSrcWidthX;
	BYTE b, g, r;
	LONG lOffsetA, lOffsetB, lOffsetC, lOffsetD, lOffset;
	for (int i = 0; i < wDIBHeight; i++)
		for (int j = 0; j < wDIBWidth; j++)
		{
			dSrcHeightY = (i + 0.5) / sy - 0.5;// ��Ӧ��ԭͼ�������С��
			dSrcWidthX = (j + 0.5) / sx - 0.5;
			iSrcHeightY = (int)dSrcHeightY;// ��Ӧ��ԭͼ������
			iSrcWidthX = (int)dSrcWidthX;
			distance_to_a_x = dSrcWidthX - iSrcWidthX;//��ԭͼ������a���ˮƽ����    
			distance_to_a_y = dSrcHeightY - iSrcHeightY;//��ԭͼ������a��Ĵ�ֱ����    

			lOffsetA = PIXEL_OFFSET(iSrcHeightY, iSrcWidthX, wSrcBytesPerLine);//ͼ���Aƫ���� 
			lOffsetB = PIXEL_OFFSET(iSrcHeightY, iSrcWidthX + 1, wSrcBytesPerLine);//ͼ���Bƫ���� 
			lOffsetC = PIXEL_OFFSET(iSrcHeightY + 1, iSrcWidthX, wSrcBytesPerLine);//ͼ���Cƫ���� 
			lOffsetD = PIXEL_OFFSET(iSrcHeightY + 1, iSrcWidthX + 1, wSrcBytesPerLine);//ͼ���Dƫ����  

			// PS. BOUND(x, mn, mx)	((x) < (mn) ? (mn) : ((x) > (mx) ? (mx) : (x))) ȡ��ֵ
			b = (BYTE)BOUND(
				*(lpSrcDIBits + lOffsetA) * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				*(lpSrcDIBits + lOffsetB) * distance_to_a_x*(1 - distance_to_a_y) +
				*(lpSrcDIBits + lOffsetC) * distance_to_a_y*(1 - distance_to_a_x) +
				*(lpSrcDIBits + lOffsetD) * distance_to_a_y*distance_to_a_x,
				0, 255);
			g = (BYTE)BOUND(
				*(lpSrcDIBits + lOffsetA + 1) * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				*(lpSrcDIBits + lOffsetB + 1) * distance_to_a_x*(1 - distance_to_a_y) +
				*(lpSrcDIBits + lOffsetC + 1) * distance_to_a_y*(1 - distance_to_a_x) +
				*(lpSrcDIBits + lOffsetD + 1) * distance_to_a_y*distance_to_a_x,
				0, 255);
			r = (BYTE)BOUND(
				*(lpSrcDIBits + lOffsetA + 2) * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				*(lpSrcDIBits + lOffsetB + 2) * distance_to_a_x*(1 - distance_to_a_y) +
				*(lpSrcDIBits + lOffsetC + 2) * distance_to_a_y*(1 - distance_to_a_x) +
				*(lpSrcDIBits + lOffsetD + 2) * distance_to_a_y*distance_to_a_x,
				0, 255);

			lOffset = PIXEL_OFFSET(i, j, wBytesPerLine);// ����Ŀ��ͼ��ǰ����ƫ����
			*(lpDestImage + lOffset++) = b;
			*(lpDestImage + lOffset++) = g;
			*(lpDestImage + lOffset) = r;
		}


	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);	
	GlobalUnlock(hDib24);
	GlobalFree(hDib24);
	GlobalUnlock(hNewDib);

	//! ԭͼ�Ŵ�9��
	GlobalUnlock(hDib);
	hTmpDib = ChangeDIBCanvasSize(hDib, (int)(wSrcDIBWidth * sy), (int)(wSrcDIBHeight * sx), LEFT_UP);
	GlobalFree(hDib);
	hDib = CopyHandle(hTmpDib);
	GlobalFree(hTmpDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
}

/*
˫���β�ֵ����
https://wenku.baidu.com/view/5e889b1755270722192ef78b.html
���� �� 1-2*Abs(x)^2+Abs(x)^3���������� , 0<=Abs(x)<1
   S(x)=| 4-8*Abs(x)+5*Abs(x)^2-Abs(x)^3��, 1<=Abs(x)<2
   ���� �� 0������������������������������ , Abs(x)>=2
*/
static double BiCubicInterpolator(double x)
{
	//coefficient is set to -0.5.
	//if (x < 0) x = -x;//ȡ����ֵ
	//if (x <= 1) return (1.5 * x - 2.5) * x * x + 1;
	//else if (x < 2) return ((-0.5 * x + 2.5) * x - 4) * x + 2;
	//else return 0.0;

	////coefficient is set to -1.
	//if (x < 0) x = -x;//ȡ����ֵ
	//if (x <= 1) return (1.0 * x - 2.0) * x * x + 1;//1  -2  1
	//else if (x < 2) return ((-1 * x + 5) * x - 8) * x + 4;//-1  5  -8  4
	//else return 0.0;

	//coefficient is set to A2 = -7/12 , A3 = 1/12.
	if (x < 0) x = -x;//ȡ����ֵ
	if (x <= 1) return (4.0 * x - 7.0) * x * x / 3.0 + 1;
	else if (x < 2) return ((-7.0 * x / 12.0 + 3.0) * x - 59.0 / 12.0) * x + 15.0 / 6.0;
	else if (x < 3) return ((x / 12.0 - 2/3.0) * x + 21.0 / 12.0) * x - 3.0 / 2.0;
	else return 0.0;
}
/*
	˫���β�ֵ�Ŵ�9��
	����f(i+u,j+v) = [A] * [B] * [C]
[A]=[ S(u + 1)��S(u + 0)��S(u - 1)��S(u - 2) ]
������ f(i-1, j-1)��f(i-1, j+0)��f(i-1, j+1)��f(i-1, j+2) ��
[B]= �� f(i+0, j-1)��f(i+0, j+0)��f(i+0, j+1)��f(i+0, j+2) ��
������ f(i+1, j-1)��f(i+1, j+0)��f(i+1, j+1)��f(i+1, j+2) ��
������ f(i+2, j-1)��f(i+2, j+0)��f(i+2, j+1)��f(i+2, j+2) ��
������ S(v + 1) ��
[C]= �� S(v + 0) ��
������ S(v - 1) ��
������ S(v - 2) ��
���� �� 1-2*Abs(x)^2+Abs(x)^3���������� , 0<=Abs(x)<1
S(x)=�� 4-8*Abs(x)+5*Abs(x)^2-Abs(x)^3��, 1<=Abs(x)<2
���� �� 0������������������������������ , Abs(x)>=2
S(x)�Ƕ� Sin(x*Pi)/x �ıƽ���Pi��Բ���ʡ����У�
*/
BOOL BicubicInterpolationDIB(HDIB hDib)
{
	double sy = 3, sx = 3;//�Ŵ�ϵ��

	//!1 �п�
	if (hDib == NULL)
		return FALSE;

	// start wait cursor
	WaitCursorBegin();

	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);//��������Դʵ��

	if (!hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	HDIB hDib24 = CopyHandle(hNewDib);
	if (!hDib24)
	{
		WaitCursorEnd();
		return FALSE;
	}

	//! 2  process!
	//! Source DIB attributes
	WORD wSrcDIBWidth = (WORD)DIBWidth(hDib24);
	WORD wSrcDIBHeight = (WORD)DIBHeight(hDib24);
	WORD wSrcBytesPerLine = (WORD)BytesPerLine(hDib24);
	DWORD dwSrcImageSize = wSrcBytesPerLine * wSrcDIBHeight;

	//! ��ͼ�������ű�
	HDIB hTmpDib = ChangeDIBCanvasSize(hNewDib, (int)(wSrcDIBWidth * sy), (int)(wSrcDIBHeight * sx), LEFT_UP);
	GlobalFree(hNewDib);
	hNewDib = CopyHandle(hTmpDib);
	GlobalFree(hTmpDib);
	if (!hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);//����Ŀ�껭���ڴ�
	if (!hFilteredBits)
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);// ָ����ʱ��Ŀ��ͼ��λ�ڴ�

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);// ָ��Ŀ��ʵ���ڴ�
	LPBYTE lpDIBits = FindDIBBits(lpDIB);// ָ��Ŀ��ʵ��λ��ַ
	LPBYTE lpSrcDIBits = FindDIBBits((LPBYTE)GlobalLock(hDib24));// ָ��Դʵ��λ��ַ

	//! 3
	 //! ��Ҫ������.
	double dx, dy, dSrcHeightY, dSrcWidthX, d_kx, d_ky;
	int  iSrcHeightY, iSrcWidthX, iSrcHeightY1, iSrcWidthX1;
	BYTE b, g, r;
	LONG lOffsetA, lOffsetB, lOffsetC, lOffsetD, lOffset;
	bool bIntegerY, bIntegerX;//ˮƽWidth x j    ��ֱHeight y i
	for (int i = 0; i < wDIBHeight; i++)
	{
		dSrcHeightY = (i + 0.5) / sy - 0.5;
		iSrcHeightY = (int)dSrcHeightY;// ԭͼ��y����
		dy = dSrcHeightY - iSrcHeightY;// ��ֱ����							
		if (dy < 1e-6 && dy > -1e-6) // ӳ�䵽ԭͼy�����Ƿ�Ϊ����
			bIntegerY = true;
		else
			bIntegerY = false;

		for (int j = 0; j < wDIBWidth; j++)
		{
			dSrcWidthX = (j + 0.5) / sx - 0.5;
			iSrcWidthX = (int)dSrcWidthX;// ԭͼ��x����
			dx = dSrcWidthX - iSrcWidthX;// ˮƽ����
			if (dx < 1e-6 && dx > -1e-6) // ӳ�䵽ԭͼx�����Ƿ�Ϊ����
				bIntegerX = true;
			else
				bIntegerX = false;

			// x��y���궼������
			if (bIntegerX && bIntegerY)
			{
				lOffset = PIXEL_OFFSET(i, j, wBytesPerLine);// ����Ŀ��ͼ��ǰ����ƫ����
				lOffsetA = PIXEL_OFFSET(iSrcHeightY, iSrcWidthX, wSrcBytesPerLine);//ԭͼ���Ӧ��ƫ���� 

				b = (BYTE)BOUND(*(lpSrcDIBits + lOffsetA), 0, 255);
				g = (BYTE)BOUND(*(lpSrcDIBits + lOffsetA + 1), 0, 255);
				r = (BYTE)BOUND(*(lpSrcDIBits + lOffsetA + 2), 0, 255);

				*(lpDestImage + lOffset++) = b;
				*(lpDestImage + lOffset++) = g;
				*(lpDestImage + lOffset) = r;
				continue;
			}

			b = 0;
			g = 0;
			r = 0;

			// y����������
			if (bIntegerY)
			{
				for (int m = -1; m < 3; m++)
				{
					// Xϵ��
					d_kx = BiCubicInterpolator((double)m - dx);

					iSrcWidthX1 = iSrcWidthX + m;
					if (iSrcWidthX1 < 0)
						iSrcWidthX1 = 0;
					if (iSrcWidthX1 > wSrcDIBWidth - 1)
						iSrcWidthX1 = wSrcDIBWidth - 1;

					lOffsetA = PIXEL_OFFSET(iSrcHeightY, iSrcWidthX1, wSrcBytesPerLine);// ����ԭͼ��ǰ����ƫ����
					b += d_kx * (*(lpSrcDIBits + lOffsetA));
					g += d_kx * (*(lpSrcDIBits + lOffsetA + 1));
					r += d_kx * (*(lpSrcDIBits + lOffsetA + 2));
				}
				lOffset = PIXEL_OFFSET(i, j, wBytesPerLine);// ����Ŀ��ͼ��ǰ����ƫ����
				*(lpDestImage + lOffset++) = (BYTE)BOUND(b, 0, 255);
				*(lpDestImage + lOffset++) = (BYTE)BOUND(g, 0, 255);
				*(lpDestImage + lOffset) = (BYTE)BOUND(r, 0, 255);
				continue;
			}

			// x����������
			if (bIntegerX)
			{
				for (int n = -1; n < 3; n++)
				{
					// Yϵ��
					d_ky = BiCubicInterpolator(dy - (double)n);
					iSrcHeightY1 = iSrcHeightY + n;
					if (iSrcHeightY1 < 0)
						iSrcHeightY1 = 0;
					if (iSrcHeightY1 > wSrcDIBHeight - 1)
						iSrcHeightY1 = wSrcDIBHeight - 1;

					lOffsetA = PIXEL_OFFSET(iSrcHeightY1, iSrcWidthX, wSrcBytesPerLine);// ����ԭͼ��ǰ����ƫ����
					b += d_ky * (*(lpSrcDIBits + lOffsetA));
					g += d_ky * (*(lpSrcDIBits + lOffsetA + 1));
					r += d_ky* (*(lpSrcDIBits + lOffsetA + 2));
				}
				lOffset = PIXEL_OFFSET(i, j, wBytesPerLine);// ����Ŀ��ͼ��ǰ����ƫ����
				*(lpDestImage + lOffset++) = (BYTE)BOUND(b, 0, 255);
				*(lpDestImage + lOffset++) = (BYTE)BOUND(g, 0, 255);
				*(lpDestImage + lOffset) = (BYTE)BOUND(r, 0, 255);
				continue;
			}

			// ���ź�ӳ�䵽ԭͼ�ϵ����ص�x,y���궼��Ϊ����
			for (int n = -1; n < 3; n++)
			{
				// Yϵ��
				d_ky = BiCubicInterpolator(dy - (double)n);

				iSrcHeightY1 = iSrcHeightY + n;
				if (iSrcHeightY1 < 0)
					iSrcHeightY1 = 0;
				if (iSrcHeightY1 > wSrcDIBHeight - 1)
					iSrcHeightY1 = wSrcDIBHeight - 1;

				for (int m = -1; m < 3; m++)
				{
					// Xϵ��
					d_kx = d_ky * BiCubicInterpolator((double)m - dx);

					iSrcWidthX1 = iSrcWidthX + m;
					if (iSrcWidthX1 < 0)
						iSrcWidthX1 = 0;
					if (iSrcWidthX1 > wSrcDIBWidth - 1)
						iSrcWidthX1 = wSrcDIBWidth - 1;

					lOffsetA = PIXEL_OFFSET(iSrcHeightY1, iSrcWidthX1, wSrcBytesPerLine);// ����ԭͼ��ǰ����ƫ����
					b += d_kx * (*(lpSrcDIBits + lOffsetA));
					g += d_kx * (*(lpSrcDIBits + lOffsetA + 1));
					r += d_kx* (*(lpSrcDIBits + lOffsetA + 2));
				}
			}
			lOffset = PIXEL_OFFSET(i, j, wBytesPerLine);// ����Ŀ��ͼ��ǰ����ƫ����
			*(lpDestImage + lOffset++) = (BYTE)BOUND(b, 0, 255);
			*(lpDestImage + lOffset++) = (BYTE)BOUND(g, 0, 255);
			*(lpDestImage + lOffset) = (BYTE)BOUND(r, 0, 255);
		
		}
	}

	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);
	GlobalUnlock(hDib24);
	GlobalFree(hDib24);
	GlobalUnlock(hNewDib);

	//! ԭͼ�Ŵ�9��
	GlobalUnlock(hDib);
	hTmpDib = ChangeDIBCanvasSize(hDib, (int)(wSrcDIBWidth * sy), (int)(wSrcDIBHeight * sx), LEFT_UP);
	GlobalFree(hDib);
	hDib = CopyHandle(hTmpDib);
	GlobalFree(hTmpDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
} 

BOOL Mt4DIB(HDIB hDib)
{
	// Check for valid DIB handle 
	if (hDib == NULL)
		return FALSE;

	// start wait cursor
	WaitCursorBegin();

	HDIB hNewDib = NULL;
	// we only convolute 24bpp DIB, so first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);//��������Դʵ��

	if (!hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	//! 2  process!
	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);//������ʱ��Ŀ�껭���ڴ�
	if (!hFilteredBits)
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);// ָ����ʱ��Ŀ��ͼ��λ�ڴ�

	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);// ָ��Ŀ��ʵ���ڴ�
	LPBYTE lpDIBits = FindDIBBits(lpDIB);// ָ��Ŀ��ʵ���ֽڵ�ַ

	//! 3
	//! ��Ҫ������.
	double dSrcHeightY, dSrcWidthX;
	int  iSrcHeightY, iSrcWidthX;
	BYTE b, g, r;
	LONG lOffsetA, lOffsetB, lOffsetC, lOffsetD, lOffset;
	for (int i = 0; i < wDIBHeight; i++)
		for (int j = 0; j < wDIBWidth; j++)
		{/*
			lOffsetA = PIXEL_OFFSET(i, j, wBytesPerLine);//ͼ���Aƫ���� 
			lOffsetB = PIXEL_OFFSET(i, j + 1, wBytesPerLine);//ͼ���Bƫ���� 
			lOffsetC = PIXEL_OFFSET(i + 1, j, wBytesPerLine);//ͼ���Cƫ���� 
			lOffsetD = PIXEL_OFFSET(i + 1, j + 1, wBytesPerLine);//ͼ���Dƫ����  

			 // PS. BOUND(x, mn, mx)	((x) < (mn) ? (mn) : ((x) > (mx) ? (mx) : (x))) ȡ��ֵ
			b = (BYTE)BOUND(
				*(lpDIBits + lOffsetA) * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				*(lpDIBits + lOffsetB) * distance_to_a_x*(1 - distance_to_a_y) +
				*(lpDIBits + lOffsetC) * distance_to_a_y*(1 - distance_to_a_x) +
				*(lpDIBits + lOffsetD) * distance_to_a_y*distance_to_a_x,
				0, 255);
			g = (BYTE)BOUND(
				*(lpSrcDIBits + lOffsetA + 1) * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				*(lpSrcDIBits + lOffsetB + 1) * distance_to_a_x*(1 - distance_to_a_y) +
				*(lpSrcDIBits + lOffsetC + 1) * distance_to_a_y*(1 - distance_to_a_x) +
				*(lpSrcDIBits + lOffsetD + 1) * distance_to_a_y*distance_to_a_x,
				0, 255);
			r = (BYTE)BOUND(
				*(lpSrcDIBits + lOffsetA + 2) * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				*(lpSrcDIBits + lOffsetB + 2) * distance_to_a_x*(1 - distance_to_a_y) +
				*(lpSrcDIBits + lOffsetC + 2) * distance_to_a_y*(1 - distance_to_a_x) +
				*(lpSrcDIBits + lOffsetD + 2) * distance_to_a_y*distance_to_a_x,
				0, 255);

			lOffset = PIXEL_OFFSET(i, j, wBytesPerLine);// ����Ŀ��ͼ��ǰ����ƫ����
			*(lpDestImage + lOffset++) = b;
			*(lpDestImage + lOffset++) = g;
			*(lpDestImage + lOffset) = r;*/
		}


	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);
	GlobalUnlock(hNewDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
}

BOOL ImageNegativesDIB(HDIB hDib)
{
	// Check for valid DIB handle 
	if (hDib == NULL)
		return FALSE;

	// start wait cursor
	WaitCursorBegin();

	HDIB hNewDib = NULL;
	// first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);

	if (!hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// process!
	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);//������ʱ��Ŀ�껭���ڴ�
	if (!hFilteredBits)
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);// ָ����ʱ��Ŀ��ͼ�������ڴ�
	// get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);// ָ��Ŀ��ʵ���ڴ�
	LPBYTE lpDIBBits = FindDIBBits(lpDIB);// ָ��Ŀ��ʵ����������

	 //! ��Ҫ������.
	LONG lOffset;
	for (int i = 0; i < wDIBHeight; i++)
		for (int j = 0; j < wDIBWidth; j++)
		{
			lOffset = PIXEL_OFFSET(i, j, wBytesPerLine);// ����Ŀ��ͼ��ǰ����ƫ����
			*(lpDestImage + lOffset++) = ~*(lpDIBBits + lOffset);
			*(lpDestImage + lOffset++) = ~*(lpDIBBits + lOffset);
			*(lpDestImage + lOffset) = ~*(lpDIBBits + lOffset);
		}

	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);
	GlobalUnlock(hNewDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
}

BOOL LogTransformationDIB(HDIB hDib, int iA, int iB)
{
	// Check for valid DIB handle 
	if (hDib == NULL)
		return FALSE;

	// start wait cursor
	WaitCursorBegin();

	HDIB hNewDib = NULL;
	// first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);

	if (!hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// process!
	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);//������ʱ��Ŀ�껭���ڴ�
	if (!hFilteredBits)
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);// ָ����ʱ��Ŀ��ͼ�������ڴ�

	 // get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);// ָ��Ŀ��ʵ���ڴ�
	LPBYTE lpDIBBits = FindDIBBits(lpDIB);// ָ��Ŀ��ʵ����������

	 //! ��Ҫ������.
	LogTrans(lpDIBBits, lpDestImage,
		wDIBWidth, wDIBHeight, wBytesPerLine, iA, iB * 0.001);


	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);
	GlobalUnlock(hNewDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
}

BOOL ContrastStretchingDIB(HDIB hDib, float X1, float Y1, float X2, float Y2)
{
	// Check for valid DIB handle 
	if (hDib == NULL)
		return FALSE;

	// start wait cursor
	WaitCursorBegin();

	HDIB hNewDib = NULL;
	// first convert DIB to 24bpp
	WORD wBitCount = DIBBitCount(hDib);
	if (wBitCount != 24)
		hNewDib = ConvertDIBFormat(hDib, 24, NULL);
	else
		hNewDib = CopyHandle(hDib);

	if (!hNewDib)
	{
		WaitCursorEnd();
		return FALSE;
	}

	// process!
	// new DIB attributes
	WORD wDIBWidth = (WORD)DIBWidth(hNewDib);
	WORD wDIBHeight = (WORD)DIBHeight(hNewDib);
	WORD wBytesPerLine = (WORD)BytesPerLine(hNewDib);
	DWORD dwImageSize = wBytesPerLine * wDIBHeight;

	// Allocate and lock memory for filtered image data
	HGLOBAL hFilteredBits = GlobalAlloc(GHND, dwImageSize);//������ʱ��Ŀ�껭���ڴ�
	if (!hFilteredBits)
	{
		WaitCursorEnd();
		return FALSE;
	}
	LPBYTE lpDestImage = (LPBYTE)GlobalLock(hFilteredBits);// ָ����ʱ��Ŀ��ͼ�������ڴ�

	 // get bits address in DIB
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);// ָ��Ŀ��ʵ���ڴ�
	LPBYTE lpDIBBits = FindDIBBits(lpDIB);// ָ��Ŀ��ʵ����������

	//! ��Ҫ������.
	GrayStretch(lpDIBBits, lpDestImage,
		wDIBWidth, wDIBHeight, wBytesPerLine,
		X1, X2, Y1, Y2);


	// a filtered image is available in lpDestImage
	// copy it to DIB bits
	memcpy(lpDIBBits, lpDestImage, dwImageSize);

	// cleanup temp buffers
	GlobalUnlock(hFilteredBits);
	GlobalFree(hFilteredBits);
	GlobalUnlock(hNewDib);

	// rebuild hDib
	HDIB hTmp = NULL;
	if (wBitCount != 24)
		hTmp = ConvertDIBFormat(hNewDib, wBitCount, NULL);
	else
		hTmp = CopyHandle(hNewDib);
	GlobalFree(hNewDib);
	DWORD dwSize = GlobalSize(hTmp);
	memcpy((LPBYTE)GlobalLock(hDib), (LPBYTE)GlobalLock(hTmp), dwSize);
	GlobalUnlock(hTmp);
	GlobalFree(hTmp);
	GlobalUnlock(hDib);
	WaitCursorEnd();

	return TRUE;
}

BOOL LocalImageEnhancementDIB(HDIB hDib)
{
	return 0;
}

BOOL MyMedianFilterDIB(HDIB hDib)
{
	return 0;
}

BOOL SobleOperatorDIB(HDIB hDib)
{
	return 0;
}

BOOL LaplacianSharpeningDIB(HDIB hDib)
{
	return 0;
}

BOOL PalmprintImageEnhancementDIB(HDIB hDib)
{
	return 0;
}

BOOL FrequencyDomainFilterDIB(HDIB hDib)
{
	return 0;
}

BOOL HomomorphicFilterDIB(HDIB hDib)
{
	return 0;
}

BOOL BattivoLowPassFilterDIB(HDIB hDib)
{
	return 0;
}

BOOL BattivoHighPassFilterDIB(HDIB hDib)
{
	return 0;
}

BOOL BattivoBandpassFilterDIB(HDIB hDib)
{
	return 0;
}

BOOL BattivoBandRejectionFilterDIB(HDIB hDib)
{
	return 0;
}

BOOL Exercise443DIB(HDIB hDib)
{
	return 0;
}

BOOL VariableThresholdDIB(HDIB hDib)
{
	return 0;
}

BOOL OstuImageSegmentationDIB(HDIB hDib)
{
	return 0;
}

BOOL FaceRecognitionDIB(HDIB hDib)
{
	return 0;
}

BOOL IrisSegmentationDIB(HDIB hDib)
{
	return 0;
}

BOOL BoundaryTrackingAlgorithmDIB(HDIB hDib)
{
	return 0;
}

BOOL RegionRefinementDIB(HDIB hDib)
{
	return 0;
}

BOOL GraylevelCooccurrenceMatrixDIB(HDIB hDib)
{
	return 0;
}

BOOL Example1116DIB(HDIB hDib)
{
	return 0;
}

BOOL Example1127DIB(HDIB hDib)
{
	return 0;
}

BOOL FigureRecognitionDIB(HDIB hDib)
{
	return 0;
}

BOOL PeachImageRecognitionDIB(HDIB hDib)
{
	return 0;
}




