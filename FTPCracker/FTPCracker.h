
// FTPCracker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFTPCrackerApp:
// �йش����ʵ�֣������ FTPCracker.cpp
//

class CFTPCrackerApp : public CWinApp
{
public:
	CFTPCrackerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFTPCrackerApp theApp;