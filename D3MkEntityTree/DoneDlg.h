
#pragma once
#include "h:\hdqrs\program.5\extra\d3mkentitytree\d3mkentitytree\xhyperlink_demo\xhyperlink.h"

class CDoneDlg : public CDialog
{
public:
	CDoneDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDoneDlg();

// ダイアログ データ
	enum { IDD = IDD_DONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CXHyperLink m_wndLink;
	CString m_strHref;
};
