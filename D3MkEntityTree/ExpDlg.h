
// +--------------------------------------------------
// |
// | ExpDlg.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

#include "XHyperLink.h"
#include "Structure.h"
#include "SelRendererTarDlg.h"

class CExpDlg : public CDialog
{
	// 
	CString m_strCnf;
	// 
	CFont m_font;
	// 
	CDoom3ContWorkpad m_cwk;
	// 
	CSelRendererTarDlg m_wndSelTar;

	// 
	DECLARE_DYNAMIC(CExpDlg)
	// 
	DECLARE_MESSAGE_MAP()

public:
	// 
	CExpDlg(CWnd* pParent = NULL);
	// 
	enum { IDD = IDD_DIALOG_UI };

protected:
	// 
	virtual void DoDataExchange(CDataExchange* pDX);
	// 
	void Revert(bool fRevert);
	// 
	LRESULT OnXHyperLink(WPARAM, LPARAM);

public:
	CString m_strDoom3;
	CString m_strNewFolder;
	afx_msg void OnBnClickedButtonDoom3();
	afx_msg void OnBnClickedButtonNewfolder();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	BOOL m_fExpose;
	BOOL m_fGamma;
	BOOL m_fMakeAvi;
	BOOL m_fARB_fragment_program;
	BOOL m_fARB_texture_env_dot3;
	CXHyperLink m_wndSelEntityDef;
	CXHyperLink m_wndSelMapobjs;
	afx_msg void OnStnClickedStaticSelEntitydef();
	afx_msg void OnStnClickedStaticSelMapobjs();
	BOOL m_fMake_entityDef;
	BOOL m_fMake_Mapobjs;
	afx_msg void OnBnClickedButton1();
	BOOL m_fMkHHP;
	BOOL m_fLow;
	BOOL m_fEntityModel;
	CXHyperLink m_wndSelRendererTar;
	afx_msg void OnStnClickedStaticSelRendererTar();
	int m_iGameSel;
};
