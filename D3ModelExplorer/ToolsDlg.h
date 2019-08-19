
#pragma once

#include "Tool1Dlg.h"
#include "Tool2Dlg.h"
#include "Tool3Dlg.h"
#include "afxcmn.h"

class CToolsDlg : public CDialog
{
	// 
	CFont m_font;

	// 
	void Sel(int i)
	{
		w1.ShowWindow(SW_HIDE);
		w2.ShowWindow(SW_HIDE);
		w3.ShowWindow(SW_HIDE);

		CWnd *p = NULL;
		switch (i) {
		case 0: p = &w1; break;
		case 1: p = &w2; break;
		case 2: p = &w3; break;
		}
		if (p == NULL) return;

		p->ShowWindow(SW_SHOW);
	}
	// 
	void SetsUp(CWnd &w)
	{
		CWnd *p = &w;

		CRect rc;
		m_wndTab.GetWindowRect(rc);
		m_wndTab.AdjustRect(false, &rc);

		ScreenToClient(&rc);

		p->ModifyStyle(
			0 |WS_POPUP |WS_CAPTION |WS_DISABLED, 
			0 |WS_CHILD |DS_CONTROL, 
			SWP_FRAMECHANGED
			);
		p->ModifyStyleEx(
			0, 
			0 |WS_EX_CONTROLPARENT, 
			SWP_FRAMECHANGED
			);

		p->MoveWindow(&rc);

		CWnd *p2 = p->GetDlgItem(IDC_STATIC_CAPTION);
		if (p2) {
			p2->SetFont(&m_font);
		}
	}

	// 
	DECLARE_DYNAMIC(CToolsDlg)

public:
	// 
	CTool1Dlg w1;
	// 
	CTool2Dlg w2;
	// 
	CTool3Dlg w3;

	// 
	CToolsDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_wndTab;
	afx_msg void OnTcnSelchangeTabTools(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnOK();
};
