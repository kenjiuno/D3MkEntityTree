
#pragma once
#include "h:\hdqrs\program.5\extra\d3mkentitytree\d3mkentitytree\xhyperlink_demo\xhyperlink.h"

class CDoneDlg : public CDialog
{
public:
	CDoneDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDoneDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CXHyperLink m_wndLink;
	CString m_strHref;
};
