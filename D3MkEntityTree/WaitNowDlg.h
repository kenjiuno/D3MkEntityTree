
// +--------------------------------------------------
// |
// | WaitNowDlg.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once


// CWaitNowDlg �_�C�A���O

class CWaitNowDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaitNowDlg)

public:
	CWaitNowDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CWaitNowDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_WAIT_NOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
