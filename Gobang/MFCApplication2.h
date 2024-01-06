
// MFCApplication2.h : main header file for the MFCApplication2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Options.h"
#include "MFCApplication2View.h"
#include "MainFrm.h"
#include <stack>


// CMFCApplication2App:
// See MFCApplication2.cpp for the implementation of this class
//

class CMFCApplication2App : public CWinAppEx
{
public:
	CMFCApplication2App() noexcept;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	
};

extern CMFCApplication2App theApp;
