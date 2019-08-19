
// +--------------------------------------------------
// |
// | AboutDlg.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "stdafx.h"
#include "D3MkEntityTree.h"
#include "AboutDlg.h"


// CAboutDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_HINT, m_wndHint);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAboutDlg ���b�Z�[�W �n���h��

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndHint.EnableAutoUrlDetect();
	m_wndHint.LoadFromResource("RTF", MAKEINTRESOURCE(IDR_RTF_ABOUT));
	m_wndHint.EnableLink4Browse();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
