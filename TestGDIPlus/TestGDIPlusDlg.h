
// TestGDIPlusDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


class Gdiplus::Image;


// CTestGDIPlusDlg 对话框
class CTestGDIPlusDlg : public CDialogEx
{
// 构造
public:
	CTestGDIPlusDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTGDIPLUS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	Gdiplus::Image* m_pImageBg; //背景


	Gdiplus::Image* m_pImage_Zhuce; //注册

	HBITMAP m_hMemBmp;
	CRect m_rcClient;

	CRect m_rc_btn_zhuce;

	HBITMAP CreateMemBitmap(); //创建位图

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CButton m_btn_zc;
	afx_msg void OnBnClickedBtnZhuce();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
