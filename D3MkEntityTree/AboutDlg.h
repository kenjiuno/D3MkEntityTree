
// +--------------------------------------------------
// |
// | Aboutdlg.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once
#include "afxcmn.h"
#include "CustRichEditCtrl.h"

// CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CCustRichEditCtrl m_wndHint;
	virtual BOOL OnInitDialog();
};
