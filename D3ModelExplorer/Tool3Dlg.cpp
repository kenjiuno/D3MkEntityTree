// Tool3Dlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "D3ModelExplorer.h"
#include "Tool3Dlg.h"
#include "FUt.h"
#include ".\tool3dlg.h"


// CTool3Dlg ダイアログ

IMPLEMENT_DYNAMIC(CTool3Dlg, CDialog)
CTool3Dlg::CTool3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTool3Dlg::IDD, pParent)
	, m_strExe(_T(""))
{
}

void CTool3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strExe);
	DDX_Control(pDX, IDC_EDIT1, m_wndEditExe);
	DDX_Control(pDX, IDC_RICHED_CREDITS, m_wndRich);
}


BEGIN_MESSAGE_MAP(CTool3Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CTool3Dlg メッセージ ハンドラ

void CTool3Dlg::OnBnClickedButton1()
{
	SelectToolFile(m_wndEditExe, "exe", "xKViewer.exe (*.exe)|*.exe||");
}

void CTool3Dlg::OnBnClickedButtonTest()
{
	if (!UpdateData())
		return;
	if (FileExists(m_strExe)) {
		AfxMessageBox(IDS_PROGRAM_VALID, MB_OK|MB_ICONINFORMATION);
	}
	else {
		AfxMessageBox(IDS_PROGRAM_ABSENT, MB_OK|MB_ICONEXCLAMATION);
		OnBnClickedButton1();
	}

	AfxMessageBox(IDS_CHECK_FINE, MB_OK|MB_ICONINFORMATION);
}

BOOL CTool3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndRich.EnableAutoUrlDetect();
	m_wndRich.LoadFromFile( FindGoodFile("Tool3.rtf") );
	m_wndRich.EnableLink4Browse();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
