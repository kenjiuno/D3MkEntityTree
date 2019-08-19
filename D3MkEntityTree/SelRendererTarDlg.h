#pragma once
#include "afxwin.h"


// CSelRendererTarDlg ダイアログ

class CSelRendererTarDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelRendererTarDlg)

public:
	CSelRendererTarDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSelRendererTarDlg();

// ダイアログ データ
	enum { IDD = IDD_SEL_RENDERTAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int m_iSel;
	CStatic m_wndSample;
	CStatic m_wndPv;
	afx_msg void OnBnClickedButtonTest();
	CComboBox m_wndSel;
	CStatic m_wndDev;
};
