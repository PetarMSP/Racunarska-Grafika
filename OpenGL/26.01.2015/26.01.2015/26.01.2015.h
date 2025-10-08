
// 26.01.2015.h : main header file for the 26.01.2015 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy26012015App:
// See 26.01.2015.cpp for the implementation of this class
//

class CMy26012015App : public CWinApp
{
public:
	CMy26012015App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy26012015App theApp;
