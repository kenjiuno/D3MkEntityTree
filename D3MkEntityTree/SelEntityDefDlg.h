
// +--------------------------------------------------
// |
// | SelEntityDefDlg.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

#include "CheckMarkedTreeCtrl.h"
#include "Structure.h"

// CSelEntityDefDlg ダイアログ

class CSelEntityDefDlg : public CDialog
{
	// 
	typedef map<CString, HTREEITEM> ElementMap;

	// 
	ElementMap m_em, m_vm;
	// 
	HTREEITEM m_hRoot;

	// 
	HTREEITEM AddElement(CString str);
	// 
	bool IsMarked(CString str)
	{
		return m_fSelAll || (m_names.find(str) != m_names.end());
	}

	// 
	static CString GetName(CString str)
	{
		int p = str.ReverseFind('/');
		if (p < 0)
			return str;
		return str.Mid(p +1);
	}
	// 
	void Commit();

	// 
	DECLARE_DYNAMIC(CSelEntityDefDlg)

public:
	// 
	enum { IDD = IDD_DIALOG_SEL_ENTITYDEF };

	// 
	CString m_strDoom3;
	// 
	bool m_fUse_entityDef;
	// 
	bool m_fSelAll;
	// 
	NameSet m_names;
	// 
	GameSel m_gs;

	// 
	CSelEntityDefDlg(CWnd* pParent = NULL);
	// 
	virtual ~CSelEntityDefDlg();

protected:
	// 
	virtual void DoDataExchange(CDataExchange* pDX);
	//
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonRefresh();
	CCheckMarkedTreeCtrl m_wndTree;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	BOOL m_fOnly;
};
