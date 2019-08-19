// D3ModelExplorer.h : main header file for the D3ModelExplorer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

extern CString g_strCnf;
extern CString g_strExpDir;

// CD3ModelExplorerApp:
// See D3ModelExplorer.cpp for the implementation of this class
//

class CD3ModelExplorerApp : public CWinApp
{
public:
	CD3ModelExplorerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CD3ModelExplorerApp theApp;

extern
void SelectToolFile(CEdit &wndEdit, LPCTSTR pszExt, LPCTSTR pszFilter);

extern
CString FindGoodFile(CString strPath);
