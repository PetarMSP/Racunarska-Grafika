
// 22.01.2013.h : main header file for the 22.01.2013 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy22012013App:
// See 22.01.2013.cpp for the implementation of this class
//

class CMy22012013App : public CWinApp
{
public:
	CMy22012013App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy22012013App theApp;
