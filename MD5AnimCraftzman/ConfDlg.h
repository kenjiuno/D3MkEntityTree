#pragma once
#include "afxwin.h"


// CConfDlg �_�C�A���O

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

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CONF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDoom3;
	afx_msg void OnBnClickedButtonDoom3();
	CEdit m_wndDoom3;
};
