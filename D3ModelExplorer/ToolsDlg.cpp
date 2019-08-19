// ToolsDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "D3ModelExplorer.h"
#include "ToolsDlg.h"
#include ".\toolsdlg.h"


// CToolsDlg ダイアログ

IMPLEMENT_DYNAMIC(CToolsDlg, CDialog)
CToolsDlg::CToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolsDlg::IDD, pParent)
{
}

void CToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_TOOLS, m_wndTab);
}


BEGIN_MESSAGE_MAP(CToolsDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TOOLS, OnTcnSelchangeTabTools)
END_MESSAGE_MAP()


// CToolsDlg メッセージ ハンドラ

BOOL CToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	lf.lfWeight = FW_BOLD;
	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf);

	w1.Create(this);
	w2.Create(this);
	w3.Create(this);

	CString str;
	
	w1.GetWindowText(str); m_wndTab.InsertItem(0, str);
	w2.GetWindowText(str); m_wndTab.InsertItem(1, str);
	w3.GetWindowText(str); m_wndTab.InsertItem(2, str);

	SetsUp(w1);
	SetsUp(w2);
	SetsUp(w3);

	Sel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CToolsDlg::OnTcnSelchangeTabTools(NMHDR *pNMHDR, LRESULT *pResult)
{
	Sel(m_wndTab.GetCurSel());

	*pResult = 0;
}

void CToolsDlg::OnOK()
{
	w1.UpdateData();
	w2.UpdateData();
	w3.UpdateData();

	CDialog::OnOK();
}
