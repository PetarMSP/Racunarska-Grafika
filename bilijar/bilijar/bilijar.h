
// bilijar.h : main header file for the bilijar application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CbilijarApp:
// See bilijar.cpp for the implementation of this class
//

class CbilijarApp : public CWinApp
{
public:
	CbilijarApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CbilijarApp theApp;
