// MD5AnimCraftzman.h : main header file for the MD5AnimCraftzman application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "ConfDlg.h"

// CMD5AnimCraftzmanApp:
// See MD5AnimCraftzman.cpp for the implementation of this class
//

#include "Context.h"

#include "OnIdle.h"

#include <algorithm>

class CMD5AnimCraftzmanApp : public CWinApp
{
	// 
	CConfDlg m_wndCnf;
	// 
	CMyDoom3Context m_com;

	// 
	vector<COnIdle *> m_vec;

public:
	// 
	CString m_strCnf;

	// 
	CMD5AnimCraftzmanApp();
	// 
	CMyDoom3Context &GetCom()
	{
		return m_com;
	}
	// 
	void AddIdle(COnIdle *p)
	{
		m_vec.push_back(p);
	}
	// 
	void RemoveIdle(COnIdle *p)
	{
		m_vec.erase(find(m_vec.begin(), m_vec.end(), p), m_vec.end());
	}

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnOptsConf();
	afx_msg void OnOptsUpdate();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
};

extern CMD5AnimCraftzmanApp theApp;

CMD5AnimCraftzmanApp *GetApp();

CMyDoom3Context &GetCom();

inline CString GetCnfFile() { return GetApp()->m_strCnf; }
