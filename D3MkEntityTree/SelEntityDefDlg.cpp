
// +--------------------------------------------------
// |
// | SelEntityDefDlg.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "stdafx.h"
#include "D3MkEntityTree.h"
#include "SelEntityDefDlg.h"
#include "Context.h"
#include "WaitNowDlg.h"

#define DEFAULT_CHECK_STATE 0

// CSelEntityDefDlg ダイアログ

IMPLEMENT_DYNAMIC(CSelEntityDefDlg, CDialog)
CSelEntityDefDlg::CSelEntityDefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelEntityDefDlg::IDD, pParent)
	, m_fOnly(FALSE)
	, m_gs(gsDoom3)
{
}

CSelEntityDefDlg::~CSelEntityDefDlg()
{
}

void CSelEntityDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SEL, m_wndTree);
	DDX_Check(pDX, IDC_CHECK_ONLY, m_fOnly);
}


BEGIN_MESSAGE_MAP(CSelEntityDefDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CSelEntityDefDlg メッセージ ハンドラ

void CSelEntityDefDlg::OnBnClickedButtonRefresh()
{
	if (!UpdateData()) return;

	CMyDoom3Context a;
	a.Run(m_strDoom3, "", false, false, false, false, false, m_fUse_entityDef ? croParseDef : croParseMaterial, false, 0, m_gs);

	m_wndTree.DeleteAllItems();
	m_em.clear();
	m_vm.clear();

	m_hRoot = m_wndTree.InsertItem(m_fUse_entityDef ? "entityDef" : "Mapobjects", 0, 0);

	CWaitCursor wc;

	CDoom3Workpad &wk = a.Get_wk();
	if (m_fUse_entityDef) {
		CDoom3entityDefMap::iterator
			iterPos = wk.entityDefMap.begin(),
			iterEnd = wk.entityDefMap.end();
		for (; iterPos != iterEnd; iterPos++) {
			HTREEITEM h = AddElement(iterPos->first);
			m_vm[iterPos->first] = h;
		}
	} else {
		NameSet::iterator
			iterPos = wk.mapobjsNames.begin(),
			iterEnd = wk.mapobjsNames.end();
		for (; iterPos != iterEnd; iterPos++) {
			HTREEITEM h = AddElement(*iterPos);
			m_vm[*iterPos] = h;
		}
	}

	{
		ElementMap::iterator
			iterPos = m_vm.begin(),
			iterEnd = m_vm.end();
		for (; iterPos != iterEnd; iterPos++) {
			if (IsMarked(iterPos->first)) m_wndTree.SetCheckState(iterPos->second, 1);
		}
	}

	m_wndTree.ShowWindow(SW_HIDE);
	UpdateWindow();
	m_wndTree.ShowWindow(SW_SHOW);
}

HTREEITEM CSelEntityDefDlg::AddElement(CString str)
{
	char cMark = m_fUse_entityDef ? '_' : '/';

	HTREEITEM h = m_hRoot;
	int p0 = 0;
	for (; ; ) {
		int p1 = str.Find(cMark, p0);
		int n1 = (p1 < 0) ? str.GetLength() : p1;

		CString strElem = str.Mid(0, n1);

		ElementMap::iterator
			iterPos = m_em.find(strElem),
			iterEnd = m_em.end();
		if (iterPos == iterEnd) {
			CString strName = GetName(strElem);
			h = m_wndTree.InsertItem(strName, 0, 0, h);
			m_em[strElem] = h;
		} else {
			h = iterPos->second;
		}

		if (p1 < 0) {
			return h;
		}
		p0 = p1 +1;
	}
}

void CSelEntityDefDlg::Commit()
{
	m_names.clear();

	ElementMap::iterator
		iterPos = m_vm.begin(),
		iterEnd = m_vm.end();
	for (; iterPos != iterEnd; iterPos++) {
		if (m_wndTree.GetCheckState(iterPos->second) == 1)
			m_names.insert(iterPos->first);
	}
}

void CSelEntityDefDlg::OnBnClickedOk()
{
	if (!UpdateData()) return;

	Commit();
	OnOK();
}

BOOL CSelEntityDefDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_hRoot = m_wndTree.InsertItem(m_fUse_entityDef ? "entityDef" : "Mapobjects", 0, 0);

	CWaitCursor wc;

	{
		int n = 0;

		NameSet::iterator
			iterPos = m_names.begin(),
			iterEnd = m_names.end();
		for (; iterPos != iterEnd; iterPos++) {
			HTREEITEM h = AddElement(*iterPos);
			m_wndTree.SetCheckState(h, 1);
			m_vm[*iterPos] = h;
			n++;
		}

		if (n == 0) m_wndTree.DeleteItem(m_hRoot), m_hRoot = NULL;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
