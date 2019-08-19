// ConfDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MD5AnimCraftzman.h"
#include "ConfDlg.h"
#include "OSP.h"
#include "ProfStr.h"

using namespace OSP;

// CConfDlg ダイアログ

IMPLEMENT_DYNAMIC(CConfDlg, CDialog)
CConfDlg::CConfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfDlg::IDD, pParent)
	, m_strDoom3(_T(""))
{
}

CConfDlg::~CConfDlg()
{
}

void CConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DOOM3, m_strDoom3);
	DDX_Control(pDX, IDC_EDIT_DOOM3, m_wndDoom3);
}


BEGIN_MESSAGE_MAP(CConfDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DOOM3, OnBnClickedButtonDoom3)
END_MESSAGE_MAP()

static const DWORD nSelDirFlags = 0
	|OFN_ENABLESIZING
	|OFN_EXPLORER
	|OFN_HIDEREADONLY
	|OFN_LONGNAMES
	|OFN_PATHMUSTEXIST
	;

// CConfDlg メッセージ ハンドラ

void CConfDlg::OnBnClickedButtonDoom3()
{
	CString strFileName;

	m_wndDoom3.GetWindowText(strFileName);

	CFileDialog wndDlg(
		true,
		"exe",
		OSP_JoinPath(strFileName, "(DIR)"),
		nSelDirFlags,
		"DOOM3.exe (*.exe)|*.exe||",
		this
		);
	if (wndDlg.DoModal() != IDOK) return;

	strFileName = OSP_GetDir(wndDlg.GetPathName(), false);

	m_wndDoom3.SetWindowText(strFileName);
}

void CConfDlg::Revert(bool f)
{
	if (f) {
		ProfStr::LoadProfileStringFrom("D3AnimCraftzman", "Doom3", m_strDoom3, "", GetCnfFile());
	} else {
		ProfStr::SaveProfileStringTo("D3AnimCraftzman", "Doom3", m_strDoom3, GetCnfFile());
	}
}
