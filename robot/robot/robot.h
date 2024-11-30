
// robot.h : main header file for the robot application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CrobotApp:
// See robot.cpp for the implementation of this class
//

class CrobotApp : public CWinApp
{
public:
	CrobotApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CrobotApp theApp;
