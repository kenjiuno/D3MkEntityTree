
// +--------------------------------------------------
// |
// | Aboutdlg.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once
#include "afxcmn.h"
#include "CustRichEditCtrl.h"

// CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CCustRichEditCtrl m_wndHint;
	virtual BOOL OnInitDialog();
};
