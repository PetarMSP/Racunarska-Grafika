
// 27.01.2011.h : main header file for the 27.01.2011 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy27012011App:
// See 27.01.2011.cpp for the implementation of this class
//

class CMy27012011App : public CWinApp
{
public:
	CMy27012011App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy27012011App theApp;
