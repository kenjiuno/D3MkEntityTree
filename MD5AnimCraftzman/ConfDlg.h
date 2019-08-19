#pragma once
#include "afxwin.h"


// CConfDlg ダイアログ

class CConfDlg : public CDialog
{
	// 
	DECLARE_DYNAMIC(CConfDlg)

public:
	// 
	CConfDlg(CWnd* pParent = NULL);
	// 
	virtual ~CConfDlg();
	// 
	void Revert(bool f);

// ダイアログ データ
	enum { IDD = IDD_CONF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDoom3;
	afx_msg void OnBnClickedButtonDoom3();
	CEdit m_wndDoom3;
};
