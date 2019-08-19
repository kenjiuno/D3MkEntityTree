#pragma once

// CTool1Dlg ダイアログ

class CTool1Dlg : public CDialog
{
	// 
	DECLARE_DYNAMIC(CTool1Dlg)

public:
	// 
	CTool1Dlg(CWnd* pParent = NULL);
	// 
	bool Create(CWnd *pParentWnd)
	{
		if (CreateDlg(MAKEINTRESOURCE(IDD), pParentWnd))
			return true;
		return false;
	}

// ダイアログ データ
	enum { IDD = IDD_TOOL1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CString m_strExe;
	CEdit m_wndEdit1;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonTest();
	CCustRichEditCtrl m_wndRich;
	virtual BOOL OnInitDialog();
};
