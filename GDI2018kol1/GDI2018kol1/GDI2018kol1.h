
// GDI2018kol1.h : main header file for the GDI2018kol1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGDI2018kol1App:
// See GDI2018kol1.cpp for the implementation of this class
//

class CGDI2018kol1App : public CWinApp
{
public:
	CGDI2018kol1App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGDI2018kol1App theApp;
