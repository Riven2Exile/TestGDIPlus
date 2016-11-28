
// TestGDIPlusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestGDIPlus.h"
#include "TestGDIPlusDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTestGDIPlusDlg 对话框



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


// CTestGDIPlusDlg 消息处理程序

BOOL CTestGDIPlusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	// 1. gdi的初始化!!!
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// 添加 GWL_EXSTYLE 属性, UpdateLayeredWindow这个函数需要用到
	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED) //此处处理导致 窗体透明了
	{
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED); 
	}
	

	ModifyStyle(0, WS_MINIMIZEBOX | // 点击任务栏可以最小化
		 //WS_CAPTION |
		WS_SYSMENU);	 // 右击任务栏可以弹出系统菜单

	// 另外，如果懒得去计算客户区域的话，要把对话框的border属性，和 title bar 去掉

	//// 一张图
	wchar_t str[MAX_PATH] = { 0 };
	GetCurrentDirectory(sizeof(str), str);
	wchar_t *p = L"bg.png";
	m_pImageBg = Gdiplus::Image::FromFile(p);


	// 调整背景大小
	GetWindowRect(&m_rcClient);
	SetWindowPos(NULL, m_rcClient.left, m_rcClient.top, m_pImageBg->GetWidth(), m_pImageBg->GetHeight(), SWP_SHOWWINDOW);
	m_rcClient.right = m_rcClient.left + m_pImageBg->GetWidth();
	m_rcClient.bottom = m_rcClient.top + m_pImageBg->GetHeight();

	// 创建背景位图
	m_hMemBmp = CreateMemBitmap();



	// 按钮位图
	m_pImage_Zhuce = Gdiplus::Image::FromFile(L"start1.png");

	m_btn_zc.GetWindowRect(&m_rc_btn_zhuce);
	//ScreenToClient(&m_rc_btn_zhuce); //转成客户区的点
	//m_btn_zc.GetWindowRect(&m_rc_btn_zhuce);
	m_rc_btn_zhuce.right = m_rc_btn_zhuce.left + m_pImage_Zhuce->GetWidth();
	m_rc_btn_zhuce.bottom = m_rc_btn_zhuce.top + m_pImage_Zhuce->GetHeight();
	m_btn_zc.MoveWindow(&m_rc_btn_zhuce, TRUE); //先重新设置大小

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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestGDIPlusDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestGDIPlusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestGDIPlusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
	{

		// 每帧绘制
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 创建MemDC
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		HGDIOBJ hOldBmp = ::SelectObject(memDC.GetSafeHdc(), m_hMemBmp);
		COLORREF dwColor = 0x00000000;
		memDC.FillSolidRect(m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(), dwColor);

		Gdiplus::Graphics graphics(memDC.GetSafeHdc());

		//绘制背景
		graphics.DrawImage(m_pImageBg, m_rcClient.left, m_rcClient.top, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);


		//绘制按钮
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
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, NULL); //同这个方式并不好!
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CTestGDIPlusDlg::OnBnClickedBtnZhuce()
{
	// TODO:  在此添加控件通知处理程序代码
	AfxMessageBox(L"注册 ");
}


LRESULT CTestGDIPlusDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return CDialogEx::OnNcHitTest(point);
}


BOOL CTestGDIPlusDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此添加专用代码和/或调用基类
	return CDialogEx::PreCreateWindow(cs);
}
