#pragma once


// COptsDlg ダイアログ

class COptsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptsDlg)

public:
	COptsDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~COptsDlg();

// ダイアログ データ
	enum { IDD = IDD_OPTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDoom3;
	afx_msg void OnBnClickedButtonDoom3();
};
