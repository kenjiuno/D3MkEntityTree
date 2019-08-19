// Tool2Dlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "D3ModelExplorer.h"
#include "Tool2Dlg.h"
#include "FUt.h"
#include ".\tool2dlg.h"


// CTool2Dlg ダイアログ

IMPLEMENT_DYNAMIC(CTool2Dlg, CDialog)
CTool2Dlg::CTool2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTool2Dlg::IDD, pParent)
	, m_strExe(_T(""))
{
}

void CTool2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strExe);
	DDX_Control(pDX, IDC_EDIT1, m_wndEditExe);
	DDX_Control(pDX, IDC_RICHED_CREDITS, m_wndRich);
}


BEGIN_MESSAGE_MAP(CTool2Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CTool2Dlg メッセージ ハンドラ

void CTool2Dlg::OnBnClickedButton1()
{
	SelectToolFile(m_wndEditExe, "exe", "modelviewer.exe (*.exe)|*.exe||");
}

void CTool2Dlg::OnBnClickedButtonTest()
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

BOOL CTool2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndRich.EnableAutoUrlDetect();
	m_wndRich.LoadFromFile( FindGoodFile("Tool2.rtf") );
	m_wndRich.EnableLink4Browse();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
