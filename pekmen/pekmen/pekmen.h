
// pekmen.h : main header file for the pekmen application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CpekmenApp:
// See pekmen.cpp for the implementation of this class
//

class CpekmenApp : public CWinApp
{
public:
	CpekmenApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CpekmenApp theApp;
