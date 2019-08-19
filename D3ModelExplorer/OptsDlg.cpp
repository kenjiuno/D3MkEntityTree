// OptsDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "D3ModelExplorer.h"
#include "OptsDlg.h"
#include "OSP.h"

using namespace OSP;

// COptsDlg �_�C�A���O

IMPLEMENT_DYNAMIC(COptsDlg, CDialog)
COptsDlg::COptsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptsDlg::IDD, pParent)
	, m_strDoom3(_T(""))
{
}

COptsDlg::~COptsDlg()
{
}

void COptsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DOOM3, m_strDoom3);
}


BEGIN_MESSAGE_MAP(COptsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DOOM3, OnBnClickedButtonDoom3)
END_MESSAGE_MAP()


// COptsDlg ���b�Z�[�W �n���h��

static const DWORD nSelDirFlags = 0
	|OFN_ENABLESIZING
	|OFN_EXPLORER
	|OFN_HIDEREADONLY
	|OFN_LONGNAMES
	|OFN_PATHMUSTEXIST
	;

void COptsDlg::OnBnClickedButtonDoom3()
{
	if (!UpdateData()) return;

	CFileDialog wndDlg(
		true,
		"exe",
		OSP_JoinPath(m_strDoom3, "(DIR)"),
		nSelDirFlags,
		"DOOM3.exe (*.exe)|*.exe||",
		this
		);
	if (wndDlg.DoModal() != IDOK) return;

	m_strDoom3 = OSP_GetDir(wndDlg.GetPathName());
	UpdateData(false);
}
