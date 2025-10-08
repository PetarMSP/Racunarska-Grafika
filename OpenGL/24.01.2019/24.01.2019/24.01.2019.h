
// 24.01.2019.h : main header file for the 24.01.2019 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy24012019App:
// See 24.01.2019.cpp for the implementation of this class
//

class CMy24012019App : public CWinApp
{
public:
	CMy24012019App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy24012019App theApp;
