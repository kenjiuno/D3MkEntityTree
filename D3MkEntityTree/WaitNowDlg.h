
// +--------------------------------------------------
// |
// | WaitNowDlg.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once


// CWaitNowDlg ダイアログ

class CWaitNowDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaitNowDlg)

public:
	CWaitNowDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CWaitNowDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_WAIT_NOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
