#pragma once
#include "h:\hdqrs\program.5\extra\d3mkentitytree\d3mkentitytree\custricheditctrl.h"

class CTool3Dlg : public CDialog
{
	// 
	DECLARE_DYNAMIC(CTool3Dlg)

public:
	// 
	CTool3Dlg(CWnd* pParent = NULL);
	// 
	bool Create(CWnd *pParentWnd)
	{
		if (CreateDlg(MAKEINTRESOURCE(IDD), pParentWnd))
			return true;
		return false;
	}

// �_�C�A���O �f�[�^
	enum { IDD = IDD_TOOL3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CString m_strExe;
	CEdit m_wndEditExe;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonTest();
	CCustRichEditCtrl m_wndRich;
	virtual BOOL OnInitDialog();
};
