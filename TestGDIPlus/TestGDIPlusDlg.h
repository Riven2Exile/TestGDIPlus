
// TestGDIPlusDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


class Gdiplus::Image;


// CTestGDIPlusDlg �Ի���
class CTestGDIPlusDlg : public CDialogEx
{
// ����
public:
	CTestGDIPlusDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTGDIPLUS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	Gdiplus::Image* m_pImageBg; //����


	Gdiplus::Image* m_pImage_Zhuce; //ע��

	HBITMAP m_hMemBmp;
	CRect m_rcClient;

	CRect m_rc_btn_zhuce;

	HBITMAP CreateMemBitmap(); //����λͼ

	// ���ɵ���Ϣӳ�亯��
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
