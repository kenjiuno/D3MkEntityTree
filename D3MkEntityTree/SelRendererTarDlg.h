#pragma once
#include "afxwin.h"


// CSelRendererTarDlg �_�C�A���O

class CSelRendererTarDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelRendererTarDlg)

public:
	CSelRendererTarDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CSelRendererTarDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SEL_RENDERTAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	int m_iSel;
	CStatic m_wndSample;
	CStatic m_wndPv;
	afx_msg void OnBnClickedButtonTest();
	CComboBox m_wndSel;
	CStatic m_wndDev;
};
