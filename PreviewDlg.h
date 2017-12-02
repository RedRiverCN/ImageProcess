#pragma once

// CPreViewDlg 对话框

#include "resource.h"
#include "Dibapi.h"
class CPreviewDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	int		m_nType;
	bool	m_bEnableClose;

	HDIB m_hDib;
	UINT	 m_nTempWidth;				// 处理后图像的宽度
	UINT	 m_nTempHeight;				// 处理后图像的高度
	BYTE*	m_pImageTempBuffer;			// 处理后的像素数组
//	int X_image; //图像的宽度
//	int Y_image; //图像的高度

//	int		m_nPicWidth;
//	int		m_nPicHeight;

	CPreviewDlg(UINT	 nTempWidth, UINT	 nTempHeight, BYTE*	pImageTempBuffer, HDIB m_hDib, CWnd* pParent = NULL);
	CPreviewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPreviewDlg();

	virtual BOOL OnInitDialog();
	void OnOK();
	void OnCancel();
	
	void Refresh()
	{
		Invalidate();
	}

	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();

	// 对话框数据
	enum { IDD = IDD_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
