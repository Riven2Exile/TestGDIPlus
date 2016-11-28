
// TestGDIPlusDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestGDIPlus.h"
#include "TestGDIPlusDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestGDIPlusDlg �Ի���



CTestGDIPlusDlg::CTestGDIPlusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestGDIPlusDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestGDIPlusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_BTN_ZHUCE, m_btn_zc);
}

BEGIN_MESSAGE_MAP(CTestGDIPlusDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDB_BTN_ZHUCE, &CTestGDIPlusDlg::OnBnClickedBtnZhuce)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CTestGDIPlusDlg ��Ϣ�������

BOOL CTestGDIPlusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	// 1. gdi�ĳ�ʼ��!!!
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// ��� GWL_EXSTYLE ����, UpdateLayeredWindow���������Ҫ�õ�
	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED) //�˴������� ����͸����
	{
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED); 
	}
	

	ModifyStyle(0, WS_MINIMIZEBOX | // ���������������С��
		 //WS_CAPTION |
		WS_SYSMENU);	 // �һ����������Ե���ϵͳ�˵�

	// ���⣬�������ȥ����ͻ�����Ļ���Ҫ�ѶԻ����border���ԣ��� title bar ȥ��

	//// һ��ͼ
	wchar_t str[MAX_PATH] = { 0 };
	GetCurrentDirectory(sizeof(str), str);
	wchar_t *p = L"bg.png";
	m_pImageBg = Gdiplus::Image::FromFile(p);


	// ����������С
	GetWindowRect(&m_rcClient);
	SetWindowPos(NULL, m_rcClient.left, m_rcClient.top, m_pImageBg->GetWidth(), m_pImageBg->GetHeight(), SWP_SHOWWINDOW);
	m_rcClient.right = m_rcClient.left + m_pImageBg->GetWidth();
	m_rcClient.bottom = m_rcClient.top + m_pImageBg->GetHeight();

	// ��������λͼ
	m_hMemBmp = CreateMemBitmap();



	// ��ťλͼ
	m_pImage_Zhuce = Gdiplus::Image::FromFile(L"start1.png");

	m_btn_zc.GetWindowRect(&m_rc_btn_zhuce);
	//ScreenToClient(&m_rc_btn_zhuce); //ת�ɿͻ����ĵ�
	//m_btn_zc.GetWindowRect(&m_rc_btn_zhuce);
	m_rc_btn_zhuce.right = m_rc_btn_zhuce.left + m_pImage_Zhuce->GetWidth();
	m_rc_btn_zhuce.bottom = m_rc_btn_zhuce.top + m_pImage_Zhuce->GetHeight();
	m_btn_zc.MoveWindow(&m_rc_btn_zhuce, TRUE); //���������ô�С

	//rgn.CreatePolygonRgn()
	CPoint pos[4];
	int half_w = m_pImage_Zhuce->GetWidth() / 2;
	pos[0] = CPoint(half_w, 0);
	pos[1] = CPoint(m_pImage_Zhuce->GetWidth(), half_w);
	pos[2] = CPoint(half_w, m_pImage_Zhuce->GetWidth());
	pos[3] = CPoint(0, half_w);
	CRgn rgn;
	BOOL ret = rgn.CreatePolygonRgn(pos, 4, ALTERNATE);
	m_btn_zc.SetWindowRgn(rgn, TRUE);
	
	
	//BOOL ret = rgn.CreateRectRgn(0, 0, rcTemp.Width(), rcTemp.Height());
	

	

	this->SetTimer(1, 30, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestGDIPlusDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestGDIPlusDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestGDIPlusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestGDIPlusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1:
	{

		// ÿ֡����
		CClientDC dc(this);
		CDC* pDC = &dc;

		// ����MemDC
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		HGDIOBJ hOldBmp = ::SelectObject(memDC.GetSafeHdc(), m_hMemBmp);
		COLORREF dwColor = 0x00000000;
		memDC.FillSolidRect(m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(), dwColor);

		Gdiplus::Graphics graphics(memDC.GetSafeHdc());

		//���Ʊ���
		graphics.DrawImage(m_pImageBg, m_rcClient.left, m_rcClient.top, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);


		//���ư�ť
// 		m_btn_zc.GetWindowRect(&m_rc_btn_zhuce);
// 		ScreenToClient(&m_rc_btn_zhuce);
		graphics.DrawImage(m_pImage_Zhuce, m_rc_btn_zhuce.left, m_rc_btn_zhuce.top, m_rc_btn_zhuce.Width(), m_rc_btn_zhuce.Height());


		/////
		BLENDFUNCTION blendfunc;
		blendfunc.BlendOp = AC_SRC_OVER;
		blendfunc.BlendFlags = 0;
		blendfunc.SourceConstantAlpha = 255;
		blendfunc.AlphaFormat = AC_SRC_ALPHA;

		static CRect wnd_rc;
		GetWindowRect(&wnd_rc);
		CPoint ptWnd(wnd_rc.TopLeft());
		CSize szWnd = wnd_rc.Size();
		CPoint ptSrc(0, 0);

		// UpdateLayeredWindow
		UpdateLayeredWindow(pDC, &ptWnd, &szWnd, &memDC, &ptSrc, 0, &blendfunc, ULW_ALPHA);


		::SelectObject(memDC.GetSafeHdc(), hOldBmp);

		//AfxMessageBox(L"a");
	}break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

HBITMAP CTestGDIPlusDlg::CreateMemBitmap()
{
	BYTE* pBits = NULL;
	BITMAPINFOHEADER bmih = { sizeof(BITMAPINFOHEADER) };

	bmih.biWidth = m_rcClient.right - m_rcClient.left;
	bmih.biHeight = m_rcClient.bottom - m_rcClient.top;
	bmih.biPlanes = 1;
	bmih.biBitCount = 32;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	HBITMAP hMemBitmap = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS, (VOID**)&pBits, NULL, 0);
	return hMemBitmap;
}


void CTestGDIPlusDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, NULL); //ͬ�����ʽ������!
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CTestGDIPlusDlg::OnBnClickedBtnZhuce()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	AfxMessageBox(L"ע�� ");
}


LRESULT CTestGDIPlusDlg::OnNcHitTest(CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialogEx::OnNcHitTest(point);
}


BOOL CTestGDIPlusDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ����ר�ô����/����û���
	return CDialogEx::PreCreateWindow(cs);
}
