
// TestGDIPlus.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestGDIPlusApp: 
// �йش����ʵ�֣������ TestGDIPlus.cpp
//

class CTestGDIPlusApp : public CWinApp
{
public:
	CTestGDIPlusApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestGDIPlusApp theApp;